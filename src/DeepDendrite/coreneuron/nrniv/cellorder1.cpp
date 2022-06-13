#include <stdio.h>
#include <iostream>
#include "coreneuron/nrniv/nrn_assert.h"
#include "coreneuron/nrniv/cellorder.h"
#include "coreneuron/nrniv/tnode.h"
#include "coreneuron/nrnoc/multicore.h"

// just for use_interleave_permute
#include "coreneuron/nrniv/nrniv_decl.h"

#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <string.h>

using namespace std;
namespace coreneuron {
static size_t groupsize = 32;

class EdgeCut
{
public:
	TNode *tail, *head;
	int dc, tail_id, head_id, cut_id;
	EdgeCut* parent;

	//vector<tagCut*> children;
};

static bool tnode_earlier(TNode* a, TNode* b) {
    bool result = false;
    if (a->treesize < b->treesize) {  // treesize dominates
        result = true;
    } else if (a->treesize == b->treesize) {
        if (a->hash < b->hash) {  // if treesize same, keep identical trees together
            result = true;
        } else if (a->hash == b->hash) {
            result = a->nodeindex < b->nodeindex;  // identical trees ordered by nodeindex
        }
    }
    return result;
}

static bool ptr_tnode_earlier(TNode* a, TNode* b) {
    return tnode_earlier(a, b);
}

TNode::TNode(int ix) {
    nodeindex = ix;
    cellindex = 0;
    groupindex = -1;
    level = 0;
    hash = 0;
    treesize = 1;
    nodevec_index = 0;
    treenode_order = 0;
    branchindex = -1;
    nodeorder = -1;
    parent = NULL;
    children.reserve(2);
}

TNode::~TNode() {
}

size_t TNode::mkhash() {  // call on all nodes in leaf to root order
    // concept from http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
    std::sort(children.begin(), children.end(), ptr_tnode_earlier);
    hash = children.size();
    treesize = 1;
    for (size_t i = 0; i < children.size(); ++i) {  // need sorted by child hash
        hash ^= children[i]->hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        treesize += children[i]->treesize;
    }
    return hash;  // hash of leaf nodes is 0
}

static void tree_analysis(int* parent, int nnode, int ncell, VecTNode&);
static void node_interleave_order(int ncell, VecTNode&);
static void dynamic_interleave(int ncell, VecTNode &nodevec, int nnode, int nthread_per_cell, int*& prev_node, int*& next_node);
static int dynamic_interleave2(int ncell, VecTNode &nodevec, int nnode, int nthread_per_cell, int*& prev_node, int*& next_node);
static int primary_branch_split(int ncell, VecTNode &nodevec, int nnode, int nthread_per_cell, int*& prev_node, int*& next_node);
static void tree_partitioning(int ncell, VecTNode& nodevec, int ncut, int nnode);
static void node_interleave_order_warp(int ncell, VecTNode& nodevec, int nnode);
static void admin_warp(int ncell,
                       VecTNode& nodevec,
                       int*& firstnode,
                       int*& lastnode,
                       int*& max_level_per_thread,
                       int*& min_level_per_thread,
                       int*& prev_node,
                       int*& next_node,
                       int*& children_num,
                       int& max_level,
                       int& nthread_each_cell,
                       int& threads_num);
static void admin_dynamic(int branch_per_cell,
                   int ncell,
                   VecTNode& nodevec,
                   int& norder,
                   int*& prev_node_nodeindex,
                   int*& next_node_nodeindex,
                   int*& prev_node,
                   int*& next_node,
                   int*& max_order_per_thread,
                   int*& min_order_per_thread,
                   int*& firstnode,
                   int*& lastnode,
                   int*& stride,
                   int* nodeorder);

static void admin_tp(int branch_per_cell,
                   int ncell,
                   VecTNode& nodevec,
                   int& norder,
                   int*& prev_node,
                   int*& next_node,
                   int*& max_order_per_thread,
                   int*& min_order_per_thread,
                   int*& firstnode,
                   int*& lastnode);

static void admin_dynamic2(int branch_per_cell,
                        int ncell,
                        int nbranch,
                        VecTNode& nodevec,
                        int& norder,
                        int*& max_order_per_thread,
                        int*& min_order_per_thread,
                        int*& firstnode, 
                        int*& lastnode,
                        int*& stride,
                        int*& map_t2c,
                        int* prev_node_nodeindex,
                        int* next_node_nodeindex,
                        int*& prev_node,
                        int*& next_node,
                        int* nodeorder);
static void admin1(int ncell,
                   VecTNode& nodevec,
                   int& nwarp,
                   int& nstride,
                   int*& stride,
                   int*& firstnode,
                   int*& lastnode,
                   int*& cellsize);
static void admin2(int ncell,
                   VecTNode& nodevec,
                   int& nwarp,
                   int& nstride,
                   int*& stridedispl,
                   int*& strides,
                   int*& rootbegin,
                   int*& nodebegin,
                   int*& ncycles);
static void check(VecTNode&);
static void prtree(VecTNode&);

typedef std::pair<TNode*, int> TNI;
typedef std::map<size_t, pair<TNode*, int> > HashCnt;
typedef vector<TNI> TNIVec;

static char* stree(TNode* nd) {
    char s[1000];

    if (nd->treesize > 100) {
        return strdup("");
    }
    s[0] = '(';
    s[1] = '\0';
    for (size_t i = 0; i < nd->children.size(); ++i) {  // need sorted by child hash
        char* sr = stree(nd->children[i]);
        strcat(s, sr);
        free(sr);
    }
    strcat(s, ")");
    return strdup(s);
}

/*
assess the quality of the ordering. The measure is the size of a contiguous
list of nodes whose parents have the same order. How many contiguous lists
have that same size. How many nodes participate in that size list.
Modify the quality measure from experience with performance. Start with
list of (nnode, size_participation)
*/
static void quality(VecTNode& nodevec, size_t max = 32) {
    size_t qcnt = 0;  // how many contiguous nodes have contiguous parents

    // first ncell nodes are by definition in contiguous order
    for (size_t i = 0; i < nodevec.size(); ++i) {
        if (nodevec[i]->parent != NULL) {
            break;
        }
        qcnt += 1;
    }
    size_t ncell = qcnt;

    // key is how many parents in contiguous order
    // value is number of nodes that participate in that
    map<size_t, size_t> qual;
    size_t ip_last = 10000000000;
    for (size_t i = ncell; i < nodevec.size(); ++i) {
        size_t ip = nodevec[i]->parent->nodevec_index;
        // i%max == 0 means that if we start a warp with 8 and then have 32
        // the 32 is broken into 24 and 8. (modify if the arrangement during
        // gaussian elimination becomes more sophisticated.(
        if (ip == ip_last + 1 && i % max != 0) {  // contiguous
            qcnt += 1;
        } else {
            if (qcnt == 1) {
                // printf("unique %ld p=%ld ix=%d\n", i, ip, nodevec[i]->nodeindex);
            }
            qual[max] += (qcnt / max) * max;
            size_t x = qcnt % max;
            if (x) {
                qual[x] += x;
            }
            qcnt = 1;
        }
        ip_last = ip;
    }
    qual[max] += (qcnt / max) * max;
    size_t x = qcnt % max;
    if (x) {
        qual[x] += x;
    }

    // print result
    qcnt = 0;
#if 0
  for (map<size_t, size_t>::iterator it = qual.begin(); it != qual.end(); ++it) {
    qcnt += it->second;
    printf("%6ld %6ld\n", it->first, it->second);
  }
#endif
#if 0
  printf("qual.size=%ld  qual total nodes=%ld  nodevec.size=%ld\n",
    qual.size(), qcnt, nodevec.size());
#endif

    // how many race conditions. ie refer to same parent on different core
    // of warp (max cores) or parent in same group of max.
    size_t maxip = ncell;
    size_t nrace1 = 0;
    size_t nrace2 = 0;
    set<size_t> ipused;
    for (size_t i = ncell; i < nodevec.size(); ++i) {
        TNode* nd = nodevec[i];
        size_t ip = nd->parent->nodevec_index;
        if (i % max == 0) {
            maxip = i;
            ipused.clear();
        }
        if (ip >= maxip) {
            nrace1 += 1;
        } /*else*/
        {
            if (ipused.find(ip) != ipused.end()) {
                nrace2 += 1;
                if (ip >= maxip) {
                    // printf("race for parent %ld (parent in same group as multiple users))\n",
                    // ip);
                }
            } else {
                ipused.insert(ip);
            }
        }
    }
#if 0
  printf("nrace = %ld (parent in same group of %ld nodes)\n", nrace1, max);
  printf("nrace = %ld (parent used more than once by same group of %ld nodes)\n", nrace2, max);
#endif
}

size_t level_from_root(VecTNode& nodevec) {
    size_t maxlevel = 0;
    for (size_t i = 0; i < nodevec.size(); ++i) {
        TNode* nd = nodevec[i];
        if (nd->parent) {
            nd->level = nd->parent->level + 1;
            if (maxlevel < nd->level) {
                maxlevel = nd->level;
            }
        } else {
            nd->level = 0;
        }
    }
    return maxlevel;
}

size_t level_from_leaf(VecTNode& nodevec) {
    size_t maxlevel = 0;
    for (size_t i = nodevec.size() - 1; true; --i) {
        TNode* nd = nodevec[i];
        size_t lmax = 0;
        for (size_t ichild = 0; ichild < nd->children.size(); ++ichild) {
            if (lmax <= nd->children[ichild]->level) {
                lmax = nd->children[ichild]->level + 1;
            }
        }
        nd->level = lmax;
        if (maxlevel < lmax) {
            maxlevel = lmax;
        }
        if (i == 0) {
            break;
        }
    }
    return maxlevel;
}

static void set_cellindex(int ncell, VecTNode& nodevec) {
    for (int i = 0; i < ncell; ++i) {
        nodevec[i]->cellindex = i;
    }
    for (size_t i = 0; i < nodevec.size(); ++i) {
        TNode& nd = *nodevec[i];
        for (size_t j = 0; j < nd.children.size(); ++j) {
            TNode* cnode = nd.children[j];
            cnode->cellindex = nd.cellindex;
        }
    }
}

static void set_groupindex(VecTNode& nodevec) {
    for (size_t i = 0; i < nodevec.size(); ++i) {
        TNode* nd = nodevec[i];
        if (nd->parent) {
            nd->groupindex = nd->parent->groupindex;
        } else {
            nd->groupindex = i / groupsize;
        }
    }
}

#if 0
#define MSS MSS_ident_stat
typedef map<size_t, size_t> MSS;
static bool vsmss_comp(const pair<size_t, MSS*>& a, const pair<size_t, MSS*>& b) {
  bool result = false;
  const MSS::iterator& aa = a.second->begin();
  const MSS::iterator& bb = b.second->begin();
  if (aa->first < bb->first) {
    result = true;
  }else if (aa->first == bb->first) {
    if (aa->second < bb->second) {
      result = true;
    }
  }
  return result;
}
#endif

// how many identical trees and their levels
// print when more than one instance of a type
// reverse the sense of levels (all leaves are level 0) to get a good
// idea of the depth of identical subtrees.
static void ident_statistic(VecTNode& nodevec, size_t ncell) {
    // reverse sense of levels
    //  size_t maxlevel = level_from_leaf(nodevec);
    size_t maxlevel = level_from_root(nodevec);

    // # in each level
    vector<vector<size_t> > n_in_level(maxlevel + 1);
    for (size_t i = 0; i <= maxlevel; ++i) {
        n_in_level[i].resize(ncell / groupsize);
    }
    for (size_t i = 0; i < nodevec.size(); ++i) {
        n_in_level[nodevec[i]->level][nodevec[i]->groupindex]++;
    }
    printf("n_in_level.size = %ld\n", n_in_level.size());
    for (size_t i = 0; i < n_in_level.size(); ++i) {
        printf("%5ld\n", i);
        for (size_t j = 0; j < n_in_level[i].size(); ++j) {
            printf(" %5ld", n_in_level[i][j]);
        }
        printf("\n");
    }

#if 0
  typedef map<size_t, MSS> MSMSS;
  typedef vector<pair<size_t, MSS*> > VSMSS;
  MSMSS info;
  for (size_t i=0; i < nodevec.size(); ++i) {
    TNode* nd = nodevec[i];
    info[nd->hash][nd->level]++;
  }

  VSMSS vinfo;
  for (MSMSS::iterator i = info.begin(); i != info.end(); ++i) {
    vinfo.push_back(pair<size_t, MSS*>(i->first, &(i->second)));
  }
  std::sort(vinfo.begin(), vinfo.end(), vsmss_comp);

  for (VSMSS::iterator i = vinfo.begin(); i < vinfo.end(); ++i) {
    MSS* ival = i->second;
    if (ival->size() > 1 || ival->begin()->second > 8) {
      printf("hash %ld", i->first);
      for (MSS::iterator j = ival->begin(); j != ival->end(); ++j) {
        printf(" (%ld, %ld)", j->first, j->second);
      }
      printf("\n");
    }
  }
  printf("max level = %ld\n", maxlevel);
#endif
}
#undef MSS

int* node_order_branch(int ncell,
                int nnode,
                int& branch_per_cell,
                int* parent,
                int& norder,
                int& nthread,
                int*& prev_node,
                int*& next_node,
                int*& max_order_per_thread,
                int*& min_order_per_thread,
                int*& firstnode,
                int*& lastnode,
                int*& stride,
                int*& map_t2c,
                int*& stridedispl) {
    VecTNode nodevec;
    if (0)
        prtree(nodevec);  // avoid unused warning

    // nodevec[0:ncell] in increasing size, with identical trees together,
    // and otherwise nodeindex order
    tree_analysis(parent, nnode, ncell, nodevec);
    check(nodevec);

    set_cellindex(ncell, nodevec);
    set_groupindex(nodevec);
    level_from_root(nodevec);

    FILE *fp = fopen("parent_array", "wb");
    fwrite(&ncell, sizeof(int), 1, fp);
    fwrite(&nnode, sizeof(int), 1, fp);
    fwrite(parent, sizeof(int), nnode, fp);
    fclose(fp);
    int *prev_nodeindex = new int[nnode];
    int *next_nodeindex = new int[nnode];
    int nbranch = 0;
    nbranch = dynamic_interleave2(ncell, nodevec, nnode, branch_per_cell, prev_nodeindex, next_nodeindex);
    //nbranch = primary_branch_split(ncell, nodevec, nnode, branch_per_cell, prev_nodeindex, next_nodeindex);
    //tree_partitioning(ncell, nodevec, branch_per_cell, nnode);
    //nbranch = branch_per_cell * ncell;
    //node_interleave_order_warp(ncell, nodevec, nnode);
    nthread = nbranch;
    for (int ithread = 0; ithread < nrn_nthread; ithread++)
    {
        NrnThread *nt = nrn_threads + ithread;
        if (nt->ncell > 0)
        {
            if (nt->resetv_each_cell != NULL)
            {
                for (int inode = 0; inode < nnode; inode++)
                {
                    
                    nodevec[inode]->reset_v = nt->resetv_each_cell[nodevec[inode]->cellindex];
                    nt->resetv_each_node[inode] = nodevec[inode]->reset_v;
                }
            }
        }
    }
    // nodevec[ncell:nnode] cells are interleaved in nodevec[0:ncell] cell order
    //node_interleave_order_branch(ncell, nodevec, branch_per_cell, nnode, prev_nodeindex, next_nodeindex);
        //node_interleave_order_dfs(ncell, nodevec);
        //node_interleave_order(ncell, nodevec);
    check(nodevec);

#if 0
  for (int i=0; i < ncell; ++i) {
    TNode& nd = *nodevec[i];
    printf("%d size=%ld hash=%ld ix=%d\n", i, nd.treesize, nd.hash, nd.nodeindex);
  }
#endif

    if (0)
        ident_statistic(nodevec, ncell);
    quality(nodevec);

    //fp = fopen("cell0_node_3.txt", "w");
    // the permutation
    int* nodeorder = new int[nnode];
    for (int i = 0; i < nnode; ++i) {
        TNode& nd = *nodevec[i];
        nodeorder[nd.nodeindex] = i;
    }
    //fclose(fp);
    fp = fopen("order3", "wb");
    fwrite(nodeorder, sizeof(int), nnode, fp);
    fclose(fp);

    // administrative statistics for gauss elimination
    //admin_branch(branch_per_cell, ncell, nodevec, norder, prev_nodeindex, next_nodeindex, prev_node, next_node, 
    //            max_order_per_thread, min_order_per_thread, firstnode, lastnode, cellsize, nodeorder);
    //admin_dynamic(branch_per_cell, ncell, nodevec, norder, prev_nodeindex, next_nodeindex, prev_node, next_node, 
    //            max_order_per_thread, min_order_per_thread, firstnode, lastnode, stride, nodeorder);
    admin_dynamic2(branch_per_cell, ncell, nbranch, nodevec, norder, max_order_per_thread, min_order_per_thread, firstnode, lastnode, stride, map_t2c, prev_nodeindex, next_nodeindex, prev_node, next_node, nodeorder);
    //admin_tp(branch_per_cell, ncell, nodevec, norder, prev_node, next_node, 
    //    max_order_per_thread, min_order_per_thread, firstnode, lastnode);
    //int* children_num = NULL;
    //admin_warp(ncell, nodevec, firstnode, lastnode, max_order_per_thread, min_order_per_thread, 
    //           prev_node, next_node, children_num, norder, branch_per_cell, nthread);
    //printf("after admin\n");
    //if (children_num)
    //    delete[] children_num;
    

    if (prev_nodeindex)
    {
        delete []prev_nodeindex;
        delete []next_nodeindex;
        prev_nodeindex = NULL;
        next_nodeindex = NULL;
    }
        
#if 1
    int ntopol = 1;
    for (int i = 1; i < ncell; ++i) {
        if (nodevec[i - 1]->hash != nodevec[i]->hash) {
            ntopol += 1;
        }
    }
    printf("%d distinct tree topologies\n", ntopol);
#endif

    for (size_t i = 0; i < nodevec.size(); ++i) {
        //printf("%d:%p\n", i, nodevec[i]);
        delete nodevec[i];     
    }
    printf("delete nodevec finish\n");
    return nodeorder;
}

// for cells with same size, keep identical trees together

// parent is (unpermuted)  nnode length vector of parent node indices.
// return a permutation (of length nnode) which orders cells of same
// size so that identical trees are grouped together.
// Note: cellorder[ncell:nnode] are the identify permutation.

int* node_order(int ncell,
                int nnode,
                int* parent,
                int& nwarp,
                int& nstride,
                int*& stride,
                int*& firstnode,
                int*& lastnode,
                int*& cellsize,
                int*& stridedispl) {
    VecTNode nodevec;
    if (0)
        prtree(nodevec);  // avoid unused warning

    // nodevec[0:ncell] in increasing size, with identical trees together,
    // and otherwise nodeindex order
    tree_analysis(parent, nnode, ncell, nodevec);
    check(nodevec);

    set_cellindex(ncell, nodevec);
    set_groupindex(nodevec);
    level_from_root(nodevec);

    // nodevec[ncell:nnode] cells are interleaved in nodevec[0:ncell] cell order
    if (use_interleave_permute == 1) {
        node_interleave_order(ncell, nodevec);
    } else {
        group_order2(nodevec, groupsize, ncell);
    }
    check(nodevec);

    for (int ithread = 0; ithread < nrn_nthread; ithread++)
    {
        NrnThread *nt = nrn_threads + ithread;
        if (nt->ncell > 0 && nt->resetv_each_cell != NULL)
        {
            for (int inode = 0; inode < nnode; inode++)
            {
                nodevec[inode]->reset_v = nt->resetv_each_cell[nodevec[inode]->cellindex];
                nt->resetv_each_node[inode] = nodevec[inode]->reset_v;
            }
        }
    }

#if 0
  for (int i=0; i < ncell; ++i) {
    TNode& nd = *nodevec[i];
    printf("%d size=%ld hash=%ld ix=%d\n", i, nd.treesize, nd.hash, nd.nodeindex);
  }
#endif

    if (0)
        ident_statistic(nodevec, ncell);
    quality(nodevec);

    // the permutation
    int* nodeorder = new int[nnode];
    for (int i = 0; i < nnode; ++i) {
        TNode& nd = *nodevec[i];
        nodeorder[nd.nodeindex] = i;
    }

    FILE *fp = fopen("order1", "wb");
    fwrite(nodeorder, sizeof(int), nnode, fp);
    fclose(fp);
    // administrative statistics for gauss elimination
    if (use_interleave_permute == 1) {
        admin1(ncell, nodevec, nwarp, nstride, stride, firstnode, lastnode, cellsize);
    } else {
        //  admin2(ncell, nodevec, nwarp, nstride, stridedispl, stride, rootbegin, nodebegin,
        //  ncycles);
        admin2(ncell, nodevec, nwarp, nstride, stridedispl, stride, firstnode, lastnode, cellsize);
    }

    int ntopol = 1;
    for (int i = 1; i < ncell; ++i) {
        if (nodevec[i - 1]->hash != nodevec[i]->hash) {
            ntopol += 1;
        }
    }
#ifdef DEBUG
    printf("%d distinct tree topologies\n", ntopol);
#endif

    for (size_t i = 0; i < nodevec.size(); ++i) {
        delete nodevec[i];
    }

    return nodeorder;
}

void check(VecTNode& nodevec) {
    // printf("check\n");
    size_t nnode = nodevec.size();
    size_t ncell = 0;
    for (size_t i = 0; i < nnode; ++i) {
        nodevec[i]->nodevec_index = i;
        if (nodevec[i]->parent == NULL) {
            ncell++;
        }
    }
    for (size_t i = 0; i < ncell; ++i) {
        nrn_assert(nodevec[i]->parent == NULL);
    }
    for (size_t i = ncell; i < nnode; ++i) {
        TNode& nd = *nodevec[i];
        if (nd.parent->nodevec_index >= nd.nodevec_index) {
            printf("error i=%ld nodevec_index=%ld parent=%ld\n", i, nd.nodevec_index,
                   nd.parent->nodevec_index);
        }
        nrn_assert(nd.nodevec_index > nd.parent->nodevec_index);
    }
}

void prtree(VecTNode& nodevec) {
    size_t nnode = nodevec.size();
    for (size_t i = 0; i < nnode; ++i) {
        nodevec[i]->nodevec_index = i;
    }
    for (size_t i = 0; i < nnode; ++i) {
        TNode& nd = *nodevec[i];
        printf("%ld p=%d   c=%ld l=%ld o=%ld   ix=%d pix=%d\n", i,
               nd.parent ? int(nd.parent->nodevec_index) : -1, nd.cellindex, nd.level,
               nd.treenode_order, nd.nodeindex, nd.parent ? int(nd.parent->nodeindex) : -1);
    }
}

void tree_analysis(int* parent, int nnode, int ncell, VecTNode& nodevec) {
    //  VecTNode nodevec;

    // create empty TNodes (knowing only their index)
    nodevec.reserve(nnode);
    for (int i = 0; i < nnode; ++i) {
        nodevec.push_back(new TNode(i));
    }

    // determine the (sorted by hash) children of each node
    for (int i = nnode - 1; i >= ncell; --i) {
        nodevec[i]->parent = nodevec[parent[i]];
        nodevec[i]->mkhash();
        nodevec[parent[i]]->children.push_back(nodevec[i]);
    }

    // determine hash of the cells
    for (int i = 0; i < ncell; ++i) {
        nodevec[i]->mkhash();
    }

    std::sort(nodevec.begin(), nodevec.begin() + ncell, tnode_earlier);
}

static bool interleave_comp(TNode* a, TNode* b) {
    bool result = false;
    if (a->treenode_order < b->treenode_order) {
        result = true;
    } else if (a->treenode_order == b->treenode_order) {
        if (a->cellindex < b->cellindex) {
            result = true;
        }
    }
    return result;
}

// sort so nodevec[ncell:nnode] cell instances are interleaved. Keep the
// secondary ordering with respect to treenode_order so each cell is still a tree.

void node_interleave_order(int ncell, VecTNode& nodevec) {
    int* order = new int[ncell];
    for (int i = 0; i < ncell; ++i) {
        order[i] = 0;
        nodevec[i]->treenode_order = order[i]++;
    }
    for (size_t i = 0; i < nodevec.size(); ++i) {
        TNode& nd = *nodevec[i];
        for (size_t j = 0; j < nd.children.size(); ++j) {
            TNode* cnode = nd.children[j];
            cnode->treenode_order = order[nd.cellindex]++;
        }
    }
    delete[] order;

    //  std::sort(nodevec.begin() + ncell, nodevec.end(), contig_comp);
    std::sort(nodevec.begin() + ncell, nodevec.end(), interleave_comp);

#if 0
  for (size_t i=0; i < nodevec.size(); ++i) {
    TNode& nd = *nodevec[i];
    printf("%ld cell=%ld ix=%d\n",  i, nd.cellindex, nd.nodeindex);
  }
#endif
}

void set_branch_by_leaf(TNode* root, int& leaf_num)
{
    if (root == NULL)
        return;
    root->branchindex = leaf_num;
    if (root->children.size() == 0)
        leaf_num++;
    int child_num = root->children.size();
    for (int i = 0; i < child_num; i++)
        set_branch_by_leaf(root->children[i], leaf_num);
}

static bool sort_by_level(TNode* a, TNode* b)
{
    bool result = false;
    if (a->cellindex < b->cellindex)
        result = true;
    else if (a->cellindex == b->cellindex)
    {
        if (a->level < b->level)
            result = true;
        else if (a->level == b->level)
        {
            if (a->branchindex < b->branchindex)
                result = true;
        }
    }
    return result;
}

void node_interleave_order_warp(int ncell, VecTNode& nodevec, int nnode)
{
    int* order = new int[ncell];
    for (int i = 0; i < ncell; i++)
    {
        int leaf_num = 0;
        set_branch_by_leaf(nodevec[i], leaf_num);
        nodevec[i]->leaf_num = leaf_num;
    }

    std::sort(nodevec.begin() + ncell, nodevec.end(), sort_by_level);
    for (int i = 0; i < nnode; i++)
        nodevec[i]->current_index = i;
}

static bool sort_by_branch_order(TNode* a, TNode* b)
{
    bool result = false;
    if (a->groupindex < b->groupindex)
    {
        result = true;
    }
    else if (a->groupindex == b->groupindex)
    {
        if (a->nodeorder < b->nodeorder)
        {
            result = true;
        }
        else if (a->nodeorder == b->nodeorder)
        {
            if (a->branchindex < b->branchindex)
            {
                result = true;
            }
            else if (a->branchindex == b->branchindex)
            {
                if (a->cellindex > b->cellindex)
                    result = true;
                else if (a->cellindex == b->cellindex)
                    return a->nodeindex < b->nodeindex;
            }
        }
    }
    
    return result;
}

void modify_orders(TNode *root, int number_to_add)
{
    int start = 1, index = 0;
    queue <TNode*> q;
    q.push(root);

    while (!q.empty())
    {
        TNode *nd = q.front();
        q.pop();
        if (!start)
            nd->nodeorder += number_to_add;
        else
            start = 0;
        for (int ichild = 0, nchild = nd->children.size(); ichild < nchild; ichild++)
        {
            q.push(nd->children[ichild]);
        }
        index++;
    }
}

void modify_branchindex(TNode* root, int* sorted_branchindex, int* branch_size)
{
    queue<TNode*> q;
    q.push(root);
    while (!q.empty())
    {
        TNode *nd = q.front();
        q.pop();
        nd->branch_size = branch_size[nd->branchindex];
        nd->branchindex = sorted_branchindex[nd->branchindex];
        for (int ichild = 0, nchild = nd->children.size(); ichild < nchild; ichild++)
        {
            q.push(nd->children[ichild]);
        }
    }
}

void argsort(int* a, int len, int* idx)
{
    multimap< int, int > m;
    multimap<int, int>::iterator iter;
    for (int i = 0; i < len; i++)
    {
        m.insert(pair<int, int>(a[i], i));
    }
    
    int index = len - 1;
    for (iter = m.begin(); iter != m.end(); iter++)
    {
        idx[iter->second] = index;
        //printf("first:%d second:%d sorted:%d\n", iter->first, iter->second, len-index-1);
        index--;
    }
}

struct CompareNode : public std::binary_function<TNode*, TNode*, bool>
{
	bool operator()(const TNode* a, const TNode* b) const
	{
		bool result = false;
		if (a->level < b->level)
			result = true;
		else if (a->level == b->level)
		{
			return a->current_index > b->current_index;
		}
		return result;
	}
};

void init_tree(VecTNode nodevec, int icell, TNode **tree_nodes)
{
	queue<TNode*> q;
	q.push(nodevec[icell]);
	TNode *nd;
	int index = 0;
	while (!q.empty())
	{
		nd = q.front();
		q.pop();
		nd->src_dc = nd->down_component = nd->treesize;
		nd->current_index = index;
        tree_nodes[index] = nd;
		index++;
		for (int i = 0, j = nd->children.size(); i < j; i++)
			q.push(nd->children[i]);
	}
}

bool need_split(VecTNode nodevec, int icell, int nthread_per_cell)
{
    queue<TNode*> q;
    TNode *nd;
    q.push(nodevec[icell]);
    bool result = false;
    int total_branch = 0;
    while (!q.empty())
    {
        nd = q.front();
        q.pop();
        int c_size = nd->children.size();
        /*if (c_size >= nthread_per_cell)
        {
            result = true;
            break;
        }*/
        if (c_size == 0)
            total_branch++;
        if (total_branch >= nthread_per_cell)
        {
            result = true;
            break;
        }
        for (int i = 0; i < c_size; i++)
            q.push(nd->children[i]);
    }
    return result;
}

/* s_branch_id: in-warp start thread id of cell
 * iwarp: warp id
 * 
 */
void split_cell(VecTNode &nodevec, int icell, int nthread_per_cell, int s_branch_id, int iwarp,
                int *branch_size, int &max_order, int* prev_node, int* next_node)
{
    int cell_size = (int)nodevec[icell]->treesize;
    int order = 0, max_level = -1;
    TNode **tree_nodes = new TNode*[cell_size];
    TNode **node_to_compute = new TNode*[nthread_per_cell];
    TNode **prev_compute_node = new TNode*[nthread_per_cell];
    init_tree(nodevec, icell, tree_nodes);

    max_order = -1;
    int *children_left = new int[cell_size];
    priority_queue< TNode*, vector<TNode*>, CompareNode > pq;
    for (int inode = 0; inode < cell_size; inode++)
    {
        children_left[inode] = (int)tree_nodes[inode]->children.size();
        if ((int)tree_nodes[inode]->level > max_level)
            max_level = (int)tree_nodes[inode]->level;
        if (children_left[inode] == 0)
        {
            pq.push(tree_nodes[inode]);
        }
    }

    TNode *nd = NULL;
    while (!pq.empty())
    {
        int nnode_to_compute = 0;
        for (int ithread = 0; ithread < nthread_per_cell; ithread++)
        {
            if (pq.empty())
                break;
            nd = pq.top();
            pq.pop();
            node_to_compute[nnode_to_compute++] = nd;
        }
        if (nnode_to_compute == 0)
            break;
        //printf("order:%d\n", order);
        for (int ithread = 0; ithread < nnode_to_compute; ithread++)
        {
            nd = node_to_compute[ithread];
            nd->branchindex = s_branch_id + ithread;
            nd->nodeorder = order;
            nd->groupindex = iwarp;

            branch_size[(int)nd->branchindex]++;
            if (order > 0)
            {
                next_node[nd->nodeindex] = prev_compute_node[ithread]->nodeindex;
                prev_node[prev_compute_node[ithread]->nodeindex] = nd->nodeindex;
                //printf("prev_nodeindex:%d prev_currentindex:%d\n", prev_compute_node[ithread]->nodeindex, prev_compute_node[ithread]->current_index);
            }
            //printf("nodeindex:%d current_index:%d ", nd->nodeindex, nd->current_index);
            prev_compute_node[ithread] = nd;

            if (nd->parent)
            {
                children_left[nd->parent->current_index]--;
                if (children_left[nd->parent->current_index] == 0)
                    pq.push(nd->parent);
            }
        }

        //printf("\n");
        if (order > max_order)
            max_order = order;
        order++;
        //max_order = nd->nodeorder;
    }

    //printf("cell:%d order:%d max_level:%d\n", icell, max_order, max_level);
    for (int inode = 0; inode < cell_size; inode++)
    {
        tree_nodes[inode]->nodeorder = max_order - tree_nodes[inode]->nodeorder;
    }
    delete []node_to_compute;
    delete []prev_compute_node;
    delete []tree_nodes;
    delete []children_left;
    
}

void set_branch_info(TNode *root, int branchindex, int groupindex)
{
    //int cellsize = (int)root->treesize;
    int order = 0;
    TNode *nd = NULL;
    queue<TNode*> q;
    q.push(root);

    while (!q.empty())
    {
        nd = q.front();
        q.pop();
        nd->nodeorder = order;
        nd->branchindex = branchindex;
        nd->groupindex = groupindex;
        order++;
        for (int i = 0, j = nd->children.size(); i < j; i++)
            q.push(nd->children[i]);
    }
}

static int dynamic_interleave2(int ncell, VecTNode &nodevec, int nnode, int nthread_per_cell, int*& prev_node, int*& next_node)
{
    int istart = 0, iend = 0;
    int used_nthread = 0, max_order = -1;
    int s_branch_id = 0, iwarp = 0;
    int nbranch = 0;
    bool *split = NULL;
	memset(prev_node, -1, sizeof(int) * nnode);
	memset(next_node, -1, sizeof(int) * nnode);
    split = new bool[ncell];
    printf("ncell:%d\n", ncell);
    for (int i = 0; i < ncell; i++)
        split[i] = false;

    while (iend < ncell)
    {
        used_nthread = 0;
        max_order = -1;
        int branch_size[32];
        for (int i = 0; i < 32; i++)
        {
            branch_size[i] = 0;
        }
        //group cells into different warps 
        for (iend = istart; iend < ncell; iend++)
        {
            split[iend] = need_split(nodevec, iend, nthread_per_cell);
            //printf("icell:%d split:%d used_thread:%d size:%d\n", iend, split[iend], used_nthread, nodevec[iend]->treesize);
            if (!split[iend])
                used_nthread++;
            else
                used_nthread += nthread_per_cell;
            if (used_nthread > 32)
                break;
            else if (used_nthread == 32)
            {
                iend++;
                break;
            }
        }

        s_branch_id = 0;
        //printf("iwarp:%d istart:%d iend:%d\n", iwarp, istart, iend);
        for (int icell = istart; icell < iend; icell++)
        {
            //printf("\ticell:%d size:%d split:%d s_branch:%d\n", icell, nodevec[icell]->treesize, split[icell], s_branch_id);
            if (split[icell])
            {
                split_cell(nodevec, icell, nthread_per_cell, s_branch_id, iwarp, branch_size, max_order, prev_node, next_node);
                s_branch_id += nthread_per_cell;
                nbranch += nthread_per_cell;
            }
            else
            {
                set_branch_info(nodevec[icell], s_branch_id, iwarp);
                s_branch_id++;
                nbranch++;
            }
        }
        int sorted_branchindex[32];
        // sort by branch size from large to small
        argsort(branch_size, 32, sorted_branchindex);
        for (int icell = istart; icell < iend; icell++)
        {
            modify_branchindex(nodevec[icell], sorted_branchindex, branch_size);
        }
        istart = iend;
        iwarp++;
    }

    // align nodes in cells with different sizes
    int *max_order_per_cell = NULL, *max_order_per_group = NULL;
    int nwarp = iwarp;
    printf("nwarps:%d nbranch:%d\n", nwarp, nbranch);
    max_order_per_cell = new int[ncell];
    max_order_per_group = new int[nwarp];
    memset(max_order_per_cell, -1, sizeof(int) * ncell);
    memset(max_order_per_group, -1, sizeof(int) * nwarp);

    for (int i = 0; i < nnode; i++)
    {
        TNode *nd = nodevec[i];
        if (nd->nodeorder > max_order_per_cell[nd->cellindex])
            max_order_per_cell[nd->cellindex] = nd->nodeorder;
        if (nd->nodeorder > max_order_per_group[nd->groupindex])
            max_order_per_group[nd->groupindex] = nd->nodeorder;
    }
    for (int icell = 0; icell < ncell; icell++)
    {
        TNode *nd = nodevec[icell];
        int diff = max_order_per_group[nd->groupindex] - max_order_per_cell[nd->cellindex];
        if (diff > 0)
            modify_orders(nd, diff);
    }

    delete[] max_order_per_cell;
    delete[] max_order_per_group;
    delete[] split;

    for (int i = 0; i < nnode; i++)
    {
        if (prev_node[i] > nnode || prev_node[i] < -1)
            printf("i:%d prev_node:%d\n", i, prev_node[i]);
        if (next_node[i] > nnode || next_node[i] < -1)
            printf("i:%d next_node:%d\n", i, next_node[i]);
    }
	std::sort(nodevec.begin() + ncell, nodevec.end(), sort_by_branch_order);
    return nwarp * 32;
}

static void push_subtree(vector<TNode*> &vec, TNode* root, int ithread)
{
    queue<TNode*> q;
    q.push(root);
    TNode* nd = NULL;
    while (!q.empty())
    {
        nd = q.front();
        q.pop();
        if (nd == NULL)
            continue;
        nd->branchindex = ithread;
        vec.push_back(nd);
        for (int i = 0, n = nd->children.size(); i < n; i++)
            q.push(nd->children[i]);
    }
}

static void set_branchindex_and_nodeorder(vector<TNode*>& node_vec, int*& prev_node, int*& next_node, 
                                          int max_group_size, int ncell_each_warp, int ithread) 
{
    int order2add = max_group_size - node_vec.size();
    int nthread_each_cell = 32 / ncell_each_warp;
    int order = 0;
    for (int inode = 0, nnode = node_vec.size(); inode < nnode; inode++)
    {
        TNode* nd = node_vec[inode];
        nd->groupindex = nd->cellindex / ncell_each_warp;
        nd->branchindex = nd->cellindex % ncell_each_warp * nthread_each_cell + ithread;
        nd->nodeorder = order + order2add;
        order++;
        if (inode > 0)
            prev_node[nd->nodeindex] = node_vec[inode - 1]->nodeindex;
        if (inode < nnode - 1)
            next_node[nd->nodeindex] = node_vec[inode + 1]->nodeindex;
    }
}

static int primary_branch_split(int ncell, VecTNode &nodevec, int nnode, int nthread_each_cell, int*& prev_node, int*& next_node)
{
    int ncell_each_warp = 32 / nthread_each_cell;
    int max_icell, max_order = -1;
	memset(prev_node, -1, sizeof(int) * nnode);
	memset(next_node, -1, sizeof(int) * nnode);
    for (int icell = 0; icell < ncell; icell++)
    {
        TNode *root = NULL;
        TNode **tree_nodes = new TNode*[nodevec[icell]->treesize];
        init_tree(nodevec, icell, tree_nodes);
        int soma_nd_id_in_tree = -1;
        TNode* soma_nd = NULL;
        int max_nchild_branch = -1;

        // find the node with max number of children branch
        for (int i = 0; i < nodevec[icell]->treesize; i++)
        {
            int nchild_branch = 0;
            TNode *nd = tree_nodes[i];
            for (int ichild = 0, nchild = nd->children.size(); ichild < nchild; ichild++)
            {
                TNode *pchild = nd->children[ichild];
                if (pchild->treesize > 4) // spines are not branch
                    nchild_branch++;
            }
            if (nchild_branch > max_nchild_branch)
            {
                soma_nd_id_in_tree = i;
                soma_nd = nd;
                max_nchild_branch = nchild_branch;
            }
        }

        // partition each branch into groups
        //vector<int> group_size(nthread_each_cell, 0);
        vector<vector<TNode*>> node_by_group;
        node_by_group.resize(nthread_each_cell);
        int max_group_id = -1;
        int max_group_size = 0;
        for (int ibranch = 0, nbranch = soma_nd->children.size(); ibranch < nbranch; ibranch++)
        {
            int min_temp_size = 999999;
            int group_to_add = -1;
            
            // find a group with minimum node number, put current branch into this group
            for (int igroup = 0; igroup < nthread_each_cell; igroup++)
            {
                if (node_by_group[igroup].size() < min_temp_size)
                {
                    min_temp_size = node_by_group[igroup].size();
                    group_to_add = igroup;
                }
            }

            //printf("ibranch:%d group_to_add:%d branchsize:%d\n", ibranch, group_to_add, soma_nd->children[ibranch]->treesize);
            //head_by_group[group_to_add].push_back(soma_nd->children[ibranch]);
            int start = node_by_group[group_to_add].size();
            push_subtree(node_by_group[group_to_add], soma_nd->children[ibranch], group_to_add);
            //group_size[group_to_add] += soma_nd->children[ibranch]->treesize;
            if (node_by_group[group_to_add].size() > max_group_size)
            {
                max_group_size = node_by_group[group_to_add].size();
                max_group_id = group_to_add;
            }
        }

        // set branch index and nodeorder
        int nroot_node = 0;
        for (int i = 0; i < nodevec[icell]->treesize; i++)
        {
            TNode *nd = tree_nodes[i];
            if (nd->branchindex == -1)
            {
                node_by_group[max_group_id].emplace(node_by_group[max_group_id].begin() + nroot_node, nd);
                nroot_node++;
                max_group_size++;
            }
        }

        for (int igroup = 0; igroup < nthread_each_cell; igroup++)
        {
            set_branchindex_and_nodeorder(node_by_group[igroup], prev_node, next_node, max_group_size, ncell_each_warp, igroup);
        }
        /*int max_branch_order = 0;
        vector<int> norder_by_branch(nthread_each_cell, 0);
        for (int ithread = 0; ithread < nthread_each_cell; ithread++)
        {
            printf("ithread:%d groupsize:%d nhead:%d norder:%d\n", ithread, group_size[ithread], head_by_group[ithread].size(), norder_by_branch[ithread]);
            int order = 0;
            for (int ihead = 0, nhead = head_by_group[ithread].size(); ihead < nhead; ihead++)
            {
                //printf("ithread:%d ihead:%d head_nd:%p\n", ithread, ihead, head_by_group[ithread][ihead]);
                set_branchindex(head_by_group[ithread][ihead], icell, ncell_each_warp, ithread, prev_node, next_node, order);
            }
            max_branch_order = order > max_branch_order ? order : max_branch_order;
            norder_by_branch[ithread] = order;
        }

        int nroot_node = 0;
        for (int i = 1; i < nodevec[icell]->treesize; i++)
        {
            TNode *nd = tree_nodes[i];
            if (nd->branchindex == -1)
            {
                nd->nodeorder = nroot_node;
                nroot_node++;
                nd->branchindex = max_group_id;
                group_size[max_group_id]++;
                norder_by_branch[max_group_id]++;
            }
        }
        for (int ihead = 0, nhead = head_by_group[max_group_id].size(); ihead < nhead; ihead++)
        {
            head_by_group[max_group_id][ihead]->nodeorder += nroot_node;
            modify_orders(head_by_group[max_group_id][ihead], nroot_node);
        }

        for (int ithread = 0; ithread < nthread_each_cell; ithread++)
        {
            printf("ithread:%d groupsize:%d nhead:%d norder:%d\n", ithread, group_size[ithread], head_by_group[ithread].size(), norder_by_branch[ithread]);
            for (int ihead = 0, nhead = head_by_group[ithread].size(); ihead < nhead; ihead++)
            {
                head_by_group[ithread][ihead]->nodeorder += norder_by_branch[max_group_id] - norder_by_branch[ithread];
                modify_orders(head_by_group[ithread][ihead], norder_by_branch[max_group_id] - norder_by_branch[ithread]);
            }
        }*/
        delete[] tree_nodes;
        tree_nodes = NULL;
    }
    /*for (int inode = 0; inode < 100; inode++)
    {
        TNode* nd = nodevec[inode];
        TNode* par = nd->parent;
        printf("inode:%d nodeindex:%d prev:%d next:%d branch:%d order:%d ", inode, nd->nodeindex, prev_node[inode], next_node[inode], nd->branchindex, nd->nodeorder);
        if (par)
            printf("par:%d", par->nodeindex);
        printf("\n");
    }*/
	std::sort(nodevec.begin() + ncell, nodevec.end(), sort_by_branch_order);
    return ncell * nthread_each_cell;
}

void update_edge_cuts(EdgeCut** edge_cuts, int ncut, int* map_n2c)
{
	for (int icut = 0; icut < ncut; icut++)
	{
		edge_cuts[icut]->dc = edge_cuts[icut]->head->down_component;
		edge_cuts[icut]->tail_id = edge_cuts[icut]->tail->current_index;
		edge_cuts[icut]->head_id = edge_cuts[icut]->head->current_index;
		TNode *pd = edge_cuts[icut]->tail;
		while (pd)
		{
			if (pd->current_index > -1 && map_n2c[pd->current_index] != -1)
			{
				edge_cuts[icut]->parent = edge_cuts[map_n2c[pd->current_index]];
				break;
			}
			pd = pd->parent;
		}
		if (pd == NULL)
			edge_cuts[icut]->parent = NULL;
	}
}

void down_shift(EdgeCut** edge_cuts, int cut_to_shift, int *map_n2c, int cut_on_root, int ncut)
{
	TNode *nd = edge_cuts[cut_to_shift]->head;
	int temp = -1;
	int child_shift_to;
	for (int ichild = 0, nchild = nd->children.size(); ichild < nchild; ichild++)
	{
		if (map_n2c[nd->children[ichild]->current_index] == -1 && nd->children[ichild]->down_component > temp)
		{
			child_shift_to = ichild;
			temp = nd->children[ichild]->down_component;
		}
	}

	edge_cuts[cut_to_shift]->tail = nd;
	edge_cuts[cut_to_shift]->head = nd->children[child_shift_to];
	nd->down_component -= nd->children[child_shift_to]->down_component;
	nd->src_dc = nd->down_component;
	int dc_change = nd->children[child_shift_to]->down_component - edge_cuts[cut_to_shift]->dc;
	edge_cuts[cut_to_shift]->dc = edge_cuts[cut_to_shift]->head->down_component;
	if (edge_cuts[cut_to_shift]->parent == NULL)
	{
		if (cut_on_root > 0)
			map_n2c[nd->current_index] = ncut - cut_on_root;
		else
			map_n2c[nd->current_index] = -1;
	}
	else
		map_n2c[nd->current_index] = -1;


	map_n2c[nd->children[child_shift_to]->current_index] = cut_to_shift;

	TNode *pd = edge_cuts[cut_to_shift]->tail;
	bool flag = true;
	while (pd)
	{
		if (pd != edge_cuts[cut_to_shift]->tail && pd->down_component > -1)
		{
			pd->down_component -= dc_change;
			pd->src_dc = pd->down_component;
		}
		else if (pd->down_component <= -1)
			pd->src_dc -= dc_change;
		if (flag)
		{
			if (pd->children.size() > 1)
			{
				flag = false;
			}
			else
			{
				pd->down_component = -1;
			}
		}
		if (!flag && pd != edge_cuts[cut_to_shift]->tail && pd->down_component <= -1)
		{
			//pd->down_component = pd->children[0]->down_component + pd->weight;
			//pd->src_dc -= dc_change;
			pd->down_component = pd->src_dc;
		}
		if (pd->current_index <= -1 || map_n2c[pd->current_index] != -1)
			break;

		pd = pd->parent;

	}

	update_edge_cuts(edge_cuts, ncut, map_n2c);

}

int set_node_order(TNode *root, int *map_n2c, int order_start, int branch_id, int group_id)
{
	int order = 0;
	queue<TNode*> q;
	q.push(root);
	while (!q.empty())
	{
		TNode* nd = q.front();
		q.pop();
		if (map_n2c[nd->current_index] > -1 && order > 0)
		{
			//printf("nodeindex:%d cellindex:%d\n", nd->nodeindex, nd->cellindex);
			continue;
		}
		nd->nodeorder = order + order_start;
		nd->branchindex = branch_id;
		nd->groupindex = group_id;
		order++;
		for (int ichild = 0, nchild = nd->children.size(); ichild < nchild; ichild++)
		{
			if (map_n2c[nd->children[ichild]->current_index] <= -1)
				q.push(nd->children[ichild]);
		}
	}
	return order + order_start;
}

void sort_cuts(EdgeCut **edge_cuts, int ncut)
{
	for (int i = 0; i < ncut; i++)
	{
		for (int j = 0; j < ncut - i - 1; j++)
		{
			if (edge_cuts[j]->head->level > edge_cuts[j + 1]->head->level || (edge_cuts[j]->head->level == edge_cuts[j + 1]->head->level && edge_cuts[j]->dc > edge_cuts[j + 1]->dc))
			{
				EdgeCut* temp;
				temp = edge_cuts[j];
				edge_cuts[j] = edge_cuts[j + 1];
				edge_cuts[j + 1] = temp;
			}
		}
	}

	//for (int i = 0; i < ncut; i++) 
	//{
	//	edge_cuts[i]->cut_id = i;
	//}
}

int cal_cost(TNode *root, int* map_n2c, EdgeCut **edge_cuts, int ncut, int cut_on_root)
{
	EdgeCut **edge_cuts_copy = new EdgeCut*[ncut];
	//int *cost = new int[root->treesize];
	//memset(cost, -1, sizeof(int) * (root->treesize));
	int max_order = -1, temp_order;
	int cost;
	//vector<vector<EdgeCut*> > all_children;
	for (int icut = 0; icut < ncut; icut++)
	{
		edge_cuts_copy[icut] = new EdgeCut();
	}

	for (int icut = 0; icut < ncut; icut++)
	{
		edge_cuts_copy[icut]->dc = edge_cuts[icut]->dc;
		edge_cuts_copy[icut]->head = edge_cuts[icut]->head;
		edge_cuts_copy[icut]->tail = edge_cuts[icut]->tail;
		edge_cuts_copy[icut]->parent = edge_cuts[icut]->parent;
	}

	sort_cuts(edge_cuts_copy, ncut);
	/*for (int icut = 0; icut < ncut - cut_on_root; icut++)
	{
	if (edge_cuts_copy[icut]->parent)
	{
	all_children[edge_cuts_copy[icut]->parent->cut_id].push_back(edge_cuts_copy[icut]);
	}
	}*/
	//temp_order = edge_cuts_copy[0]->dc + root->down_component;
	//max_order = max_order > temp_order ? max_order : temp_order;
	//int temp_order = set_node_order(root, map_n2c, 0, 0, 0);
	//temp_order = set_node_order(edge_cuts_copy[0]->head, map_n2c, temp_order, 0, 0);

	/*for (int icut = 1; icut < ncut - cut_on_root; icut++)
	{
	temp_order = edge_cuts_copy[icut]->
	}*/
	//cost = root->down_component;
	cost = -1;
	for (int icut = 0; icut < ncut; icut++)
	{
		if ((int)edge_cuts_copy[icut]->head->level + edge_cuts_copy[icut]->dc > cost)
			cost = edge_cuts_copy[icut]->head->level + edge_cuts_copy[icut]->dc;
		/*int level_cost = 1;
		TNode *pd = edge_cuts_copy[icut]->tail;
		while (pd && pd->current_index > -1)
		{
		int edge_cost = edge_cuts_copy[icut]->dc + level_cost;
		int temp_cost = pd->src_dc > edge_cost ? pd->src_dc : edge_cost;
		cost[pd->current_index] = cost[pd->current_index] > temp_cost ? cost[pd->current_index] : temp_cost;
		level_cost++;
		pd = pd->parent;
		}*/
	}
	for (int icut = 0; icut < ncut; icut++)
		delete edge_cuts_copy[icut];
	//temp_order = cost[0];
	delete[] edge_cuts_copy;
	edge_cuts_copy = NULL;
	//delete[] cost;
	//cost = NULL;
	return root->down_component + cost;
}

void set_branchindex(TNode* nd, int branchindex)
{
	if (!nd)
		return;
	nd->branchindex = branchindex;
	for (int ichild = 0, nchild = nd->children.size(); ichild < nchild; ichild++)
		set_branchindex(nd->children[ichild], branchindex);
}

static void tree_partitioning(int ncell, VecTNode& nodevec, int ncut, int nnode)
{
	int ncell_per_warp = 32 / ncut;
	int max_icell, max_order = -1;
	for (int icell = 0; icell < ncell; icell++)
	{
		//icell = 418;
		int eval = 99999, cost;
		TNode *root = NULL;
		TNode **tree_nodes = new TNode*[nodevec[icell]->treesize];
		init_tree(nodevec, icell, tree_nodes);
		int cut_on_root = ncut;
		int best_root_dc, cut_child_id = 0, best_cut_on_root = ncut;
		size_t child_size = 0;
		EdgeCut **edge_cuts = new EdgeCut*[ncut];
		EdgeCut **best_cuts = new EdgeCut*[ncut];
		for (int icut = 0; icut < ncut; icut++)
		{
			edge_cuts[icut] = new EdgeCut();
			best_cuts[icut] = new EdgeCut();
		}

		//root = nodevec[icell];
		if (nodevec[icell]->children.size() > 1)
		{
			root = new TNode(-1);
			root->current_index = -1;
			root->children.push_back(nodevec[icell]);
			root->treesize = nodevec[icell]->treesize + 1;
			root->src_dc = root->down_component = nodevec[icell]->down_component + 1;
			nodevec[icell]->parent = root;
		}
		else
			root = nodevec[icell];

		int *map_n2c = new int[root->treesize]; // map_n2c[n] = c if the head of cut[c] is node[n]
		int *best_map_n2c = new int[root->treesize];
		memset(map_n2c, -1, sizeof(int) * root->treesize);
		memset(best_map_n2c, -1, sizeof(int) * root->treesize);
		//int *map_n2c = new int[nodevec[icell]->treesize]; // map_n2c[n] = c if the head of cut[c] is node[n]
		//int *best_map_n2c = new int[nodevec[icell]->treesize];
		//memset(map_n2c, -1, sizeof(int) * nodevec[icell]->treesize);
		//memset(best_map_n2c, -1, sizeof(int) * nodevec[icell]->treesize);
		//edge_cuts[0]->tail = edge_cuts[0]->head = nodevec[icell];
		//edge_cuts[0]->parent = NULL;
		//edge_cuts[0]->dc = 0;
		//for (int ichild = 0; ichild < root->children.size(); ichild++)
		//{
		//	if (root->children[ichild]->treesize > child_size)
		//	{
		//		child_size = root->children[ichild]->treesize;
		//		cut_child_id = ichild;
		//	}
		//}
		for (int icut = 0; icut < ncut; icut++)
		{
			edge_cuts[icut]->tail = root;
			edge_cuts[icut]->head = root->children[0];
			edge_cuts[icut]->dc = edge_cuts[icut]->head->down_component;
			//edge_cuts[icut]->tail = nodevec[icell];
			//edge_cuts[icut]->head = nodevec[icell]->children[0];
			//edge_cuts[icut]->dc = edge_cuts[icut]->head->down_component;
			edge_cuts[icut]->parent = NULL;
			//best_cuts[icut] = edge_cuts[icut];
			//edge_cuts[icut]->children.reserve(2);
			//edge_cuts[0]->children.push_back(edge_cuts[icut]);
		}
		root->down_component -= edge_cuts[0]->dc;
		//nodevec[icell]->down_component -= edge_cuts[1]->dc;
		map_n2c[edge_cuts[0]->head->current_index] = 0;
		int max_dc = 99999;

		while (root->down_component < max_dc || cut_on_root > 0)
		{
			//down shift
			int cut_to_shift = -1, temp = -1;
			for (int icut = 0; icut < ncut; icut++)
			{
				if (edge_cuts[icut]->dc > temp)
				{
					bool valid = false;
					TNode *head = edge_cuts[icut]->head;
					for (int ichild = 0, nchild = head->children.size(); ichild < nchild; ichild++)
					{
						if (head->children[ichild]->down_component > -1 && map_n2c[head->children[ichild]->current_index] == -1)
						{
							valid = true;
							break;
						}
					}
					if (valid)
					{
						cut_to_shift = icut;
						temp = edge_cuts[icut]->dc;
					}

				}
			}
			if (cut_to_shift == -1)
				break;
			if (edge_cuts[cut_to_shift]->tail == root)
				cut_on_root--;
			//if (edge_cuts[cut_to_shift]->tail == nodevec[icell])
			//	cut_on_root--;

			down_shift(edge_cuts, cut_to_shift, map_n2c, cut_on_root, ncut);
			//max_dc = -1;
			//int root_dc = root->down_component;
			//int root_dc = nodevec[icell]->down_component;

			for (int icut = 0; icut < ncut; icut++)
				if (edge_cuts[icut]->dc > max_dc)
					max_dc = edge_cuts[icut]->dc;

			cost = cal_cost(root, map_n2c, edge_cuts, ncut, cut_on_root);
			if (cost < eval)
			{
				for (int icut = 0; icut < ncut - cut_on_root; icut++)
				{
					best_cuts[icut]->tail_id = edge_cuts[icut]->tail->current_index;
					best_cuts[icut]->head_id = edge_cuts[icut]->head->current_index;
					best_cuts[icut]->head = edge_cuts[icut]->head;
					best_cuts[icut]->tail = edge_cuts[icut]->tail;
					best_cuts[icut]->dc = edge_cuts[icut]->dc;
					best_cuts[icut]->parent = edge_cuts[icut]->parent;
					best_root_dc = nodevec[icell]->down_component;
					best_cut_on_root = cut_on_root;
					//best_map_n2c[best_cuts[icut]->head_id] = icut;
				}
				eval = cost;
			}
		}

		//sort best_cuts by level, best_cuts[0] has minimun level
		//printf("icell:%d\n", icell);
		for (int i = 0; i < ncut - best_cut_on_root; i++)
		{
			for (int j = 0; j < ncut - best_cut_on_root - i - 1; j++)
			{
				if (best_cuts[j]->head->level > best_cuts[j + 1]->head->level || (best_cuts[j]->head->level == best_cuts[j + 1]->head->level && best_cuts[j]->dc > best_cuts[j + 1]->dc))
				{
					EdgeCut* temp;
					temp = best_cuts[j];
					best_cuts[j] = best_cuts[j + 1];
					best_cuts[j + 1] = temp;
				}
			}
		}

		memset(best_map_n2c, -1, sizeof(int) * root->treesize);
		//memset(best_map_n2c, -1, sizeof(int) * nodevec[icell]->treesize);
		for (int icut = 0; icut < ncut - best_cut_on_root; icut++)
		{
			//printf("cut %d dc:%d head:%d tail:%d level:%d\n", icut, best_cuts[icut]->dc, best_cuts[icut]->head->nodeindex, best_cuts[icut]->tail->nodeindex, best_cuts[icut]->head->level);
			best_map_n2c[best_cuts[icut]->head_id] = icut;
		}
		//printf("cost:%d\n", eval);

		//for (int i = 0; i < ncut; i++)
		//printf("cell:%d cut %d: dc:%d tail:%d head:%d level:%d\n", icell, i, best_cuts[i]->dc, best_cuts[i]->tail_id, best_cuts[i]->head_id, best_cuts[i]->head->level);

		int temp_order;
		if (best_cut_on_root <= 0)
		{
			temp_order = set_node_order(nodevec[icell], best_map_n2c, 0, 0, icell / ncell_per_warp);
			temp_order = set_node_order(best_cuts[0]->head, best_map_n2c, temp_order, 0, icell / ncell_per_warp);
			for (int icut = 1; icut < ncut - best_cut_on_root; icut++)
			{
				int temp = set_node_order(best_cuts[icut]->head, best_map_n2c, best_cuts[icut]->tail->nodeorder + 1, icut, icell / ncell_per_warp);
				temp_order = temp_order > temp ? temp_order : temp;
			}
		}
		else
		{
			temp_order = set_node_order(nodevec[icell], best_map_n2c, 0, 0, icell / ncell_per_warp);
			for (int icut = 0; icut < ncut - best_cut_on_root; icut++)
			{
				int temp = set_node_order(best_cuts[icut]->head, best_map_n2c, best_cuts[icut]->tail->nodeorder + 1, icut + 1, icell / ncell_per_warp);
				temp_order = temp_order > temp ? temp_order : temp;
			}
		}

		//printf("cell:%d size:%d norder:%d\n", icell, nodevec[icell]->treesize, temp_order);

		for (int icut = 0; icut < ncut; icut++)
		{
			delete edge_cuts[icut];
			delete best_cuts[icut];
		}
		if (root != nodevec[icell])
		{
			delete root;
			nodevec[icell]->parent = NULL;
		}
		if (max_order < temp_order)
		{
			max_order = temp_order;
			max_icell = icell;
		}

		delete[] tree_nodes;
		delete[] edge_cuts;
		delete[] best_cuts;
		delete[] map_n2c;
		delete[] best_map_n2c;
	}

	printf("max_order:%d max_icell:%d\n", max_order, max_icell);
    std::sort(nodevec.begin() + ncell, nodevec.end(), sort_by_branch_order);
}

/*
 * prev_node: previous node, in nodeindex
 * next_node: next node to compute, in nodeindex
 */
static void dynamic_interleave(int ncell, VecTNode &nodevec, int nnode, int nthread_per_cell, int*& prev_node, int*& next_node)
{
	//prev_node = new int[nnode];
	//next_node = new int[nnode];
    int *branch_size = new int[32];
	memset(prev_node, -1, sizeof(int) * nnode);
	memset(next_node, -1, sizeof(int) * nnode);
	int ncell_per_warp = 32 / nthread_per_cell;
    int nwarp = (ncell + ncell_per_warp - 1) / ncell_per_warp;
    for (int iwarp = 0; iwarp < nwarp; iwarp++)
    {
        memset(branch_size, 0, sizeof(int) * 32);
        for (int icell_in_warp = 0; icell_in_warp < ncell_per_warp; icell_in_warp++)
        {
            int icell = iwarp * ncell_per_warp + icell_in_warp;
            if (icell >= ncell)
                break;
            int cell_size = (int)nodevec[icell]->treesize;
            int max_order = -1, order = 0, max_level = -1;
            TNode **tree_nodes = new TNode*[cell_size];
            TNode **node_to_compute = new TNode*[nthread_per_cell];
            TNode **prev_compute_node = new TNode*[nthread_per_cell];
            init_tree(nodevec, icell, tree_nodes);

            //for (int inode = 0; inode < cell_size; inode++)
            //	printf("node:%d children:%d\n", inode, tree_nodes[inode]->children.size());
            //if (nodevec[icell]->hash != nodevec[0]->hash)
            //	printf("error\n");

            //max_order = -1;
            int *children_left = new int[cell_size];
            priority_queue< TNode*, vector<TNode*>, CompareNode > pq;
            for (int inode = 0; inode < cell_size; inode++)
            {
                children_left[inode] = (int)tree_nodes[inode]->children.size();
                if ((int)tree_nodes[inode]->level > max_level)
                    max_level = (int)tree_nodes[inode]->level;
                if (children_left[inode] == 0)
                {
                    pq.push(tree_nodes[inode]);
                }
            }

            TNode *nd = NULL;
            while (!pq.empty())
            {
                int nnode_to_compute = 0;
                for (int ithread = 0; ithread < nthread_per_cell; ithread++)
                {
                    if (pq.empty())
                        break;
                    nd = pq.top();
                    pq.pop();
                    node_to_compute[nnode_to_compute++] = nd;
                }
                if (nnode_to_compute == 0)
                    break;
                //printf("order:%d\n", order);
                for (int ithread = 0; ithread < nnode_to_compute; ithread++)
                {
                    nd = node_to_compute[ithread];
                    nd->branchindex = icell_in_warp * nthread_per_cell + ithread;
                    nd->nodeorder = order;
                    nd->groupindex = icell / ncell_per_warp;

                    branch_size[(int)nd->branchindex]++;

                    //printf("nodeindex:%d current_index:%d ", nd->nodeindex, nd->current_index);
                    if (order > 0)
                    {
                        next_node[nd->nodeindex] = prev_compute_node[ithread]->nodeindex;
                        prev_node[prev_compute_node[ithread]->nodeindex] = nd->nodeindex;
                        //printf("prev_nodeindex:%d prev_currentindex:%d\n", prev_compute_node[ithread]->nodeindex, prev_compute_node[ithread]->current_index);
                    }
                    prev_compute_node[ithread] = nd;

                    if (nd->parent)
                    {
                        children_left[nd->parent->current_index]--;
                        if (children_left[nd->parent->current_index] == 0)
                            pq.push(nd->parent);
                    }
                }

                //printf("\n");
                if (order > max_order)
                    max_order = order;
                order++;
                //max_order = nd->nodeorder;
            }

            //printf("cell:%d order:%d max_level:%d\n", icell, max_order, max_level);
            for (int inode = 0; inode < cell_size; inode++)
            {
                tree_nodes[inode]->nodeorder = max_order - tree_nodes[inode]->nodeorder;
            }
            delete []node_to_compute;
            delete []prev_compute_node;
            delete []tree_nodes;
            delete []children_left;
             
        }
        
        int sorted_branchindex[32];
        // sort by branch size from large to small
        argsort(branch_size, 32, sorted_branchindex);
        //for (int i = 0; i < 32; i++)
        //{
        //    printf("i:%d size:%d\n", i, branch_size[i]);
        //}
        //for (int i = 0; i < 32; i++)
        //{
        //    printf("sorted i:%d size:%d\n", sorted_branchindex[i], branch_size[sorted_branchindex[i]]);
        //}
        for (int icell_in_warp = 0; icell_in_warp < ncell_per_warp; icell_in_warp++)
        {
            int icell = iwarp * ncell_per_warp + icell_in_warp;
            if (icell >= ncell)
                break;
            modify_branchindex(nodevec[icell], sorted_branchindex, branch_size);
        }
    }


    int *max_order_per_cell = NULL, *max_order_per_group = NULL;
    max_order_per_cell = new int[ncell];
    /*if (ncell * nthread_per_cell % 32 == 0)
        nwarp = ncell * nthread_per_cell / 32;
    else
        nwarp = ncell * nthread_per_cell / 32 + 1;*/

    max_order_per_group = new int[nwarp];
    max_order_per_cell = new int[ncell];
    memset(max_order_per_group, -1, sizeof(int) * nwarp);
    memset(max_order_per_cell, -1, sizeof(int) * ncell);

    for (int i = 0; i < nnode; i++)
    {
        TNode *nd = nodevec[i];
        if (nd->nodeorder > max_order_per_cell[nd->cellindex])
            max_order_per_cell[nd->cellindex] = nd->nodeorder;
        if (nd->nodeorder > max_order_per_group[nd->groupindex])
            max_order_per_group[nd->groupindex] = nd->nodeorder;
    }

    for (int icell = 0; icell < ncell; icell++)
    {
        //printf("max order:%d\n", max_order_per_cell[icell]);
        TNode *nd = nodevec[icell];
        if (max_order_per_cell[nd->cellindex] < max_order_per_group[nd->groupindex])
             modify_orders(nd, max_order_per_group[nd->groupindex] - max_order_per_cell[nd->cellindex]);
    }

    delete [] max_order_per_group;
    delete [] max_order_per_cell;
    delete [] branch_size;
    max_order_per_group = NULL;
    max_order_per_cell = NULL;
    branch_size = NULL;

	std::sort(nodevec.begin() + ncell, nodevec.end(), sort_by_branch_order);
}


static void admin_dynamic2(int branch_per_cell,
                        int ncell,
                        int nbranch,
                        VecTNode& nodevec,
                        int& norder,
                        int*& max_order_per_thread,
                        int*& min_order_per_thread,
                        int*& firstnode, 
                        int*& lastnode,
                        int*& stride,
                        int*& map_t2c,
                        int* prev_node_nodeindex,
                        int* next_node_nodeindex,
                        int*& prev_node,
                        int*& next_node,
                        int* nodeorder)
{
    std::cout << "****admin_dynamic2****" << std::endl;
    firstnode = new int[nbranch];
    lastnode = new int[nbranch];
    max_order_per_thread = new int[nbranch];
    min_order_per_thread = new int[nbranch];
    
    int nnode = nodevec.size();
    prev_node = new int[nnode];
    next_node = new int[nnode];
    norder = 0;
    for (int i = 0; i < nnode; i++)
    {
        //if (i < 50)
        //    printf("i:%d order:%d\n", i, nodevec[i]->nodeorder);
        if (norder < nodevec[i]->nodeorder)
            norder = nodevec[i]->nodeorder;
    }

    int nwarps = (nbranch + 31) / 32;
    stride = new int[nwarps * (norder + 1)];
    printf("nbranch:%d nwarps:%d norder:%d\n", nbranch, nwarps, norder);
    
    memset(stride, 0, sizeof(int) * nwarps * (norder + 1));
    memset(firstnode, -1, sizeof(int) * nbranch);
    memset(lastnode, -1, sizeof(int) * nbranch);
    memset(prev_node, -1, sizeof(int) * nnode);
    memset(next_node, -1, sizeof(int) * nnode);
    for (int i = 0; i < nbranch; i++)
    {
        max_order_per_thread[i] = -1;
        min_order_per_thread[i] = 99999;
    }
    //int *all_sizes;
    //all_sizes = new int[nbranch];
    //memset(all_sizes, 0, sizeof(int) * nbranch);

    map_t2c = new int[nbranch];
    int *map_c2t = new int[ncell];
    memset(map_t2c, -1, sizeof(int) * nbranch);
    //memset(map_c2t, -1, sizeof(int) * ncell);
    for (int i = 0; i < ncell; i++)
        map_c2t[i] = nbranch + 100; //max value
    for (int i = ncell; i < nnode; i++)
    {
        TNode *nd = nodevec[i];
        int tid = nd->groupindex * 32 + nd->branchindex;
        if (tid < map_c2t[nd->cellindex])
            map_c2t[nd->cellindex] = tid;
        if (tid >= nbranch)
            printf("**error***\n\t i:%d icell:%d tid:%d iwarp:%d branch:%d\n", i, nd->cellindex, tid, nd->groupindex, nd->branchindex);
        //all_sizes[tid]++;
        if (firstnode[tid] == -1)
            firstnode[tid] = i;
        lastnode[tid] = i;

        if (max_order_per_thread[tid] < nd->nodeorder)
            max_order_per_thread[tid] = nd->nodeorder;
        if (min_order_per_thread[tid] > nd->nodeorder)
            min_order_per_thread[tid] = nd->nodeorder;

        if (prev_node_nodeindex[nd->nodeindex] > -1)
            prev_node[nodeorder[nd->nodeindex]] = nodeorder[prev_node_nodeindex[nd->nodeindex]];
        if (next_node_nodeindex[nd->nodeindex] > -1)
            next_node[nodeorder[nd->nodeindex]] = nodeorder[next_node_nodeindex[nd->nodeindex]];
        stride[tid / 32 * (norder + 1) + nd->nodeorder] += 1;
    }

    for (int i = 0; i < ncell; i++)
    {
        map_t2c[map_c2t[i]] = i;
    }
    
    delete[] map_c2t;
}

static void admin_tp(int branch_per_cell,
                   int ncell,
                   VecTNode& nodevec,
                   int& norder,
                   int*& prev_node,
                   int*& next_node,
                   int*& max_order_per_thread,
                   int*& min_order_per_thread,
                   int*& firstnode,
                   int*& lastnode) {
    // firstnode[i] is the index of the first nonroot node of the branch
    // lastnode[i] is the index of the last node of the branch
    // cellsize is the number of nodes in the branch not counting root.
    // nstride is the maximum branch size (not counting root)
    // stride[i] is the number of branches with an ith node.
    std::cout<<"******admin_branch*****"<<std::endl;

    int nbranch = ncell * branch_per_cell;
    int ncell_per_warp = 32 / branch_per_cell;
    int nwarps = ncell / ncell_per_warp;
    int ncell_in_last_warp = ncell - ncell_per_warp * nwarps;
    firstnode = new int[nbranch];
    lastnode = new int[nbranch];
    max_order_per_thread = new int[nbranch];
    min_order_per_thread = new int[nbranch];
    //branchsize = new int[nbranch];
    int nnode = nodevec.size();
    prev_node = new int[nnode];
    next_node = new int[nnode];

    memset(firstnode, -1, sizeof(int) * nbranch);
    memset(lastnode, -1, sizeof(int) * nbranch);
    memset(prev_node, -1, sizeof(int) * nnode);
    memset(next_node, -1, sizeof(int) * nnode);
    //memset(branchsize, -1, sizeof(int) * nbranch);

    vector<vector<int>> nodeid_by_branch;
    nodeid_by_branch.resize(nbranch + 1);
    //printf("nbranch:%d\n", nbranch);

    for (int i = ncell; i < nnode; i++)
    {
        TNode* nd = nodevec[i];
        int tid;
        if ((int)nd->cellindex >= nwarps * ncell_per_warp)
            tid = 32 * (int)nd->groupindex + (int)nd->branchindex * ncell_in_last_warp + (int)(nd->cellindex % ncell_per_warp);
        else
            tid = 32 * (int)nd->groupindex + (int)nd->branchindex * ncell_per_warp + (int)(nd->cellindex % ncell_per_warp);
        //printf("i:%d tid:%d\n", i, tid);
        nodeid_by_branch[tid].push_back(i);
        if (nodeid_by_branch[tid].size() > 1)
        {
            prev_node[i] = nodeid_by_branch[tid][nodeid_by_branch[tid].size() - 2];
            next_node[nodeid_by_branch[tid][nodeid_by_branch[tid].size() - 2]] = i;
        }
    }


    for (int i = 0; i < nbranch; i++)
    {
        max_order_per_thread[i] = -1;
        min_order_per_thread[i] = 99999;
    }

    norder = -1;
    //for (int i = ncell; i < 128; i++)
    //    printf("i:%d group:%d cell:%d branch:%d order:%d\n", i, nodevec[i]->groupindex, nodevec[i]->cellindex, nodevec[i]->branchindex, nodevec[i]->nodeorder);
    for (int i = ncell; i < nnode; i++)
    {
        TNode* nd = nodevec[i];
        //int ibranch = (int)nd->branchindex;
        //int global_branchindex = nd->cellindex * branch_per_cell + ibranch;
        //int global_branchindex = nd->branchindex * ncell + (int)nd->cellindex;
        int thread_index;
        if ((int)nd->cellindex >= nwarps * ncell_per_warp)
            thread_index = 32 * (int)nd->groupindex + (int)nd->branchindex * ncell_in_last_warp + (int)(nd->cellindex % ncell_per_warp);
        else
            thread_index = 32 * (int)nd->groupindex + (int)nd->branchindex * ncell_per_warp + (int)(nd->cellindex % ncell_per_warp);
        //if (thread_index == 8)
        //    printf("group:%d cell%d branch:%d\n", nd->groupindex, nd->cellindex, nd->branchindex);
        if (firstnode[thread_index] == -1)
            firstnode[thread_index] = i;
        lastnode[thread_index] = i;

        if (nd->nodeorder > norder)
            norder = nd->nodeorder;
        if (max_order_per_thread[thread_index] < nd->nodeorder)
            max_order_per_thread[thread_index] = nd->nodeorder;
        if (min_order_per_thread[thread_index] > nd->nodeorder)
            min_order_per_thread[thread_index] = nd->nodeorder;

        /*for (int j = i - 1; j >= 0; j--)
        {
            //int temp = nodevec[j]->cellindex * branch_per_cell + nodevec[j]->branchindex;
            TNode *nn = nodevec[j];
            int temp;
            if ((int)nn->cellindex >= nwarps * ncell_per_warp)
                temp = 32 * (int)nn->groupindex + (int)nn->branchindex * ncell_in_last_warp + (int)(nn->cellindex % ncell_per_warp);
            else
                temp = 32 * (int)nn->groupindex + (int)nn->branchindex * ncell_per_warp + (int)(nn->cellindex % ncell_per_warp);
            if (temp == thread_index)
            {
                prev_node[i] = j;
                break;
            }
        }

        for (int j = i + 1; j < nnode; j++)
        {
            //int temp = nodevec[j]->cellindex * branch_per_cell + nodevec[j]->branchindex;
            TNode* nn = nodevec[j];
            int temp;
            if ((int)nn->cellindex >= nwarps * ncell_per_warp)
                temp = 32 * (int)nn->groupindex + (int)nn->branchindex * ncell_in_last_warp + (int)(nn->cellindex % ncell_per_warp);
            else
                temp = 32 * (int)nn->groupindex + (int)nn->branchindex * ncell_per_warp + (int)(nn->cellindex % ncell_per_warp);
            if (temp == thread_index)
            {
                next_node[i] = j;
                break;
            }
        }*/
    }

    printf("norder:%d\n", norder);
    
}

static void admin_dynamic(int branch_per_cell,
                   int ncell,
                   VecTNode& nodevec,
                   int& norder,
                   int*& prev_node_nodeindex,
                   int*& next_node_nodeindex,
                   int*& prev_node,
                   int*& next_node,
                   int*& max_order_per_thread,
                   int*& min_order_per_thread,
                   int*& firstnode,
                   int*& lastnode,
                   int*& stride,
                   int* nodeorder) {

    std::cout<<"*****admin_dynamic****"<<std::endl;
    int nbranch = ncell * branch_per_cell;
    int ncell_per_warp = 32 / branch_per_cell;
    int nwarps = ncell / ncell_per_warp;
    int ncell_in_last_warp = ncell - ncell_per_warp * nwarps;
    firstnode = new int[nbranch];
    lastnode = new int[nbranch];
    max_order_per_thread = new int[nbranch];
    min_order_per_thread = new int[nbranch];
    //branchsize = new int[nbranch];

    int nnode = nodevec.size();
    prev_node = new int[nnode];
    next_node = new int[nnode];
    int max_order = 0;
    for (int i = 0; i < nnode; i++)
        if (max_order < nodevec[i]->nodeorder)
            max_order = nodevec[i]->nodeorder;

    norder = max_order;
    if (ncell_in_last_warp > 0)
    {
        stride = new int [(nwarps + 1) * (max_order + 1)];
        memset(stride, 0, sizeof(int) * (nwarps + 1) * (max_order + 1));
    }
    else
    {
        stride = new int [nwarps * (max_order + 1)];
        memset(stride, 0, sizeof(int) * nwarps * (max_order + 1));
    }

    memset(firstnode, -1, sizeof(int) * nbranch);
    memset(lastnode, -1, sizeof(int) * nbranch);
    memset(prev_node, -1, sizeof(int) * nnode);
    memset(next_node, -1, sizeof(int) * nnode);
    //memset(branchsize, -1, sizeof(int) * nbranch);

    for (int i = 0; i < nbranch; i++)
    {
        max_order_per_thread[i] = -1;
        min_order_per_thread[i] = 99999;
    }
    //norder = -1;

    int *all_sizes;
    all_sizes = new int[nbranch];
    memset(all_sizes, 0, sizeof(int) * nbranch);
    for (int i = ncell; i < nnode; i++)
    {
        TNode *nd = nodevec[i];
        //int ibranch = nd->branchindex;
        int thread_index = nd->groupindex * 32 + nd->branchindex;
        all_sizes[thread_index]++;
        /*if ((int)nd->cellindex >= nwarps * ncell_per_warp)
        {
            thread_index = 32 * (int)nd->groupindex + (int)nd->branchindex * ncell_in_last_warp + (int)(nd->cellindex % ncell_per_warp);
        }
        else 
        {
            thread_index = 32 * (int)nd->groupindex + (int)nd->branchindex * ncell_per_warp + (int)(nd->cellindex % ncell_per_warp);
        }*/
        if (firstnode[thread_index] == -1)
            firstnode[thread_index] = i;
        lastnode[thread_index] = i;

        if (max_order_per_thread[thread_index] < nd->nodeorder)
            max_order_per_thread[thread_index] = nd->nodeorder;
        if (min_order_per_thread[thread_index] > nd->nodeorder)
            min_order_per_thread[thread_index] = nd->nodeorder;

        prev_node[nodeorder[nd->nodeindex]] = nodeorder[prev_node_nodeindex[nd->nodeindex]];
        next_node[nodeorder[nd->nodeindex]] = nodeorder[next_node_nodeindex[nd->nodeindex]];

        stride[thread_index / 32 * (max_order + 1) + nd->nodeorder] += 1;
        
    }

    
    //for (int iorder = 0; iorder <= norder; iorder++)
    //    printf("iorder:%d stride:%d\n", iorder, stride[iorder]);
    //for (int tid = 0; tid < 32; tid++)
    //    printf("tid:%d max:%d min:%d size:%d\n", tid, max_order_per_thread[tid], min_order_per_thread[tid], all_sizes[tid]);
    delete[] all_sizes;
    all_sizes = NULL;
    int tid = 0;
    /*for (tid = 0; tid < nbranch; tid++)
    {
        int id1 = lastnode[tid], id2 = lastnode[tid];
        for (int iorder = norder; iorder >= 0; iorder--)
        {
            if (iorder >= min_order_per_thread[tid] && iorder <= max_order_per_thread[tid])
            {
                if (id1 >= nnode || id1 < 0)
                    printf("error tid:%d id:%d iorder:%d\n", tid, id1, iorder);
                //if (id1 != id2)
                //{
                //    printf("tid:%d iorder:%d id1:%d id2:%d nodeorder1:%d nodeorder2:%d\n", tid, iorder, id1, id2, nodevec[id1]->nodeorder, nodevec[id2]->nodeorder);
                //}
                id1 = prev_node[id1];
                id2 -= stride[tid / 32 * (norder + 1) + iorder - 1];
            }
        }
    }*/


    /*for (tid = 0; tid < nbranch; tid++)
    {
        int id1 = firstnode[tid], id2 = firstnode[tid];
        for (int iorder = 0; iorder <= norder; iorder++)
        {
            if (iorder >= min_order_per_thread[tid] && iorder <= max_order_per_thread[tid])
            {
                if (id1 != id2)
                    printf("phase2 tid:%d iorder:%d id1:%d id2:%d\n", tid, iorder, id1, id2);
                id1 = next_node[id1];
                id2 += stride[tid / 32 * (norder + 1) + iorder];
            }
        }
    }*/
    
    //for (int i = 0; i < 32; i++)
    //    printf("thread:%d max_order:%d min_order:%d\n", i, max_order_per_thread[i], min_order_per_thread[i]);
}

static void admin_warp(int ncell,
                       VecTNode& nodevec,
                       int*& firstnode,
                       int*& lastnode,
                       int*& max_level_per_thread,
                       int*& min_level_per_thread,
                       int*& prev_node,
                       int*& next_node,
                       int*& children_num,
                       int& max_level,
                       int& nthread_each_cell,
                       int& threads_num)
{
    nodevec[0]->start_thread = 0;
    max_level = -1;
    int need_warp = (nodevec[0]->leaf_num - 1)  / 32 + 1;
    threads_num += need_warp * 32;
    nthread_each_cell = need_warp * 32;
    for (int i = 1; i < ncell; i++)
    {
        nodevec[i]->start_thread = nodevec[i-1]->start_thread + need_warp * 32;
        need_warp = (nodevec[i]->leaf_num - 1)  / 32 + 1;
        if (need_warp * 32 > nthread_each_cell)
            nthread_each_cell = need_warp * 32;
        threads_num += need_warp * 32;
        printf("leaf_num:%d nthread_per_cell:%d\n", nodevec[i]->leaf_num, nthread_each_cell);
    }

    threads_num = ncell * nthread_each_cell;
    for (int i = 0; i < ncell; i++)
    {
        nodevec[i]->start_thread = i * nthread_each_cell; 
    }
    
    int nnode = nodevec.size();

    firstnode = new int[threads_num];
    lastnode = new int[threads_num];
    max_level_per_thread = new int[threads_num];
    min_level_per_thread = new int[threads_num];
    next_node = new int[nnode];
    prev_node = new int[nnode];
    children_num = new int[nnode];

    for (int i = 0; i < threads_num; i++)
    {
        firstnode[i] = -1;
        lastnode[i] = -1;
        max_level_per_thread[i] = -1;
        min_level_per_thread[i] = 99999;
    }
    for (int i = 0; i < nnode; i++)
    {
        next_node[i] = -1;
        prev_node[i] = -1;
        children_num[i] = nodevec[i]->children.size();
    }
    
    for (int i = ncell; i < nnode; i++)
    {
        TNode* root_node = nodevec[nodevec[i]->cellindex];
        int thread_idx = root_node->start_thread + nodevec[i]->branchindex;
        if (firstnode[thread_idx] == -1)
            firstnode[thread_idx] = i;
        lastnode[thread_idx] = i;
        //printf("inode:%d thread_idx:%d child_size:%d\n", i, thread_idx, nodevec[i]->children.size());
        TNode* cd = NULL;
        if (nodevec[i]->children.size() > 0)
            cd = nodevec[i]->children[0];
        if (cd != NULL && next_node[i] == -1)
        {
            next_node[i] = cd->current_index;
        }

        TNode* pd = nodevec[i]->parent;
        if (pd != NULL && prev_node[i] == -1)
        {
            prev_node[i] = pd->current_index;
            //if (thread_idx == 0)
            //{
            //    printf("i:%d pindex:%d ip:%d prev:%d level:%d\n", i, nodevec[i]->current_index, pd->current_index, prev_node[i], nodevec[i]->level);
            //}
        }

        if (max_level_per_thread[thread_idx] < (int)nodevec[i]->level)
            max_level_per_thread[thread_idx] = (int)nodevec[i]->level;

        if (min_level_per_thread[thread_idx] > (int)nodevec[i]->level)
            min_level_per_thread[thread_idx] = (int)nodevec[i]->level;

        if ((int)nodevec[i]->level > max_level)
            max_level = (int)nodevec[i]->level;
        
    }
    
    //int i = lastnode[30];
    //int same = 0, diff = 0;
    //for (i = 0; i < nnode; i++)
    //{
    //    TNode* nd = nodevec[i];
    //    if (nd->parent)
    //    {
    //        if (prev_node[i] == nd->parent->current_index)
    //            same++;
    //        else
    //            diff++;
    //    }
    //}
    //printf("nnode:%d same:%d diff:%d\n", nnode, same, diff);
    //for (int level = max_level; level > 0; level--)
    //{
    //    if (level >= min_level_per_thread[30] && level <= max_level_per_thread[30])
    //    {
    //        printf("level:%d i:%d prev_node:%d\n", level, i, prev_node[i]);
    //        i = prev_node[i];
    //    }
    //}
    printf("%d\n", max_level);
}

static void admin1(int ncell,
                   VecTNode& nodevec,
                   int& nwarp,
                   int& nstride,
                   int*& stride,
                   int*& firstnode,
                   int*& lastnode,
                   int*& cellsize) {
    // firstnode[i] is the index of the first nonroot node of the cell
    // lastnode[i] is the index of the last node of the cell
    // cellsize is the number of nodes in the cell not counting root.
    // nstride is the maximum cell size (not counting root)
    // stride[i] is the number of cells with an ith node.
    firstnode = new int[ncell];
    lastnode = new int[ncell];
    cellsize = new int[ncell];

    nwarp = (ncell % warpsize == 0) ? (ncell / warpsize) : (ncell / warpsize + 1);

    for (int i = 0; i < ncell; ++i) {
        firstnode[i] = -1;
        lastnode[i] = -1;
        cellsize[i] = 0;
    }

    nstride = 0;
    for (size_t i = ncell; i < nodevec.size(); ++i) {
        TNode& nd = *nodevec[i];
        size_t ci = nd.cellindex;
        if (firstnode[ci] == -1) {
            firstnode[ci] = i;
        }
        lastnode[ci] = i;
        cellsize[ci] += 1;
        if (nstride < cellsize[ci]) {
            nstride = cellsize[ci];
        }
    }

    stride = new int[nstride + 1];  // in case back substitution accesses this
    for (int i = 0; i <= nstride; ++i) {
        stride[i] = 0;
    }
    for (size_t i = ncell; i < nodevec.size(); ++i) {
        TNode& nd = *nodevec[i];
        stride[nd.treenode_order - 1] += 1;  // -1 because treenode order includes root
    }
}

// for admin2 we allow the node organisation in warps of (say 4 cores per warp)
// ...............  ideal warp but unbalanced relative to warp with max cycles
// ...............  ncycle = 15, icore [0:4), all strides are 4.
// ...............
// ...............
//
// ..........       unbalanced relative to warp with max cycles
// ..........       ncycle = 10, not all strides the same because
// ..........       of need to avoid occasional race conditions.
//  .  . ..         icore [4:8) only 4 strides of 4
//
// ....................  ncycle = 20, uses only one core in the warp (cable)
//                       icore 8, all ncycle strides are 1

// One thing to be unhappy about is the large stride vector of size about
// number of compartments/warpsize. There are a lot of models where the
// stride for a warp is constant except for one cycle in the warp and that
// is easy to obtain when there are more than warpsize cells per warp.

static size_t stride_length(size_t begin, size_t end, VecTNode& nodevec) {
    // return stride length starting at i. Do not go past j.
    // max stride is warpsize.
    // At this time, only assume vicious parent race conditions matter.
    if (end - begin > warpsize) {
        end = begin + warpsize;
    }
    for (size_t i = begin; i < end; ++i) {
        TNode* nd = nodevec[i];
        nrn_assert(nd->nodevec_index == i);
        size_t diff = dist2child(nd);
        if (i + diff < end) {
            end = i + diff;
        }
    }
    return end - begin;
}

static void admin2(int ncell,
                   VecTNode& nodevec,
                   int& nwarp,
                   int& nstride,
                   int*& stridedispl,
                   int*& strides,
                   int*& rootbegin,
                   int*& nodebegin,
                   int*& ncycles) {
    // the number of groups is the number of warps needed
    // ncore is the number of warps * warpsize
    nwarp = nodevec[ncell - 1]->groupindex + 1;

    ncycles = new int[nwarp];
    stridedispl = new int[nwarp + 1];  // running sum of ncycles (start at 0)
    rootbegin = new int[nwarp + 1];    // index (+1) of first root in warp.
    nodebegin = new int[nwarp + 1];    // index (+1) of first node in warp.

    // rootbegin and nodebegin are the root index values + 1 of the last of
    // the sequence of constant groupindex
    rootbegin[0] = 0;
    for (size_t i = 0; i < size_t(ncell); ++i) {
        rootbegin[nodevec[i]->groupindex + 1] = i + 1;
    }
    nodebegin[0] = ncell;
    for (size_t i = size_t(ncell); i < nodevec.size(); ++i) {
        nodebegin[nodevec[i]->groupindex + 1] = i + 1;
    }

    // ncycles, stridedispl, and nstride
    nstride = 0;
    stridedispl[0] = 0;
    for (size_t iwarp = 0; iwarp < (size_t)nwarp; ++iwarp) {
        size_t j = size_t(nodebegin[iwarp + 1]);
        int nc = 0;
        size_t i = nodebegin[iwarp];
        while (i < j) {
            i += stride_length(i, j, nodevec);
            ++nc;
        }
        ncycles[iwarp] = nc;
        stridedispl[iwarp + 1] = stridedispl[iwarp] + nc;
        nstride += nc;
    }

    // strides
    strides = new int[nstride];
    nstride = 0;
    for (size_t iwarp = 0; iwarp < (size_t)nwarp; ++iwarp) {
        size_t j = size_t(nodebegin[iwarp + 1]);
        size_t i = nodebegin[iwarp];
        while (i < j) {
            int k = stride_length(i, j, nodevec);
            i += k;
            strides[nstride++] = k;
        }
    }

#if 0
printf("warp rootbegin nodebegin stridedispl\n");
for (int i = 0; i <= nwarp; ++i){
  printf("%4d %4d %4d %4d\n", i, rootbegin[i], nodebegin[i], stridedispl[i]);
}
#endif
}
}  // namespace coreneuron

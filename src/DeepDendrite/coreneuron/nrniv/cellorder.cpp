#include "coreneuron/nrnconf.h"
#include "coreneuron/nrnoc/multicore.h"
#include "coreneuron/nrnoc/nrnoc_decl.h"
#include "coreneuron/nrniv/nrn_assert.h"
#include "coreneuron/nrniv/cellorder.h"
#include "coreneuron/nrniv/tnode.h"
#include "coreneuron/nrniv/lpt.h"
#include "coreneuron/nrnoc/minisolver.h"

#include "coreneuron/nrniv/node_permute.h"  // for print_quality
#include <set>

#ifdef _OPENACC
#include <openacc.h>
#endif
namespace coreneuron {
int use_interleave_permute;
int thread_per_cell;
InterleaveInfo* interleave_info;  // nrn_nthread array

InterleaveInfo::InterleaveInfo() {
    nwarp = 0;
    nstride = 0;
    stridedispl = NULL;
    stride = NULL;
    firstnode = NULL;
    lastnode = NULL;
    cellsize = NULL;

    // for print statistics
    nnode = NULL;
    ncycle = NULL;
    idle = NULL;
    cache_access = NULL;
    child_race = NULL;

    prev_node = NULL;
    next_node = NULL;
    max_order_per_thread = NULL;
    min_order_per_thread = NULL;
    map_t2c = NULL;
    norder = 0;
    threads_num = 0;
    thread_per_cell = 0;
}

void InterleaveInfo::swap(InterleaveInfo& info) {
    std::swap(nwarp, info.nwarp);
    std::swap(nstride, info.nstride);

    std::swap(stridedispl, info.stridedispl);
    std::swap(stride, info.stride);
    std::swap(firstnode, info.firstnode);
    std::swap(lastnode, info.lastnode);
    std::swap(cellsize, info.cellsize);

    std::swap(nnode, info.nnode);
    std::swap(ncycle, info.ncycle);
    std::swap(idle, info.idle);
    std::swap(cache_access, info.cache_access);
    std::swap(child_race, info.child_race);
}

InterleaveInfo::InterleaveInfo(const InterleaveInfo& info) {
    nwarp = info.nwarp;
    nstride = info.nstride;

    copy_array(stridedispl, info.stridedispl, nwarp + 1);
    copy_array(stride, info.stride, nstride);
    copy_array(firstnode, info.firstnode, nwarp + 1);
    copy_array(lastnode, info.lastnode, nwarp + 1);
    copy_array(cellsize, info.cellsize, nwarp);

    copy_array(nnode, info.nnode, nwarp);
    copy_array(ncycle, info.ncycle, nwarp);
    copy_array(idle, info.idle, nwarp);
    copy_array(cache_access, info.cache_access, nwarp);
    copy_array(child_race, info.child_race, nwarp);
}

InterleaveInfo& InterleaveInfo::operator=(const InterleaveInfo& info) {
    // self assignment
    if (this == &info)
        return *this;

    InterleaveInfo temp(info);

    this->swap(temp);
    return *this;
}

InterleaveInfo::~InterleaveInfo() {
    if (stride) {
        delete[] stride;
        delete[] firstnode;
        delete[] lastnode;
        delete[] cellsize;
        if (prev_node)
            delete[] prev_node;
        if (next_node)
            delete[] next_node;
        if (max_order_per_thread)
            delete[] max_order_per_thread;
        if (min_order_per_thread)
            delete[] min_order_per_thread;
        if (map_t2c)
            delete[] map_t2c;
    }
    if (stridedispl) {
        delete[] stridedispl;
    }
    if (idle) {
        delete[] nnode;
        delete[] ncycle;
        delete[] idle;
        delete[] cache_access;
        delete[] child_race;
    }
}

void create_interleave_info() {
    destroy_interleave_info();
    interleave_info = new InterleaveInfo[nrn_nthread];
}

void destroy_interleave_info() {
    if (interleave_info) {
        delete[] interleave_info;
        interleave_info = NULL;
    }
}

// more precise visualization of the warp quality
// can be called after admin2
static void print_quality2(int iwarp, InterleaveInfo& ii, int* p) {
    int pc = (iwarp == 0);  // print warp 0
    pc = 0;                 // turn off printing
    int nodebegin = ii.lastnode[iwarp];
    int* stride = ii.stride + ii.stridedispl[iwarp];
    int ncycle = ii.cellsize[iwarp];

#if 0
  int nodeend = ii.lastnode[iwarp+1];
  int nnode = ii.lastnode[ii.nwarp];
#endif

    int inode = nodebegin;

    size_t nn = 0;  // number of nodes in warp. '.'
    size_t nx = 0;  // number of idle cores on all cycles. 'X'
    size_t ncacheline = 0;
    ;                // number of parent memory cacheline accesses.
                     //   assmue warpsize is max number in a cachline so all o
    size_t ncr = 0;  // number of child race. nchild-1 of same parent in same cycle

    for (int icycle = 0; icycle < ncycle; ++icycle) {
        int s = stride[icycle];
        int lastp = -2;
        if (pc)
            printf("  ");
        std::set<int> crace;  // how many children have same parent in a cycle
        for (int icore = 0; icore < warpsize; ++icore) {
            char ch = '.';
            if (icore < s) {
                int par = p[inode];
                if (crace.find(par) != crace.end()) {
                    ch = 'r';
                    ++ncr;
                } else {
                    crace.insert(par);
                }

                if (par != lastp + 1) {
                    ch = (ch == 'r') ? 'R' : 'o';
                    ++ncacheline;
                }
                lastp = p[inode++];
                ++nn;
            } else {
                ch = 'X';
                ++nx;
            }
            if (pc)
                printf("%c", ch);
        }
        if (pc)
            printf("\n");
    }

    ii.nnode[iwarp] = nn;
    ii.ncycle[iwarp] = size_t(ncycle);
    ii.idle[iwarp] = nx;
    ii.cache_access[iwarp] = ncacheline;
    ii.child_race[iwarp] = ncr;
    if (pc)
        printf("warp %d:  %ld nodes, %d cycles, %ld idle, %ld cache access, %ld child races\n",
               iwarp, nn, ncycle, nx, ncacheline, ncr);
}

static void print_quality1(int iwarp, InterleaveInfo& ii, int ncell, int* p) {
    int pc = ((iwarp == 0) || iwarp == (ii.nwarp - 1));  // warp not to skip printing
    pc = 0;                                              // turn off printing.
    int* stride = ii.stride;
    int cellbegin = iwarp * warpsize;
    int cellend = cellbegin + warpsize;
    cellend = (cellend < stride[0]) ? cellend : stride[0];

    int ncycle = 0;
    for (int i = cellbegin; i < cellend; ++i) {
        if (ncycle < ii.cellsize[i]) {
            ncycle = ii.cellsize[i];
        }
    }
    nrn_assert(ncycle == ii.cellsize[cellend - 1]);
    nrn_assert(ncycle <= ii.nstride);

    int ncell_in_warp = cellend - cellbegin;

    size_t n = 0;   // number of nodes in warp (not including roots)
    size_t nx = 0;  // number of idle cores on all cycles. X
    size_t ncacheline = 0;
    ;  // number of parent memory cacheline accesses.
       // assume warpsize is max number in a cachline so
       // first core has all o

    int inode = ii.firstnode[cellbegin];
    for (int icycle = 0; icycle < ncycle; ++icycle) {
        int sbegin = ncell - stride[icycle] - cellbegin;
        int lastp = -2;
        if (pc)
            printf("  ");
        for (int icore = 0; icore < warpsize; ++icore) {
            char ch = '.';
            if (icore < ncell_in_warp && icore >= sbegin) {
                int par = p[inode + icore];
                if (par != lastp + 1) {
                    ch = 'o';
                    ++ncacheline;
                }
                lastp = par;
                ++n;
            } else {
                ch = 'X';
                ++nx;
            }
            if (pc)
                printf("%c", ch);
        }
        if (pc)
            printf("\n");
        inode += ii.stride[icycle + 1];
    }

    ii.nnode[iwarp] = n;
    ii.ncycle[iwarp] = (size_t)ncycle;
    ii.idle[iwarp] = nx;
    ii.cache_access[iwarp] = ncacheline;
    ii.child_race[iwarp] = 0;
    if (pc)
        printf("warp %d:  %ld nodes, %d cycles, %ld idle, %ld cache access\n", iwarp, n, ncycle, nx,
               ncacheline);
}

static void warp_balance(int ith, InterleaveInfo& ii) {
    size_t nwarp = size_t(ii.nwarp);
    size_t smm[4][3];  // sum_min_max see cp below
    for (size_t j = 0; j < 4; ++j) {
        smm[j][0] = 0;
        smm[j][1] = 1000000000;
        smm[j][2] = 0;
    }
    double emax = 0.0, emin = 1.0;
    for (size_t i = 0; i < nwarp; ++i) {
        size_t n = ii.nnode[i];
        double e = double(n) / (n + ii.idle[i]);
        if (emax < e) {
            emax = e;
        }
        if (emin > e) {
            emin = e;
        }
        size_t s[4] = {n, ii.idle[i], ii.cache_access[i], ii.child_race[i]};
        for (size_t j = 0; j < 4; ++j) {
            smm[j][0] += s[j];
            if (smm[j][1] > s[j]) {
                smm[j][1] = s[j];
            }
            if (smm[j][2] < s[j]) {
                smm[j][2] = s[j];
            }
        }
    }
    std::vector<size_t> v(nwarp);
    for (size_t i = 0; i < nwarp; ++i) {
        v[i] = ii.ncycle[i];
    }
    double bal = load_balance(v);
#ifdef DEBUG
    printf("thread %d nwarp=%ld  balance=%g  warp_efficiency %g to %g\n", ith, nwarp, bal, emin,
           emax);
    const char* cp[4] = {"nodes", "idle", "ca", "cr"};
    for (size_t i = 0; i < 4; ++i) {
        printf("  %s=%ld (%ld:%ld)", cp[i], smm[i][0], smm[i][1], smm[i][2]);
    }
    printf("\n");
#endif
}

int* interleave_order(int ith, int ncell, int nnode, int* parent) {
    // return if there are no nodes to permute
    if (nnode <= 0)
        return NULL;

    // ensure parent of root = -1
    for (int i = 0; i < ncell; ++i) {
        if (parent[i] == 0) {
            parent[i] = -1;
        }
    }

    int nwarp, nstride, *stride, *firstnode, *lastnode, *cellsize, *stridedispl;
    int *prev_node, *next_node, *max_order_per_thread, *min_order_per_thread, *map_t2c, max_level, threads_num;
    int norder;

    prev_node = NULL;
    next_node = NULL;
    max_order_per_thread = NULL;
    min_order_per_thread = NULL;
    stride = NULL;
    map_t2c = NULL;
    //threads_num = ncell * thread_per_cell;

    int* order;
    if (use_interleave_permute == 1 || use_interleave_permute == 2)
    {
        order = node_order(ncell, nnode, parent, nwarp, nstride, stride, firstnode, lastnode, cellsize, stridedispl);
    }
    else 
    {
        order = node_order_branch(ncell, nnode, thread_per_cell, parent, norder, threads_num, prev_node, next_node, max_order_per_thread, min_order_per_thread, firstnode, lastnode, stride, map_t2c, stridedispl);
        nwarp = (threads_num + 31) / 32;
    }

    if (interleave_info) {
        InterleaveInfo& ii = interleave_info[ith];
        ii.nwarp = nwarp;
        ii.nstride = nstride;
        ii.thread_per_cell = thread_per_cell;
        ii.stridedispl = stridedispl;
        ii.stride = stride;
        ii.firstnode = firstnode;
        ii.lastnode = lastnode;
        ii.cellsize = cellsize;
        ii.map_t2c = map_t2c;

        ii.prev_node = prev_node;
        ii.next_node = next_node;
        ii.max_order_per_thread = max_order_per_thread;
        ii.min_order_per_thread = min_order_per_thread;
        ii.norder = norder;
        ii.threads_num = threads_num;

        if (0 && ith == 0 && use_interleave_permute == 1) {
            printf("ith=%d nstride=%d ncell=%d nnode=%d\n", ith, nstride, ncell, nnode);
            for (int i = 0; i < ncell; ++i) {
                printf("icell=%d cellsize=%d first=%d last=%d\n", i, cellsize[i], firstnode[i],
                       lastnode[i]);
            }
            for (int i = 0; i < nstride; ++i) {
                printf("istride=%d stride=%d\n", i, stride[i]);
            }
        }
        if (ith == 0 && (use_interleave_permute == 1 || use_interleave_permute == 2)) {
            // needed for print_quality[12] and done once here to save time
            int* p = new int[nnode];
            for (int i = 0; i < nnode; ++i) {
                p[i] = parent[i];
            }
            permute_ptr(p, nnode, order);
            node_permute(p, nnode, order);

            ii.nnode = new size_t[nwarp];
            ii.ncycle = new size_t[nwarp];
            ii.idle = new size_t[nwarp];
            ii.cache_access = new size_t[nwarp];
            ii.child_race = new size_t[nwarp];
            for (int i = 0; i < nwarp; ++i) {
                if (use_interleave_permute == 1) {
                    print_quality1(i, interleave_info[ith], ncell, p);
                }
                if (use_interleave_permute == 2) {
                    print_quality2(i, interleave_info[ith], p);
                }
            }
            delete[] p;
            warp_balance(ith, interleave_info[ith]);
        }
    }

    return order;
}

#if INTERLEAVE_DEBUG  // only the cell per core style
static int** cell_indices_debug(NrnThread& nt, InterleaveInfo& ii) {
    int ncell = nt.ncell;
    int nnode = nt.end;
    int* parents = nt._v_parent_index;

    // we expect the nodes to be interleave ordered with smallest cell first
    // establish consistency with ii.
    // first ncell parents are -1
    for (int i = 0; i < ncell; ++i) {
        nrn_assert(parents[i] == -1);
    }
    int* sz;
    int* cell;
    sz = new int[ncell];
    cell = new int[nnode];
    for (int i = 0; i < ncell; ++i) {
        sz[i] = 0;
        cell[i] = i;
    }
    for (int i = ncell; i < nnode; ++i) {
        cell[i] = cell[parents[i]];
        sz[cell[i]] += 1;
    }

    // cells are in inceasing sz order;
    for (int i = 1; i < ncell; ++i) {
        nrn_assert(sz[i - 1] <= sz[i]);
    }
    // same as ii.cellsize
    for (int i = 0; i < ncell; ++i) {
        nrn_assert(sz[i] == ii.cellsize[i]);
    }

    int** cellindices = new int*[ncell];
    for (int i = 0; i < ncell; ++i) {
        cellindices[i] = new int[sz[i]];
        sz[i] = 0;  // restart sz counts
    }
    for (int i = ncell; i < nnode; ++i) {
        cellindices[cell[i]][sz[cell[i]]] = i;
        sz[cell[i]] += 1;
    }
    // cellindices first and last same as ii first and last
    for (int i = 0; i < ncell; ++i) {
        nrn_assert(cellindices[i][0] == ii.firstnode[i]);
        nrn_assert(cellindices[i][sz[i] - 1] == ii.lastnode[i]);
    }

    delete[] sz;
    delete[] cell;

    return cellindices;
}

static int*** cell_indices_threads;
void mk_cell_indices() {
    cell_indices_threads = new int**[nrn_nthread];
    for (int i = 0; i < nrn_nthread; ++i) {
        NrnThread& nt = nrn_threads[i];
        if (nt.ncell) {
            cell_indices_threads[i] = cell_indices_debug(nt, interleave_info[i]);
        } else {
            cell_indices_threads[i] = NULL;
        }
    }
}
#endif  // INTERLEAVE_DEBUG

#if 1
#define GPU_V(i) nt->_actual_v[i]
#define GPU_A(i) nt->_actual_a[i]
#define GPU_B(i) nt->_actual_b[i]
#define GPU_D(i) nt->_actual_d[i]
#define GPU_RHS(i) nt->_actual_rhs[i]
#define GPU_PARENT(i) nt->_v_parent_index[i]

// How does the interleaved permutation with stride get used in
// triagularization?

// each cell in parallel regardless of inhomogeneous topology
static void triang_interleaved(NrnThread* nt,
                               int icell,
                               int icellsize,
                               int nstride,
                               int* stride,
                               int* lastnode) {
    int i = lastnode[icell];
    for (int istride = nstride - 1; istride >= 0; --istride) {
        if (istride < icellsize) {  // only first icellsize strides matter
            // what is the index
            int ip = GPU_PARENT(i);
#ifndef _OPENACC
            nrn_assert(ip >= 0);  // if (ip < 0) return;
#endif
            double p = GPU_A(i) / GPU_D(i);
            GPU_D(ip) -= p * GPU_B(i);
            GPU_RHS(ip) -= p * GPU_RHS(i);
            //if (i == 1416 || i == 1424 || i == 2274 || i == 2282)
            //    printf("i:%d d:%f rhs:%f ip:%d rhs[ip]:%f\n", i, GPU_D(i), GPU_RHS(i), ip, GPU_RHS(ip));
            i -= stride[istride];
        }
    }
}

// back substitution?
static void bksub_interleaved(NrnThread* nt,
                              int icell,
                              int icellsize,
                              int nstride,
                              int* stride,
                              int* firstnode) {
    if (nstride) {
    }  // otherwise unused
    int i = firstnode[icell];
    GPU_RHS(icell) /= GPU_D(icell);  // the root
    for (int istride = 0; istride < icellsize; ++istride) {
        int ip = GPU_PARENT(i);
#ifndef _OPENACC
        nrn_assert(ip >= 0);
#endif
        GPU_RHS(i) -= GPU_B(i) * GPU_RHS(ip);
        GPU_RHS(i) /= GPU_D(i);
        //if (i == 1416 || i == 1424 || i == 2274 || i == 2282)
        //    printf("i:%d d:%f rhs:%f ip:%d rhs[ip]:%f\n", i, GPU_D(i), GPU_RHS(i), ip, GPU_RHS(ip));
        i += stride[istride + 1];
    }
}

// icore ranges [0:warpsize) ; stride[ncycle]
static void triang_interleaved2(NrnThread* nt, int icore, int ncycle, int* stride, int lastnode) {
    int icycle = ncycle - 1;
    int istride = stride[icycle];
    int i = lastnode - istride + icore;
#if !defined(_OPENACC)
    int ii = i;
#endif

    bool has_subtrees_to_compute = true;

// clang-format off
    #pragma acc loop seq
    for (;has_subtrees_to_compute;) {  // ncycle loop
#if !defined(_OPENACC)
        // serial test, gpu does this in parallel
        for (int icore = 0; icore < warpsize; ++icore) {
            int i = ii + icore;
#endif
            if (icore < istride) {  // most efficient if istride equal  warpsize
                // what is the index
                int ip = GPU_PARENT(i);
                double p = GPU_A(i) / GPU_D(i);
                #pragma acc atomic update
                GPU_D(ip) -= p * GPU_B(i);
                #pragma acc atomic update
                GPU_RHS(ip) -= p * GPU_RHS(i);
            }
#if !defined(_OPENACC)
        }
#endif
        if (icycle == 0) {
            has_subtrees_to_compute = false;
            continue;
        }
        --icycle;
        istride = stride[icycle];
        i -= istride;
#if !defined(_OPENACC)
        ii -= istride;
#endif
    }
    // clang-format on
}

// icore ranges [0:warpsize) ; stride[ncycle]
static void bksub_interleaved2(NrnThread* nt,
                               int root,
                               int lastroot,
                               int icore,
                               int ncycle,
                               int* stride,
                               int firstnode) {
#if !defined(_OPENACC)
    for (int i = root; i < lastroot; i += 1) {
#else
// clang-format off
    #pragma acc loop seq
    // clang-format on
    for (int i = root; i < lastroot; i += warpsize) {
#endif
        GPU_RHS(i) /= GPU_D(i);  // the root
    }

    int i = firstnode + icore;
#if !defined(_OPENACC)
    int ii = i;
#endif
    for (int icycle = 0; icycle < ncycle; ++icycle) {
        int istride = stride[icycle];
#if !defined(_OPENACC)
        // serial test, gpu does this in parallel
        for (int icore = 0; icore < warpsize; ++icore) {
            int i = ii + icore;
#endif
            if (icore < istride) {
                int ip = GPU_PARENT(i);
                GPU_RHS(i) -= GPU_B(i) * GPU_RHS(ip);
                GPU_RHS(i) /= GPU_D(i);
            }
            i += istride;
#if !defined(_OPENACC)
        }
        ii += istride;
#endif
    }
}

#ifdef ENABLE_CUDA_INTERFACE
void solve_interleaved_launcher(NrnThread* nt, InterleaveInfo* info, int ncell);
#endif

int temp1[1024] = {0};
int temp2[1024] = {0};
int temp3[1024] = {0};

void solve_interleaved2(int ith) {
    static int foo = 1;
    NrnThread* nt = nrn_threads + ith;
    InterleaveInfo& ii = interleave_info[ith];
    int nwarp = ii.nwarp;
    if (nwarp == 0) {
        return;
    }
    int* ncycles = ii.cellsize;         // nwarp of these
    int* stridedispl = ii.stridedispl;  // nwarp+1 of these
    int* strides = ii.stride;           // sum ncycles of these (bad since ncompart/warpsize)
    int* rootbegin = ii.firstnode;      // nwarp+1 of these
    int* nodebegin = ii.lastnode;       // nwarp+1 of these
#ifdef _OPENACC
    int nstride = stridedispl[nwarp];
    int stream_id = nt->stream_id;
#endif

    int ncore = nwarp * warpsize;
#if 0 && defined(ENABLE_CUDA_INTERFACE)  // not implemented
    NrnThread* d_nt = (NrnThread*) acc_deviceptr(nt);
    InterleaveInfo* d_info = (InterleaveInfo*) acc_deviceptr(interleave_info+ith);
    solve_interleaved2_launcher(d_nt, d_info);
#else
#ifdef _OPENACC
// clang-format off
    #pragma acc parallel loop present(                  \
        nt[0:1], strides[0:nstride],                    \
        ncycles[0:nwarp], stridedispl[0:nwarp+1],       \
        rootbegin[0:nwarp+1], nodebegin[0:nwarp+1])     \
        if (nt->compute_gpu) async(stream_id)
// clang-format on
#endif
    for (int icore = 0; icore < ncore; ++icore) {
        int iwarp = icore / warpsize;     // figure out the >> value
        int ic = icore & (warpsize - 1);  // figure out the & mask
        int ncycle = ncycles[iwarp];
        int* stride = strides + stridedispl[iwarp];
        int root = rootbegin[iwarp];
        int lastroot = rootbegin[iwarp + 1];
        int firstnode = nodebegin[iwarp];
        int lastnode = nodebegin[iwarp + 1];
// temp1[icore] = ic;
// temp2[icore] = ncycle;
// temp3[icore] = stride - strides;
#if !defined(_OPENACC)
        if (ic == 0) {  // serial test mode. triang and bksub do all cores in warp
#endif
            triang_interleaved2(nt, ic, ncycle, stride, lastnode);
            bksub_interleaved2(nt, root + ic, lastroot, ic, ncycle, stride, firstnode);
#if !defined(_OPENACC)
        }  // serial test mode
#endif
    }
#ifdef _OPENACC
#pragma acc wait(nt->stream_id)
#endif
#endif
    if (foo == 1) {
        return;
    }
    foo = 0;
    for (int i = 0; i < ncore; ++i) {
        printf("%d => %d %d %d\n", i, temp1[i], temp2[i], temp3[i]);
    }
}

void solve_interleaved1(int ith) {
    NrnThread* nt = nrn_threads + ith;
    int ncell = nt->ncell;
    int nnode = nt->end;
    if (ncell == 0) {
        return;
    }
    InterleaveInfo& ii = interleave_info[ith];
    int nstride = ii.nstride;
    int* stride = ii.stride;
    int* firstnode = ii.firstnode;
    int* lastnode = ii.lastnode;
    int* cellsize = ii.cellsize;
    FILE *fp = NULL;
#if _OPENACC
    int stream_id = nt->stream_id;
#endif

#ifdef ENABLE_CUDA_INTERFACE
    NrnThread* d_nt = (NrnThread*)acc_deviceptr(nt);
    InterleaveInfo* d_info = (InterleaveInfo*)acc_deviceptr(interleave_info + ith);
    solve_interleaved_launcher(d_nt, d_info, ncell);
#else
#ifdef _OPENACC
    //fp = fopen("solve1", "ab");
// clang-format off
    #pragma acc parallel loop present(              \
        nt[0:1], stride[0:nstride],                 \
        firstnode[0:ncell], lastnode[0:ncell],      \
        cellsize[0:ncell]) if (nt->compute_gpu)     \
        async(stream_id)
// clang-format on
#endif
    for (int icell = 0; icell < ncell; ++icell) {
        int icellsize = cellsize[icell];
        triang_interleaved(nt, icell, icellsize, nstride, stride, lastnode);
        bksub_interleaved(nt, icell, icellsize, nstride, stride, firstnode);
    }
#ifdef _OPENACC
#pragma acc wait(nt->stream_id)
//#pragma acc update host(nt->_actual_d[0:nnode])
//#pragma acc update host(nt->_actual_rhs[0:nnode])
//fwrite(nt->_actual_d, sizeof(double), nnode, fp);    
//fwrite(nt->_actual_rhs, sizeof(double), nnode, fp);    
//fclose(fp);
#endif
#endif
}

void mini_solve1(int ith)
{
    NrnThread* nt = nrn_threads + ith;
    int len = nt->end;
    int ncell = nt->ncell;
    double t = nt->_t;
    double* vec_a = nt->_actual_a;
    double* vec_b = nt->_actual_b;
    double* vec_d = nt->_actual_d;
    double* vec_rhs = nt->_actual_rhs;
    int* parent_index = nt->_v_parent_index;

    InterleaveInfo& ii = interleave_info[ith];
    int nstride = ii.nstride;
    int* stride = ii.stride;
    int* firstnode = ii.firstnode;
    int* lastnode = ii.lastnode;
    int* cellsize = ii.cellsize;

    #pragma acc wait(nt->stream_id)//wait until operations before finished
    #pragma acc host_data use_device(vec_a, vec_b, vec_d, vec_rhs, parent_index, stride, firstnode, lastnode, cellsize) 
    {
        cudaStream_t stream = (cudaStream_t)acc_get_cuda_stream(nt->stream_id);
        cusolve_interleaved1(vec_a, vec_b, vec_d, vec_rhs, parent_index, nstride, stride, firstnode, lastnode, cellsize, ncell, len, t, stream);
    }
}

void mini_solve3(int ith)
{
    NrnThread* nt = nrn_threads + ith;
    int len=nt->end;
    double t=nt->_t;
    double* vec_a=nt->_actual_a;
    double* vec_b=nt->_actual_b;
    double* vec_d=nt->_actual_d;
    double* vec_rhs=nt->_actual_rhs;
    int* parent_index=nt->_v_parent_index;

    InterleaveInfo& ii = interleave_info[ith];
    int ncell=nt->ncell;
    int* prev_node = ii.prev_node;
    int* next_node = ii.next_node;
    int* max_order_per_thread = ii.max_order_per_thread;
    int* min_order_per_thread = ii.min_order_per_thread;
    int* firstnode = ii.firstnode;
    int* lastnode = ii.lastnode;
    int* stride = ii.stride;
    int* map_t2c = ii.map_t2c;
    //int* stride_p = ii.stride_p;
    int nthread = ii.threads_num;
    int norder = ii.norder;

    //#pragma acc wait(nt->stream_id)//wait until operations before finished
    
    //printf("***CPU***\n");
    //for (int i = 2394; i <= 2618; i++)
    //    printf("%f ", vec_d[i]);
    //printf("\n\n");
    if (nt->compute_gpu)
    {
        #pragma acc host_data use_device(vec_a, vec_b, vec_d, vec_rhs, parent_index, prev_node, next_node, max_order_per_thread, min_order_per_thread, firstnode, lastnode, stride, map_t2c) 
        {
            //cusolve_interleaved1(vec_a,vec_b,vec_d,vec_rhs,parent_index,nstride,stride,firstnode,lastnode,cellsize,ncell,len,t);
            cudaStream_t stream = (cudaStream_t)acc_get_cuda_stream(nt->stream_id);
            solve_branch(vec_a, vec_b, vec_d, vec_rhs, parent_index, prev_node, next_node,
                         max_order_per_thread, min_order_per_thread, firstnode, lastnode, 
                         stride, map_t2c, ncell, nthread, norder, len, t, stream);
            //solve_warp(vec_a, vec_b, vec_d, vec_rhs, parent_index, firstnode, lastnode, 
            //           prev_node, next_node, max_order_per_thread, min_order_per_thread,
            //           nthread, norder, ncell, len, t, stream);
        }
    }
    
}

void solve_interleaved(int ith) {
    if (use_interleave_permute == 2) {
        solve_interleaved2(ith);
    } else if (use_interleave_permute == 1){
        //solve_interleaved1(ith);
        mini_solve1(ith);
    } else {
        mini_solve3(ith);
    }
}
}  // namespace coreneuron
#endif

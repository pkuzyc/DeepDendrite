#ifndef cellorder_h
#define cellorder_h

#include <algorithm>
namespace coreneuron {
int* interleave_order(int ith, int ncell, int nnode, int* parent);

void create_interleave_info();
void destroy_interleave_info();

class InterleaveInfo {
  public:
    InterleaveInfo();
    InterleaveInfo(const InterleaveInfo&);
    InterleaveInfo& operator=(const InterleaveInfo&);
    virtual ~InterleaveInfo();
    int nwarp;  // used only by interleave2
    int nstride;
    int* stridedispl;  // interleave2: nwarp+1
    int* stride;       // interleave2: stride  length is stridedispl[nwarp]
    int* firstnode;    // interleave2: rootbegin nwarp+1 displacements
    int* lastnode;     // interleave2: nodebegin nwarp+1 displacements
    int* cellsize;     // interleave2: ncycles nwarp

    int* prev_node;
    int* next_node;
    int* max_order_per_thread;
    int* min_order_per_thread;
    int* map_t2c;
    int norder;
    int threads_num;
    int thread_per_cell;

    // statistics (nwarp of each)
    size_t* nnode;
    size_t* ncycle;
    size_t* idle;
    size_t* cache_access;
    size_t* child_race;

  private:
    void swap(InterleaveInfo& info);
};

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
                int*& stridedispl);

// interleaved from cellorder2.cpp
int* node_order(int ncell,
                int nnode,
                int* parents,
                int& nwarp,
                int& nstride,
                int*& stride,
                int*& firstnode,
                int*& lastnode,
                int*& cellsize,
                int*& stridedispl);

// copy src array to dest with new allocation
template <typename T>
void copy_array(T*& dest, T* src, size_t n) {
    dest = new T[n];
    std::copy(src, src + n, dest);
}

#define INTERLEAVE_DEBUG 0

#if INTERLEAVE_DEBUG
void mk_cell_indices();
#endif
}  // namespace coreneuron
#endif

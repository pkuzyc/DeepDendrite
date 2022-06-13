#include <cuda_runtime_api.h>

extern "C"
{
extern void hello();
extern void vec_add(double*, double*, int);
}

void cusolve_interleaved1(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell,
                        int len, double t, cudaStream_t stream);

void solve_branch(double* vec_a, 
                  double* vec_b, 
                  double* vec_d, 
                  double* vec_rhs, 
                  int* parent_index,
                  int* prev_node,
                  int* next_node,
                  int* max_order_per_thread,
                  int* min_order_per_thread,
                  int* firstnode,
                  int* lastnode,
                  int* stride,
                  int* map_t2c,
                  int ncell,
                  int nthread,
                  int norder,
                  int len,
                  double t,
                  cudaStream_t stream);

void solve_warp(double* vec_a,
                double* vec_b, 
                double* vec_d,
                double* vec_rhs,
                int* parent_index,
                int* firstnode,
                int* lastnode,
                int* prev_node,
                int* next_node,
                int* max_level_per_thread,
                int* min_level_per_thread,
                int threads_num,
                int max_level,
                int ncell,
                int len,
                double t,
                cudaStream_t stream);


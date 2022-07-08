#pragma once
#include <cuda.h>
#include <cuda_runtime.h>

extern __global__ void solve_kernel0(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, 
        int* parent_index, int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell);

extern __global__ void kernel16(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int norder, int ncell, int nthread, double t);



extern __global__ void kernel17(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* stride, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int norder, int ncell, int nthread);

extern __global__ void kernel18(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* stride, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int norder, int ncell, int nthread);

//extern __global__ void kernel17(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
//                        int* stride, int* max_order_per_thread, int* min_order_per_thread, 
//                        int* firstnode, int* lastnode, int norder, int ncell, int nthread, int* prev_node, int* next_node);



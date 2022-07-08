#include <cuda_runtime.h>
#include <assert.h>
#include <stdio.h>
#include "solver_kernels.cuh"


__global__ void kernel17(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* stride, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int norder, int ncell, int nthread)
{
    unsigned int tid;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip;
    //int icell;
    double p;
    int min_order;
    
    tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < nthread)
    {
        //start_pos = threadIdx.x / 32 * (norder + 1);
        //start_pos = tid / 32 * (norder + 1);

        //max_order = max_order_per_thread[tid];
        min_order = min_order_per_thread[tid];

        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            
            if(iorder >= min_order && i > -1)
            {
                ip = parent_index[i];
                p = vec_a[i] / vec_d[i];
                atomicAdd(vec_d + ip, -p * vec_b[i]);
                atomicAdd(vec_rhs + ip, -p * vec_rhs[i]);

                i -= stride[(tid >> 5) * (norder + 1) + iorder - 1];
            }
            //__syncthreads();
        }


        //ncell_per_warp = 32 / (nthread / ncell);
        int ncell_each_warp = 32 / (nthread / ncell);
        int icell = (tid >> 5) * (32 / (nthread / ncell)) + (tid & 31);
        i=firstnode[tid];
        if ((tid & 31) < ncell_each_warp) 
        {
            if (icell < ncell)
                vec_rhs[icell] /= vec_d[icell];
        }
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && i > -1)
            {
                ip = parent_index[i];

                p = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                vec_rhs[i] = p / vec_d[i];
                i += stride[(tid >> 5) * (norder + 1) + iorder];
            }
        }
    }
    
}








#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
#include <assert.h>
#include <stdio.h>
#include "solver_kernels.cuh"


__global__ void kernel18(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* stride, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int norder, int ncell, int nthread)
{
    unsigned int tid;
    int i, iorder, ip, next_i, next_ip;
    double p;
    int min_order;
    double cur_a, cur_b, next_a, next_b;
    //start_pos = blockIdx.x * (blockDim.x / 32) * (norder + 1);
    tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < nthread)
    {
        int start_pos = (tid >> 5) * (norder + 1) - 1;
        //start_pos = tid / 32 * (norder + 1);

        //max_order = max_order_per_thread[tid];
        min_order = min_order_per_thread[tid];

        i = lastnode[tid];
        next_a = vec_a[i];
        next_b = vec_b[i];
        next_ip = parent_index[i];

        for(iorder = norder; iorder >= 0; iorder--)
        {
            if(iorder >= min_order && i > -1)
            {
                next_i = i - stride[start_pos + iorder];
                cur_a = next_a;
                cur_b = next_b;
                ip = next_ip;
                if (next_i > -1)
                {
                    next_a = vec_a[next_i];
                    next_b = vec_b[next_i];
                    next_ip = parent_index[next_i];
                }

                p = cur_a / vec_d[i];
                atomicAdd(vec_d + ip, -p * cur_b);
                atomicAdd(vec_rhs + ip, -p * vec_rhs[i]);
                i = next_i;
            }
        }


        int ncell_each_warp = 32 / (nthread / ncell);
        int icell = (tid >> 5) * (32 / (nthread / ncell)) + (tid & 31);
        i=firstnode[tid];
        //next_i=firstnode[tid];
        if ((tid & 31) < ncell_each_warp) 
        {
            if (icell < ncell)
                vec_rhs[icell] /= vec_d[icell];
        }

        next_b = vec_b[i];
        next_ip = parent_index[i];
        start_pos = (tid >> 5) * (norder + 1);
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && i > -1)
            {
                next_i = i + stride[start_pos + iorder];
                ip = next_ip;
                next_ip = parent_index[next_i];
                cur_b = next_b;
                next_b = vec_b[next_i];
                //ip = parent_index[i];
                //p = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                p = vec_rhs[i] - cur_b * vec_rhs[ip];
                vec_rhs[i] = p / vec_d[i];
                i = next_i;
                //i += stride[start_pos + iorder];
            }
        }

    }

    tid += nthread / 2;
    if (tid < nthread)
    {
        int start_pos = (tid >> 5) * (norder + 1) - 1;
        //start_pos = tid / 32 * (norder + 1);

        //max_order = max_order_per_thread[tid];
        min_order = min_order_per_thread[tid];

        i = lastnode[tid];
        next_a = vec_a[i];
        next_b = vec_b[i];
        next_ip = parent_index[i];

        for(iorder = norder; iorder >= 0; iorder--)
        {
            if(iorder >= min_order && i > -1)
            {
                next_i = i - stride[start_pos + iorder];
                cur_a = next_a;
                cur_b = next_b;
                ip = next_ip;
                if (next_i > -1)
                {
                    next_a = vec_a[next_i];
                    next_b = vec_b[next_i];
                    next_ip = parent_index[next_i];
                }

                p = cur_a / vec_d[i];
                atomicAdd(vec_d + ip, -p * cur_b);
                atomicAdd(vec_rhs + ip, -p * vec_rhs[i]);
                i = next_i;
            }
        }


        int ncell_each_warp = 32 / (nthread / ncell);
        int icell = (tid >> 5) * (32 / (nthread / ncell)) + (tid & 31);
        i=firstnode[tid];
        //next_i=firstnode[tid];
        if ((tid & 31) < ncell_each_warp) 
        {
            if (icell < ncell)
                vec_rhs[icell] /= vec_d[icell];
        }

        next_b = vec_b[i];
        next_ip = parent_index[i];
        start_pos = (tid >> 5) * (norder + 1);
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && i > -1)
            {
                next_i = i + stride[start_pos + iorder];
                ip = next_ip;
                next_ip = parent_index[next_i];
                cur_b = next_b;
                next_b = vec_b[next_i];
                //ip = parent_index[i];
                //p = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                p = vec_rhs[i] - cur_b * vec_rhs[ip];
                vec_rhs[i] = p / vec_d[i];
                i = next_i;
                //i += stride[start_pos + iorder];
            }
        }

    }
    
}








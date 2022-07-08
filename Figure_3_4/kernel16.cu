#include <cusparse.h>
#include <cusolverSp.h>
#include <cuda_runtime_api.h>
#include <assert.h>
#include <stdio.h>
#include "solver_kernels.cuh"



__global__ void kernel16(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell;
    double p;
    double temp, reg_rhs;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);
    while (tid < nthread)
    {
        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            
            if(iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                //if (i < 0 || i >= 407168 || ip < 0 || ip >= 407168)
                //    printf("error icell:%d lastnode:%d i:%d ip:%d stride:%d\n", icell, lastnode[icell], i, ip, stride[istride]);
                p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                //temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                //atomicAdd(vec_d + ip, temp);
		atomicAdd(vec_d + ip, -p * vec_b[i]);
                //temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);
                //atomicAdd(vec_rhs + ip, temp);
		atomicAdd(vec_rhs + ip, -p * vec_rhs[i]);

                i = prev_node[i];
            }
            //__syncthreads();
        }


        ncell_per_warp = 32 / (nthread / ncell);
        i=firstnode[tid];
        if (tid % 32 < ncell_per_warp)
        {
            icell = tid / 32 * ncell_per_warp + tid % 32;
            //if (icell < 4)
            //    printf("tid:%d icell:%d\n", tid, icell);
            if (icell < ncell)
                vec_rhs[icell] /= vec_d[icell];
        }
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                
                reg_rhs = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                vec_rhs[i] = reg_rhs / vec_d[i];
                //vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                //vec_rhs[i] /= vec_d[i];
                i = next_node[i];    
            }
            //__syncthreads();
            
        }
        tid += thread_stride;
    }
}


/*__global__ void kernel16(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, bool* reg_avai, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockidx.x * blockdim.x + threadidx.x;
    unsigned int thread_stride = blockdim.x * griddim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell;
    double p;
    double temp, reg_rhs, reg_d;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);
    while (tid < nthread)
    {
        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            
            if(iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];

                
                //if (i < 0 || i >= 407168 || ip < 0 || ip >= 407168)
                //    printf("error icell:%d lastnode:%d i:%d ip:%d stride:%d\n", icell, lastnode[icell], i, ip, stride[istride]);
                p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                temp = -p * vec_b[i];
                atomicdoubleadd(vec_d + ip, temp);
                temp = -p * vec_rhs[i];
                atomicdoubleadd(vec_rhs + ip, temp);

                i = prev_node[i];
            }
            //__syncthreads();
        }


        ncell_per_warp = 32 / (nthread / ncell);
        i=firstnode[tid];
        if (tid % 32 < ncell_per_warp)
        {
            icell = tid / 32 * ncell_per_warp + tid % 32;
            //if (icell < 4)
            //    printf("tid:%d icell:%d\n", tid, icell);
            if (icell < ncell)
                vec_rhs[icell] /= vec_d[icell];
        }

        //use_reg = 0;
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip = parent_index[i];           
                reg_rhs = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                vec_rhs[i] = reg_rhs / vec_d[i];
                //vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                //vec_rhs[i] /= vec_d[i];
                i = next_node[i];
            }
            //__syncthreads();
            
        }
        tid += thread_stride;
    }
}*/

//kernel16 with opt
/*__global__ void kernel16(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, bool* reg_avai, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell;
    bool use_reg = false, use_reg2 = false;
    double p;
    double temp, reg_rhs, reg_d;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);

    extern __shared__ bool s_reg_avai[];
    for (i = 0; i <= norder; i++)
    {
        s_reg_avai[i * blockDim.x + threadIdx.x] = reg_avai[i * 128 + tid];
    }

    while (tid < nthread)
    {
        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            
            if(iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
    
                if (!use_reg)
                {
                    p = vec_a[i] / vec_d[i];
                }
                else
                {
                    p = vec_a[i] / reg_d;
                }
                use_reg2 = s_reg_avai[(iorder - 1) * blockDim.x + threadIdx.x];
                //s_reg_avai[iorder * blockDim.x + threadIdx.x] = use_reg;
                temp = -p * vec_b[i];
                if (use_reg2)
                {
                    reg_d = vec_d[ip] + temp;
                    vec_d[ip] = reg_d;
                    if (use_reg)
                    {
                        reg_rhs = vec_rhs[ip] - p * reg_rhs;
                    }
                    else
                    {
                        reg_rhs = vec_rhs[ip] - p * vec_rhs[i];
                    }
                    vec_rhs[ip] = reg_rhs;
                }
                else
                {
                    atomicDoubleAdd(vec_d + ip, temp);
                    if (use_reg)
                    {
                        temp = -p * reg_rhs;
                    }
                    else
                    {
                        temp = -p * vec_rhs[i];
                    }
                    atomicDoubleAdd(vec_rhs + ip, temp);
                }
                
                //p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                //temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                //temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);

                use_reg = use_reg2;
                i = prev_node[i];
            }
        }


        ncell_per_warp = 32 / (nthread / ncell);
        i=firstnode[tid];
        if (tid % 32 < ncell_per_warp)
        {
            icell = tid / 32 * ncell_per_warp + tid % 32;
            //if (icell < 4)
            //    printf("tid:%d icell:%d\n", tid, icell);
            if (icell < ncell)
                vec_rhs[icell] /= vec_d[icell];
        }

        //use_reg = 0;
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip = parent_index[i];           
                if (s_reg_avai[(iorder - 1) * blockDim.x + threadIdx.x])
                //{
                //    temp = vec_rhs[i] - vec_b[i] * reg_rhs;
                //}
                //else
                //{
                //    temp = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                //}
                //vec_rhs[i] = reg_rhs = temp / vec_d[i];
                reg_rhs = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                vec_rhs[i] = reg_rhs / vec_d[i];
                //vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                //vec_rhs[i] /= vec_d[i];
                i = next_node[i];
            }
            //__syncthreads();
            
        }
        tid += thread_stride;
    }
}*/





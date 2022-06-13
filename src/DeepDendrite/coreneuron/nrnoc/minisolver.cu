#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minisolver.h"
#include <math.h>


extern "C" __device__ void data_output(double* vec_a,double* vec_b,int len)
{
    double error=0;
    int i;
    for(i=0;i<len;i++)
        error+=(vec_a[i]-vec_b[i])*(vec_a[i]-vec_b[i]);
    printf("gpu error:%f ",error);
}

#if __CUDA_ARCH__ < 600
__device__ double atomicDoubleAdd(double* address, double val)
{
    unsigned long long int* address_as_ull = (unsigned long long int*)address;
    unsigned long long int old = *address_as_ull, assumed;
    do {
        assumed = old;
        old = atomicCAS(address_as_ull, assumed, __double_as_longlong(val + __longlong_as_double(assumed)));
    } while(assumed != old);
    return __longlong_as_double(old);
}
#endif

/*extern "C" void qrsolve(int* csr_row_ptr_a,int* csr_col_ind_a,double* csr_val_a,double* b_vec,double* x_vec,double* vec_rhs,int n,int i2)
{
    cusolverStatus_t status;
    cusparseStatus_t sparse_status;
    cusolverSpHandle_t handle;
    cusparseMatDescr_t descrA;
    int singuarity;
    int nnz=3*n-2*i2;

    status=cusolverSpCreate(&handle);
    if(status!=CUSOLVER_STATUS_SUCCESS)
    {
        printf("cusovler sp create error\n");
    }
    sparse_status=cusparseCreateMatDescr(&descrA);
    if(sparse_status!=CUSOLVER_STATUS_SUCCESS)
    {
        printf("mat descr create error\n");
    }
    status=cusolverSpDcsrlsvqr(handle,n,nnz,descrA,csr_val_a,csr_row_ptr_a,csr_col_ind_a,b_vec,1.0e-3,0,vec_rhs,&singuarity);
    cusolverSpDestroy(handle);
    //cudaMemcpy(vec_rhs,x_vec,sizeof(double)*n,cudaMemcpyDeviceToDevice);
}*/

__global__ void triang_kernel(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                            int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell)
{
    unsigned int tid;
    unsigned int thread_stride;
    int i, icellsize;
    int istride,ip;
    double p;
    tid = blockIdx.x * blockDim.x + threadIdx.x;
    thread_stride = blockDim.x * gridDim.x;
    while (tid < ncell)
    {
        icellsize = cellsize[tid];
        i = lastnode[tid];
        for(istride=nstride-1;istride>=0;--istride)
        {
            if(istride<icellsize)
            {
                ip=parent_index[i];
                p = vec_a[i]/vec_d[i];
                vec_d[ip] -= p*vec_b[i];
                vec_rhs[ip] -= p*vec_rhs[i];
                i -= stride[istride];
            }
        }
        tid += thread_stride;
    }
}

__global__ void bksub_kernel(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                            int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell)
{
    unsigned int tid, thread_stride;
    int i, icellsize;
    int istride, ip;
    tid = blockIdx.x * blockDim.x + threadIdx.x;
    thread_stride = blockDim.x * gridDim.x;
    while (tid < ncell)
    {
        icellsize = cellsize[tid];
        i = firstnode[tid];
        vec_rhs[tid] /= vec_d[tid];
        for(istride=0;istride<icellsize;++istride)
        {
            ip=parent_index[i];
            vec_rhs[i] -= vec_b[i]*vec_rhs[ip];
            vec_rhs[i] /= vec_d[i];
            i += stride[istride+1];
        }
        //if (tid == 0)
        //    printf("%f %f\n", vec_d[0], vec_rhs[0]);
        tid += thread_stride;
    } 
}

//__global__ void solve_kernel(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
//                            int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell)
//{
//    unsigned int tid=blockIdx.x*blockDim.x+threadIdx.x;
//    unsigned int thread_stride=blockDim.x*gridDim.x;
//    while(tid<ncell)
//    {
//        triang_kernel(vec_a,vec_b,vec_d,vec_rhs,parent_index,tid,cellsize[tid],nstride,stride,lastnode);
//        bksub_kernel(vec_a,vec_b,vec_d,vec_rhs,parent_index,tid,cellsize[tid],nstride,stride,firstnode);
//        tid += thread_stride;
//    }
//}

__global__ void triang(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* lastnode, int* cellsize, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip;
    double p;
    double temp;
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
                //printf("iorder:%d tid:%d i:%d prev_i:%d\n", iorder, tid, i, prev_node[i]);   
                ip=parent_index[i];
                //if (i < 0 || i >= 407168 || ip < 0 || ip >= 407168)
                //    printf("error icell:%d lastnode:%d i:%d ip:%d stride:%d\n", icell, lastnode[icell], i, ip, stride[istride]);
                p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                atomicAdd(vec_d + ip, temp);
                temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);
                atomicAdd(vec_rhs + ip, temp);

                i = prev_node[i];
            }
            //__syncthreads();
        }
        tid += thread_stride;
    }
}

__global__ void bksub(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                    int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                    int* firstnode, int* cellsize, int norder, int ncell, int nthread)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    ncell_per_warp = 32 / (nthread / ncell);

    while (tid < nthread)
    {
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
                
                vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                vec_rhs[i] /= vec_d[i];
                i = next_node[i];    
            }
            //__syncthreads();
            
        }
        tid += thread_stride;
    }
}

void __global__ triang_kernel_warp(double* vec_a,
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
                                int* children_num,
                                int threads_num,
                                int max_level,
                                int ncell,
                                int len)
{

    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    int max_level_local, min_level_local;
    int i, ip, level;
    double p, temp;

    while (tid < threads_num)
    {
        max_level_local = max_level_per_thread[tid];
        min_level_local = min_level_per_thread[tid];

        i = lastnode[tid];
        for (level = max_level; level > 0; level--)
        {
            if (level <= max_level_local && level >= min_level_local)
            {
                ip = parent_index[i];
                p = vec_a[i] / vec_d[i];

                //if (ip == 826 || ip == 775)
                //    printf("\nbefore p:%f b[%d]:%f d[%d]:%f d[%d]:%f\n", p, i, vec_b[i], i, vec_d[i], ip, vec_d[ip]);

                temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                atomicAdd(vec_d + ip, temp);

                //if (ip == 826 || ip == 775)
                //    printf("after p:%f b[%d]:%f d[%d]:%f d[%d]:%f\n", p, i, vec_b[i], i, vec_d[i], ip, vec_d[ip]);

                temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);
                atomicAdd(vec_rhs + ip, temp);
                
                //__threadfence_block();
                i = prev_node[i];
            }
            __syncthreads();
        }
        tid += thread_stride;
    }
}

void __global__ bksub_kernel_warp(double* vec_a,
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
                                int* children_num,
                                int threads_num,
                                int max_level,
                                int ncell,
                                int len)
{

    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    int max_level_local, min_level_local;
    int i, ip, level;
    double  temp;

    while(tid < threads_num)
    {
        max_level_local = max_level_per_thread[tid];
        min_level_local = min_level_per_thread[tid];
        i = firstnode[tid];
        if (tid == 0)
            vec_rhs[0] /= vec_d[0];
        for (level = 1; level <= max_level; level++)
        {
            if (level <= max_level_local && level >= min_level_local)
            {
                ip = parent_index[i];
                vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                vec_rhs[i] /= vec_d[i];
                i = next_node[i];
                //__threadfence_block();
            }
            __syncthreads();
        }

        tid += thread_stride;
    }
}

void __global__ solve_kernel_warp(double* vec_a,
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
                                int len)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    int max_level_local, min_level_local;
    int i, ip, level, icell;
    double p, temp;
    //printf("tid:%d max_level:%d min_level:%d lastnode:%d firstnode:%d\n", tid, max_level_per_thread[tid], min_level_per_thread[tid], lastnode[tid], firstnode[tid]);

    while (tid < threads_num)
    {
        max_level_local = max_level_per_thread[tid];
        min_level_local = min_level_per_thread[tid];
        //printf("tid:%d firstnode:%d lastnode:%d max_level:%d, min_level:%d\n", tid, firstnode[tid], lastnode[tid], max_level_local, min_level_local);

        i = lastnode[tid];
        for (level = max_level; level > 0; level--)
        {
            //if (tid == 0)
            //{
            //    printf("level:%d i:%d prev:%d max_level:%d min_level:%d\n", level, i, prev_node[i], max_level_local, min_level_local);
            //}
            if (level <= max_level_local && level >= min_level_local)
            {
                ip = parent_index[i];
                //printf("tid:%d level:%d i:%d ip:%d firstnode:%d lastnode:%d min_level:%d max_level:%d\n", tid, level, i, ip, firstnode[tid], lastnode[tid], min_level_local, max_level_local);
                p = vec_a[i] / vec_d[i];
                //if (children_num[ip] > 1)
                //{
                    temp = -p * vec_b[i];
                    //atomicDoubleAdd(vec_d + ip, temp);
                    atomicAdd(vec_d + ip, temp);
                    temp = -p * vec_rhs[i];
                    //atomicDoubleAdd(vec_rhs + ip, temp);
                    atomicAdd(vec_rhs + ip, temp);
                //}
                //else
                //{
                //    vec_d[ip] -= p * vec_b[i];
                //    vec_rhs[ip] -= p * vec_rhs[i];
                //}
                i = prev_node[i];
            }
            __syncthreads();
        }

        i = firstnode[tid];
        if (tid % (threads_num / ncell) == 0)
        {
            icell = tid / (threads_num / ncell);
            vec_rhs[icell] /= vec_d[icell];
        }
        //if (tid == 0)
        //    vec_rhs[0] /= vec_d[0];
        for (level = 1; level <= max_level; level++)
        {
            if (level <= max_level_local && level >= min_level_local)
            {
                //if (tid == 0)
                //{
                //    printf("level:%d i:%d prev:%d\n", level, i, prev_node[i]);
                //}
                ip = parent_index[i];
                vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                vec_rhs[i] /= vec_d[i];
                i = next_node[i];
            }
            __syncthreads();
        }
        //if (tid == 0)
        //    printf("\n\n");

        tid += thread_stride;
    }
}

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
                cudaStream_t stream)
{
    unsigned int threads_per_block = threads_num / ncell;
    unsigned int block_num = (threads_num + threads_per_block - 1) / threads_per_block;

    double *a_host, *b_host, *d_host, *rhs_host;
    int *parent_host, *children_host, *firstnode_host, *lastnode_host, *min_level_host, *max_level_host, *next_host, *prev_host;
    if (t - 50 > 0 && t - 50 < 0.025)
    {
        a_host = (double*)malloc(sizeof(double) * len);
        b_host = (double*)malloc(sizeof(double) * len);
        d_host = (double*)malloc(sizeof(double) * len);
        rhs_host = (double*)malloc(sizeof(double) * len);
        parent_host = (int*)malloc(sizeof(int) * len);
        //children_host = (int*)malloc(sizeof(int) * len);
        next_host = (int*)malloc(sizeof(int) * len);
        prev_host = (int*)malloc(sizeof(int) * len);
        firstnode_host = (int*)malloc(sizeof(int) * threads_num);
        lastnode_host = (int*)malloc(sizeof(int) * threads_num);
        max_level_host = (int*)malloc(sizeof(int) * threads_num);
        min_level_host = (int*)malloc(sizeof(int) * threads_num);

        cudaMemcpy(a_host, vec_a, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(b_host, vec_b, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(rhs_host, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(parent_host, parent_index, sizeof(int) * len, cudaMemcpyDeviceToHost);
        //cudaMemcpy(children_host, children_num, sizeof(int) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(next_host, next_node, sizeof(int) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(prev_host, prev_node, sizeof(int) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(firstnode_host, firstnode, sizeof(int) * threads_num, cudaMemcpyDeviceToHost);
        cudaMemcpy(lastnode_host, lastnode, sizeof(int) * threads_num, cudaMemcpyDeviceToHost);
        cudaMemcpy(max_level_host, max_level_per_thread, sizeof(int) * threads_num, cudaMemcpyDeviceToHost);
        cudaMemcpy(min_level_host, min_level_per_thread, sizeof(int) * threads_num, cudaMemcpyDeviceToHost);

        FILE* fp;
        fp = fopen("cell_data", "wb");
        fwrite(a_host, sizeof(double), len, fp);
        fwrite(b_host, sizeof(double), len, fp);
        fwrite(d_host, sizeof(double), len, fp);
        fwrite(rhs_host, sizeof(double), len, fp);
        fwrite(parent_host, sizeof(int), len, fp);
        //fwrite(children_host, sizeof(int), len, fp);
        fwrite(next_host, sizeof(int), len, fp);
        fwrite(prev_host, sizeof(int), len, fp);
        fwrite(firstnode_host, sizeof(int), threads_num, fp);
        fwrite(lastnode_host, sizeof(int), threads_num, fp);
        fwrite(max_level_host, sizeof(int), threads_num, fp);
        fwrite(min_level_host, sizeof(int), threads_num, fp);
        fclose(fp);

        fp = fopen("cell_profile", "w");
        fprintf(fp, "%d\n%d\n%d\n%d\n", ncell, len, threads_num, max_level);
        fclose(fp);
        
    }
    solve_kernel_warp<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, firstnode, lastnode, prev_node, next_node, max_level_per_thread, min_level_per_thread, threads_num, max_level, ncell, len);
    cudaDeviceSynchronize();

    if (t - 50 > 0 && t - 50 < 0.025)
    {
        FILE* result_out;
        cudaMemcpy(rhs_host, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        result_out=fopen("result_out.txt","w");
        for(int i = 0; i < len; i++)
            fprintf(result_out, "%f ", rhs_host[i]);
        fprintf(result_out, "\n");
        for(int i = 0; i < len; i++)
            fprintf(result_out, "%f ", d_host[i]);
        fclose(result_out);

    }

    //printf("block_num:%d threads_per_block:%d\n", block_num, threads_per_block);
    /*
    double rhs[2000];
    double d[2000];
    FILE* fp;
    if (t < 0.025)
    {
        cudaMemcpy(rhs, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        fp = fopen("rhs_before_3.txt", "w");
        for (int i = 0; i < len; i++)
            fprintf(fp, "%f ", rhs[i]);
        fclose(fp);

        cudaMemcpy(d, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        fp = fopen("d_before_3.txt", "w");
        for (int i = 0; i < len; i++)
            fprintf(fp, "%f ", d[i]);
        fclose(fp);
    }

    triang_kernel_warp<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, firstnode, lastnode, prev_node, 
                                                                next_node, max_level_per_thread, min_level_per_thread, children_num, threads_num, max_level, ncell, len);
    cudaDeviceSynchronize();

    if (t < 0.025)
    {
        cudaMemcpy(rhs, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        fp = fopen("rhs_after_triang_3.txt", "w");
        for (int i = 0; i < len; i++)
            fprintf(fp, "%f ", rhs[i]);
        fclose(fp);

        cudaMemcpy(d, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        fp = fopen("d_after_triang_3.txt", "w");
        for (int i = 0; i < len; i++)
            fprintf(fp, "%f ", d[i]);
        fclose(fp);
    }


    bksub_kernel_warp<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, firstnode, lastnode, prev_node, 
                                                                next_node, max_level_per_thread, min_level_per_thread, children_num, threads_num, max_level, ncell, len);
    cudaDeviceSynchronize();
    if (t < 0.025)
    {
        cudaMemcpy(rhs, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        fp = fopen("rhs_after_3.txt", "w");
        for (int i = 0; i < len; i++)
            fprintf(fp, "%f ", rhs[i]);
        fclose(fp);

        cudaMemcpy(d, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        fp = fopen("d_after_3.txt", "w");
        for (int i = 0; i < len; i++)
            fprintf(fp, "%f ", d[i]);
        fclose(fp);
    }*/

}

__global__ void kernel16_triang(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int* stride, int* map_t2c, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell, iwarp;
    double p;
    double temp;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);
    iwarp = tid / 32;
    while (tid < nthread)
    {
        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            if(iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                atomicAdd(vec_d + ip, temp);
                temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);
                atomicAdd(vec_rhs + ip, temp);

                i = prev_node[i];
                //i -= stride[iwarp * (norder + 1) + iorder - 1];
            }
            //__syncthreads();
        }
        tid += thread_stride;
    }
}

__global__ void kernel16_bksub(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int* stride, int* map_t2c, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell, iwarp;
    double p;
    double temp;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);
    while (tid < nthread)
    {
        iwarp = tid / 32;
        icell = map_t2c[tid];
        if (icell > -1)
            vec_rhs[icell] /= vec_d[icell];
        //ncell_per_warp = 32 / (nthread / ncell);
        i = firstnode[tid];
        //if (tid % 32 < ncell_per_warp)
        //{
        //    icell = tid / 32 * ncell_per_warp + tid % 32;
        //    //if (icell < 4)
        //    //    printf("tid:%d icell:%d\n", tid, icell);
        //    if (icell < ncell)
        //        vec_rhs[icell] /= vec_d[icell];
        //}
        for(iorder = 0; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                
                //reg_rhs = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                //vec_rhs[i] = reg_rhs / vec_d[i];
                vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                vec_rhs[i] /= vec_d[i];
                i = next_node[i];    
                //i += stride[iwarp * (norder + 1) + iorder];
            }
            //__syncthreads();
            
        }
        tid += thread_stride;
    }
}

__global__ void kernel16(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int* stride, int* map_t2c, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell, iwarp;
    double p;
    double temp;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);
    iwarp = tid / 32;
    while (tid < nthread)
    {
        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            if(iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                atomicAdd(vec_d + ip, temp);
                temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);
                atomicAdd(vec_rhs + ip, temp);

                //i = prev_node[i];
                i -= stride[iwarp * (norder + 1) + iorder - 1];
            }
            //__syncthreads();
        }

        icell = map_t2c[tid];
        if (icell > -1)
            vec_rhs[icell] /= vec_d[icell];
        //ncell_per_warp = 32 / (nthread / ncell);
        i = firstnode[tid];
        //if (tid % 32 < ncell_per_warp)
        //{
        //    icell = tid / 32 * ncell_per_warp + tid % 32;
        //    //if (icell < 4)
        //    //    printf("tid:%d icell:%d\n", tid, icell);
        //    if (icell < ncell)
        //        vec_rhs[icell] /= vec_d[icell];
        //}
        for(iorder = 1; iorder <= norder; ++iorder)
        {
            if (iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                
                //reg_rhs = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                //vec_rhs[i] = reg_rhs / vec_d[i];
                vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                vec_rhs[i] /= vec_d[i];
                //i = next_node[i];    
                i += stride[iwarp * (norder + 1) + iorder];
            }
            //__syncthreads();
            
        }
        //if (tid == 0)
        //    printf("%f %f\n", vec_d[0], vec_rhs[0]);
        tid += thread_stride;
    }
}

__global__ void kernel16_tp(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int* prev_node, int* next_node, int* max_order_per_thread, int* min_order_per_thread, 
                        int* firstnode, int* lastnode, int* stride, int* map_t2c, int norder, int ncell, int nthread, double t)
{
    unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int thread_stride = blockDim.x * gridDim.x;
    //unsigned int icellsize = cellsize[tid];
    int i, iorder, ip, ncell_per_warp, icell, iwarp;
    double p;
    double temp;
    int max_order = max_order_per_thread[tid];
    int min_order = min_order_per_thread[tid];
    //if ((icell == 0 || icell == 636) )
    //    printf("%d %d\n", icell, iter);
    iwarp = tid / 32;
    while (tid < nthread)
    {
        i = lastnode[tid];
        for(iorder = norder; iorder >= 0; iorder--)
        {
            if(iorder >= min_order && iorder <= max_order && i > -1)
            {
                ip=parent_index[i];
                p = vec_a[i]/vec_d[i];
                //vec_d[ip] -= p*vec_b[i];
                //vec_rhs[ip] -= p*vec_rhs[i];
                temp = -p * vec_b[i];
                //atomicDoubleAdd(vec_d + ip, temp);
                atomicAdd(vec_d + ip, temp);
                temp = -p * vec_rhs[i];
                //atomicDoubleAdd(vec_rhs + ip, temp);
                atomicAdd(vec_rhs + ip, temp);

                i = prev_node[i];
                //i -= stride[iwarp * (norder + 1) + iorder - 1];
            }
            //__syncthreads();
        }

        //icell = map_t2c[tid];
        //if (icell > -1)
        //    vec_rhs[icell] /= vec_d[icell];
        ncell_per_warp = 32 / (nthread / ncell);
        i = firstnode[tid];
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
                
                //reg_rhs = vec_rhs[i] - vec_b[i] * vec_rhs[ip];
                //vec_rhs[i] = reg_rhs / vec_d[i];
                vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
                vec_rhs[i] /= vec_d[i];
                i = next_node[i];    
                //i += stride[iwarp * (norder + 1) + iorder];
            }
            //__syncthreads();
            
        }
        //if (tid == 0)
        //    printf("%f %f\n", vec_d[0], vec_rhs[0]);
        tid += thread_stride;
    }
}


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
                  cudaStream_t stream)
{
    unsigned int threads_per_block = 128;
    unsigned int block_num = (nthread + threads_per_block - 1) / threads_per_block;
    double *a_host = NULL,*b_host = NULL, *d_host = NULL, *rhs_host = NULL;
    int *parent_host = NULL, *firstnode_host = NULL, *lastnode_host = NULL, *prev_node_host = NULL, *next_node_host = NULL, *branchsize_host = NULL;
    int *stride_host = NULL;
    int *max_order_host = NULL, *min_order_host = NULL;
    bool *reg_avai_host = NULL;
    int nwarps = 0;
    
    /*if (t - 50 > 0 && t - 50 < 0.025)
    {
        a_host = (double*)malloc(sizeof(double) * len);
        b_host = (double*)malloc(sizeof(double) * len);
        d_host = (double*)malloc(sizeof(double) * len);
        rhs_host = (double*)malloc(sizeof(double) * len);
        //branchsize_host = (int*)malloc(sizeof(int) * nthread);
        parent_host = (int*)malloc(sizeof(int) * len);
        prev_node_host = (int*)malloc(sizeof(int) * len);
        next_node_host = (int*)malloc(sizeof(int) * len);
        max_order_host = (int*)malloc(sizeof(int) * nthread);
        min_order_host = (int*)malloc(sizeof(int) * nthread);
        firstnode_host = (int*)malloc(sizeof(int) * nthread);
        lastnode_host = (int*)malloc(sizeof(int) * nthread);
        //reg_avai_host = (bool*)malloc(sizeof(bool) * len);

        cudaMemcpy(a_host, vec_a, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(b_host, vec_b, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(rhs_host, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        //cudaMemcpy(branchsize_host, branchsize, sizeof(int) * nthread, cudaMemcpyDeviceToHost);
        cudaMemcpy(parent_host, parent_index, sizeof(int) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(prev_node_host, prev_node, sizeof(int) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(next_node_host, next_node, sizeof(int) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(max_order_host, max_order_per_thread, sizeof(int) * nthread, cudaMemcpyDeviceToHost);
        cudaMemcpy(min_order_host, min_order_per_thread, sizeof(int) * nthread, cudaMemcpyDeviceToHost);
        cudaMemcpy(firstnode_host, firstnode, sizeof(int) * nthread, cudaMemcpyDeviceToHost);
        cudaMemcpy(lastnode_host, lastnode, sizeof(int) * nthread, cudaMemcpyDeviceToHost);
        //cudaMemcpy(reg_avai_host, reg_avai, sizeof(bool) * nthread * (norder + 1), cudaMemcpyDeviceToHost);

        FILE *fp;
        fp = fopen("cell_data", "wb");
        fwrite(a_host, sizeof(double), len, fp);
        fwrite(b_host, sizeof(double), len, fp);
        fwrite(d_host, sizeof(double), len, fp);
        fwrite(rhs_host, sizeof(double), len, fp);
        fwrite(parent_host, sizeof(int), len, fp);
        fwrite(prev_node_host, sizeof(int), len, fp);
        fwrite(next_node_host, sizeof(int), len, fp);
        fwrite(max_order_host, sizeof(int), nthread, fp);
        fwrite(min_order_host, sizeof(int), nthread, fp);
        fwrite(firstnode_host, sizeof(int), nthread, fp);
        fwrite(lastnode_host, sizeof(int), nthread, fp);
        //fwrite(branchsize_host, sizeof(int), nthread, fp);
        fclose(fp);

        //fp = fopen("reg_avai", "wb");
        //fwrite(reg_avai_host, sizeof(bool), nthread * (norder + 1), fp);
        //fclose(fp);

        fp = fopen("cell_profile", "w");
        fprintf(fp, "%d\n%d\n%d\n%d\n", ncell, len, nthread, norder);
        fclose(fp);

        if (nthread % 32 == 0)
            nwarps = nthread / 32;
        else 
            nwarps = nthread / 32 + 1;
        stride_host = (int*)malloc(sizeof(int) * nwarps * (norder + 1));
        cudaMemcpy(stride_host, stride, sizeof(int) * nwarps * (norder + 1), cudaMemcpyDeviceToHost);
        fp = fopen("stride_array", "wb");
        fwrite(stride_host, sizeof(int), nwarps * (norder + 1), fp);
        fclose(fp);
    }*/
    
    //cudaDeviceSynchronize();
    kernel16<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, prev_node, next_node, max_order_per_thread, min_order_per_thread, firstnode, lastnode, stride, map_t2c, norder, ncell, nthread, t);
    //kernel16_tp<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, prev_node, next_node, max_order_per_thread, min_order_per_thread, firstnode, lastnode, stride, map_t2c, norder, ncell, nthread, t);
    
    cudaDeviceSynchronize();
    //FILE *fp = NULL;
    //fp = fopen("solve3", "ab");
    //d_host = (double*)malloc(sizeof(double) * len);
    //rhs_host = (double*)malloc(sizeof(double) * len);
    //cudaMemcpy(d_host, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
    //cudaMemcpy(rhs_host, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
    //fwrite(d_host, sizeof(double), len, fp);
    //fwrite(rhs_host, sizeof(double), len, fp);
    //fclose(fp); 

    
    /*if (t - 50 > 0 && t - 50 < 0.025)
    {
        FILE* result_out;
        cudaMemcpy(rhs_host, vec_rhs, sizeof(double) * len, cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host, vec_d, sizeof(double) * len, cudaMemcpyDeviceToHost);
        result_out=fopen("result_out.txt","w");
        for(int i = 0; i < len; i++)
            fprintf(result_out, "%f ", rhs_host[i]);
        fprintf(result_out, "\n");
        for(int i = 0; i < len; i++)
            fprintf(result_out, "%f ", d_host[i]);
    
        if (result_out)
            fclose(result_out);
        if (a_host)
            free(a_host);
        if (b_host)
            free(b_host);
        if (d_host)
            free(d_host);
        if (rhs_host)
            free(rhs_host);
        if (parent_host)
            free(parent_host);
        if (prev_node_host)
            free(prev_node_host);
        if (next_node_host)
            free(next_node_host);
        if (firstnode_host)
            free(firstnode_host);
        if (lastnode_host)
            free(lastnode_host);
        if (branchsize_host)
            free(branchsize_host);

    }*/
}


void cusolve_interleaved1(double* vec_a, double* vec_b, double* vec_d, double* vec_rhs, int* parent_index,
                        int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell,
                        int len,double t, cudaStream_t stream)
{
    unsigned int threads_per_block=128;
    unsigned int block_num=(ncell+threads_per_block-1)/threads_per_block;

    double* a_host,*b_host,*d_host,*rhs_host;
    int* parent_host, *stride_host,*firstnode_host,*lastnode_host,*cellsize_host;
    FILE* fp,*result_out;
    int i;
    
    if(t-50>0 && t-50<0.025)
    {
        a_host=(double*)malloc(sizeof(double)*len);    
        b_host=(double*)malloc(sizeof(double)*len);
        d_host=(double*)malloc(sizeof(double)*len);
        rhs_host=(double*)malloc(sizeof(double)*len);
        parent_host=(int*)malloc(sizeof(int)*len);
        stride_host=(int*)malloc(sizeof(int)*(nstride+1));
        firstnode_host=(int*)malloc(sizeof(int)*ncell);
        lastnode_host=(int*)malloc(sizeof(int)*ncell);
        cellsize_host=(int*)malloc(sizeof(int)*ncell);

        cudaMemcpy(a_host,vec_a,sizeof(double)*len,cudaMemcpyDeviceToHost);
        cudaMemcpy(b_host,vec_b,sizeof(double)*len,cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host,vec_d,sizeof(double)*len,cudaMemcpyDeviceToHost);
        cudaMemcpy(rhs_host,vec_rhs,sizeof(double)*len,cudaMemcpyDeviceToHost);
        cudaMemcpy(parent_host,parent_index,sizeof(int)*len,cudaMemcpyDeviceToHost);
        cudaMemcpy(stride_host,stride,sizeof(int)*(nstride+1),cudaMemcpyDeviceToHost);
        cudaMemcpy(firstnode_host,firstnode,sizeof(int)*ncell,cudaMemcpyDeviceToHost);
        cudaMemcpy(lastnode_host,lastnode,sizeof(int)*ncell,cudaMemcpyDeviceToHost);
        cudaMemcpy(cellsize_host,cellsize,sizeof(int)*ncell,cudaMemcpyDeviceToHost);
    
        fp=fopen("cell_data","wb");
        fwrite(a_host,sizeof(double),len,fp);
        fwrite(b_host,sizeof(double),len,fp);
        fwrite(d_host,sizeof(double),len,fp);
        fwrite(rhs_host,sizeof(double),len,fp);
        fwrite(parent_host,sizeof(int),len,fp);
        fwrite(stride_host,sizeof(int),nstride+1,fp);
        fwrite(firstnode_host,sizeof(int),ncell,fp);
        fwrite(lastnode_host,sizeof(int),ncell,fp);
        fwrite(cellsize_host,sizeof(int),ncell,fp);
        
        printf("*****%d\n\n\n*****",len);
        printf("*****%d\n\n\n*****",ncell);
        printf("*****%d\n\n\n*****",nstride);
        fclose(fp);

        fp = fopen("cell_profile", "w");
        fprintf(fp, "%d\n%d\n%d\n", ncell, len, nstride);
        fclose(fp);
    }

    cudaDeviceSynchronize();
    triang_kernel<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, nstride, stride, firstnode, lastnode, cellsize, ncell);
    bksub_kernel<<<block_num, threads_per_block, 0, stream>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, nstride, stride, firstnode, lastnode, cellsize, ncell);
    cudaDeviceSynchronize();

    if(t-50>0&&t-50<0.025)
    {
        cudaMemcpy(rhs_host,vec_rhs,sizeof(double)*len,cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host,vec_d,sizeof(double)*len,cudaMemcpyDeviceToHost);
        result_out=fopen("result_out.txt","w");
        for(i=0;i<len;i++)
            fprintf(result_out,"%f ",rhs_host[i]);
        fprintf(result_out,"\n");
        for(i=0;i<len;i++)
            fprintf(result_out,"%f ",d_host[i]);
    
        fclose(result_out);
        free(a_host);
        free(b_host);
        free(d_host);
        free(rhs_host);
        free(parent_host);
        free(stride_host);
        free(firstnode_host);
        free(lastnode_host);
        free(cellsize_host);

    }
}














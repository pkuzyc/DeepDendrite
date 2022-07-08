#include <cusolverSp.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <device_launch_parameters.h>
#include "solver_kernels.cuh"

using namespace std;

template <typename T> void node_permute(T *arr, int len, int* order)
{
    T *arr_tmp = new T[len];
    for (int i = 0; i < len; i++)
    {
        arr_tmp[i] = arr[i];
    }

    for (int i = 0; i < len; i++)
    {
        arr[order[i]] = arr_tmp[i];
    }
    delete[] arr_tmp;
}

void index_permute(int *arr, int len, int *order)
{
    int *arr_tmp = new int[len];
    for (int i = 0; i < len; i++)
    {
        arr_tmp[i] = arr[i];
    }

    for (int i = 0; i < len; i++)
    {
        arr[order[i]] = order[arr_tmp[i]];
    }
    delete[] arr_tmp;
}

int* inverse_permute(int *order, int len)
{
    int *order_inv = new int[len];
    for (int i = 0; i < len; i++)
    {
        order_inv[order[i]] = i;
    }
    return order_inv;
}

void invert_permute(int *order, int len)
{
    int *order_inv = inverse_permute(order, len);
    for (int i = 0; i < len; i++)
    {
        order[i] = order_inv[i];
    }
    delete[] order_inv;
}

int main(int argc,char* argv[])
{
	//int ncell = 131072;
	//int len = 4333072;
	//int nstride = 42;
    FILE *profile;
    char profile_name[300], cell_data_filename[300], result_filename[300];
    sprintf(profile_name, "%s/%s", argv[1], "cell_profile");
    sprintf(cell_data_filename, "%s/%s", argv[1], "cell_data");
    sprintf(result_filename, "%s/%s", argv[1], "result_out.txt");
    profile = fopen(profile_name,"r");
	int ncell;
    int len, threads_num, norder;
    int nstride;
    int solver_type;
    int device_id;
    solver_type = atoi(argv[argc - 1]);
    device_id = atoi(argv[argc - 2]);
    cudaSetDevice(device_id);
    /*if (argc == 5)
        solver_type = atoi(argv[4]);
    if (argc == 6)
        solver_type = atoi(argv[5]);*/

    fscanf(profile,"%d",&ncell);
    fscanf(profile,"%d",&len);
    
    if (solver_type == 0)
    {
        fscanf(profile,"%d",&nstride);
    }

    else if (solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        fscanf(profile, "%d%d", &threads_num, &norder);
    }
    fclose(profile);
    unsigned int threads_per_block = 128;
	unsigned int block_num = (ncell + threads_per_block - 1) / threads_per_block;

	double *a_host, *b_host, *d_host, *rhs_host;
	int *parent_host, *stride_host, *firstnode_host, *lastnode_host, *cellsize_host;
    int *min_level_host, *max_level_host, *prev_host, *next_host;

	double* vec_a, *vec_b, *vec_d, *vec_rhs;
	int* parent_index, *stride, *firstnode, *lastnode, *cellsize;
    int *min_level_per_thread, *max_level_per_thread, *prev_node, *next_node;
	float *rhs_gt, *d_gt;

	FILE* fp;
	fp = fopen(cell_data_filename, "rb");

	cudaMalloc((void**)&vec_a, sizeof(double)*len);
	cudaMalloc((void**)&vec_b, sizeof(double)*len);
	cudaMalloc((void**)&vec_d, sizeof(double)*len);
	cudaMalloc((void**)&vec_rhs, sizeof(double)*len);
	cudaMalloc((void**)&parent_index, sizeof(int)*len);

	if (solver_type == 0) 
    {
        cudaMalloc((void**)&stride, sizeof(int)*(nstride+1));
	    cudaMalloc((void**)&firstnode, sizeof(int)*ncell);
	    cudaMalloc((void**)&lastnode, sizeof(int)*ncell);
	    cudaMalloc((void**)&cellsize, sizeof(int)*ncell);
    }
    else if ( solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        cudaMalloc((void**)&cellsize, sizeof(int) * threads_num);
        cudaMalloc((void**)&firstnode, sizeof(int) * threads_num);
        cudaMalloc((void**)&lastnode, sizeof(int) * threads_num);
        cudaMalloc((void**)&max_level_per_thread, sizeof(int) * threads_num);
        cudaMalloc((void**)&min_level_per_thread, sizeof(int) * threads_num);
        cudaMalloc((void**)&prev_node, sizeof(int) * len);
        cudaMalloc((void**)&next_node, sizeof(int) * len);

    }
    

	a_host = (double*)malloc(sizeof(double)*len);
	b_host = (double*)malloc(sizeof(double)*len);
	d_host = (double*)malloc(sizeof(double)*len);
	rhs_host = (double*)malloc(sizeof(double)*len);
	parent_host = (int*)malloc(sizeof(int)*len);
    if (solver_type == 0)	
    {
        stride_host = (int*)malloc(sizeof(int)*(nstride+1));
        firstnode_host = (int*)malloc(sizeof(int)*ncell);
        lastnode_host = (int*)malloc(sizeof(int)*ncell);
        cellsize_host = (int*)malloc(sizeof(int)*ncell);
    }
    else if (solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        cellsize_host = (int*)malloc(sizeof(int) * threads_num);
        next_host = (int*)malloc(sizeof(int) * len);
        prev_host = (int*)malloc(sizeof(int) * len);
        firstnode_host = (int*)malloc(sizeof(int) * threads_num);
        lastnode_host = (int*)malloc(sizeof(int) * threads_num);
        max_level_host = (int*)malloc(sizeof(int) * threads_num);
        min_level_host = (int*)malloc(sizeof(int) * threads_num);
    }
    

	fread(a_host, sizeof(double), len, fp);
	fread(b_host, sizeof(double), len, fp);
	fread(d_host, sizeof(double), len, fp);
	fread(rhs_host, sizeof(double), len, fp);
	fread(parent_host, sizeof(int), len, fp);
    if (solver_type == 0) 
    {
        fread(stride_host, sizeof(int), nstride+1, fp);
        fread(firstnode_host, sizeof(int), ncell, fp);
        fread(lastnode_host, sizeof(int), ncell, fp);
        fread(cellsize_host, sizeof(int), ncell, fp);
    }
    else if (solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        fread(prev_host, sizeof(int), len, fp);
        fread(next_host, sizeof(int), len, fp);
        fread(max_level_host, sizeof(int), threads_num, fp);
        fread(min_level_host, sizeof(int), threads_num, fp);
        fread(firstnode_host, sizeof(int), threads_num, fp);
        fread(lastnode_host, sizeof(int), threads_num, fp);
        
    }
	fclose(fp);
	
    // We use kernel16 to test the run time without 
    // GPU memory optimization. The corresponding
    // data read from the files are in optimized
    // order. So if we want to test the run time 
    // without GPU memory optimization, we should
    // first transform the data from optimized order
    // to the original order.
    int *order3 = NULL;
    if (solver_type == 16)
    {
        char order_filename[300];
        sprintf(order_filename, "%s/%s", argv[1], "order3");
        printf("%s\n", order_filename);
        fp = fopen(order_filename, "rb");
        order3 = new int[len];
        fread(order3, sizeof(int), len, fp);
        fclose(fp);

        //permute all arrays from optimized order to original order
        invert_permute(order3, len); // now order3 map from order3 to original
        node_permute(a_host, len, order3);
        node_permute(b_host, len, order3);
        node_permute(d_host, len, order3);
        node_permute(rhs_host, len, order3);
        index_permute(parent_host, len, order3);
        index_permute(prev_host, len, order3);
        index_permute(next_host, len, order3);

        for (int i = 0; i < threads_num; i++)
        {
            firstnode_host[i] = order3[firstnode_host[i]];
            lastnode_host[i] = order3[lastnode_host[i]];
        }
    }

	cudaMemcpy(vec_a, a_host, sizeof(double)*len, cudaMemcpyHostToDevice);
	cudaMemcpy(vec_b, b_host, sizeof(double)*len, cudaMemcpyHostToDevice);
	cudaMemcpy(vec_d, d_host, sizeof(double)*len, cudaMemcpyHostToDevice);
	cudaMemcpy(vec_rhs, rhs_host, sizeof(double)*len, cudaMemcpyHostToDevice);
	cudaMemcpy(parent_index, parent_host, sizeof(int)*len, cudaMemcpyHostToDevice);

    if (solver_type == 0)
    {
        cudaMemcpy(stride, stride_host, sizeof(int)*(nstride+1), cudaMemcpyHostToDevice);
        cudaMemcpy(firstnode, firstnode_host, sizeof(int)*ncell, cudaMemcpyHostToDevice);
        cudaMemcpy(lastnode, lastnode_host, sizeof(int)*ncell, cudaMemcpyHostToDevice);
        cudaMemcpy(cellsize, cellsize_host, sizeof(int)*ncell, cudaMemcpyHostToDevice);
    }
    
    else if (solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        cudaMemcpy(prev_node, prev_host, sizeof(int) * len, cudaMemcpyHostToDevice);
        cudaMemcpy(next_node, next_host, sizeof(int) * len, cudaMemcpyHostToDevice);
        cudaMemcpy(firstnode, firstnode_host, sizeof(int) * threads_num, cudaMemcpyHostToDevice);
        cudaMemcpy(lastnode, lastnode_host, sizeof(int) * threads_num, cudaMemcpyHostToDevice);
        cudaMemcpy(max_level_per_thread, max_level_host, sizeof(int) * threads_num, cudaMemcpyHostToDevice);
        cudaMemcpy(min_level_per_thread, min_level_host, sizeof(int) * threads_num, cudaMemcpyHostToDevice);
    }
    
	//cudaMemcpy(children_node, children_node_host, sizeof(int)*ncell, cudaMemcpyHostToDevice);
    //cudaMemcpy(map_a, map_a_host, sizeof(int)*len, cudaMemcpyHostToDevice);
	//cudaMemcpy(map_b, map_b_host, sizeof(int)*len, cudaMemcpyHostToDevice);

    printf("ncell:%d len:%d\n", ncell, len);
  if (solver_type == 0)
  {
	  solve_kernel0 <<< block_num, threads_per_block >>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, nstride, stride, firstnode, lastnode, cellsize, ncell);
      //get_single_cell(parent_host, ncell, len, cellsize_host, stride_host, nstride, firstnode_host);
  }
  
    else if (solver_type == 16)
    {
        threads_per_block = 128;
        int branch_per_cell = threads_num / ncell;
        int ncell_per_warp = 32 / branch_per_cell;
        block_num = (threads_num + threads_per_block - 1) / threads_per_block;
        kernel16<<<block_num, threads_per_block>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, 
                prev_node, next_node, max_level_per_thread, min_level_per_thread, firstnode, lastnode, norder, ncell, threads_num, 0);

    }
    else if (solver_type == 17 || solver_type == 18)
    {
        int *stride_host = NULL, *stride = NULL;
        int nwarp;
        if (threads_num % 32 == 0)
            nwarp = threads_num / 32;
        else
            nwarp = threads_num / 32 + 1;

        stride_host = (int*)malloc(sizeof(int) * nwarp * (norder + 1));
        char stride_filename[300];
        sprintf(stride_filename, "%s/%s", argv[1], "stride_array");
        FILE *stride_file = fopen(stride_filename, "rb");
        fread(stride_host, sizeof(int), nwarp * (norder + 1), stride_file);
        fclose(stride_file);

        cudaMalloc((void**)&stride, sizeof(int) * nwarp * (norder + 1));
        cudaMemcpy(stride, stride_host, sizeof(int) * nwarp * (norder + 1), cudaMemcpyHostToDevice);

        threads_per_block = 32;
        int branch_per_cell = threads_num / ncell;
        int ncell_per_warp = 32 / branch_per_cell;
        block_num = (threads_num + threads_per_block - 1) / threads_per_block;

        //printf("shared memeory:%d B\n", sizeof(int) * threads_per_block / 32 * (norder + 1));
        //kernel17<<<block_num, threads_per_block, sizeof(int) * threads_per_block / 32 * (norder + 1)>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, 
        //        stride, max_level_per_thread, min_level_per_thread, firstnode, lastnode, norder, ncell, threads_num, 0);
        if (solver_type == 17)
        {
            cudaFuncSetAttribute(kernel17, cudaFuncAttributePreferredSharedMemoryCarveout, cudaSharedmemCarveoutMaxL1);
            kernel17<<<block_num, threads_per_block>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, stride, max_level_per_thread, min_level_per_thread, firstnode, lastnode, norder, ncell, threads_num);
        }

        if (solver_type == 18)
        {
            cudaError_t err = cudaGetLastError();
            if (err != cudaSuccess)
                printf("%s\n", cudaGetErrorString(err));
            else
                printf("success\n");
            //cudaFuncSetAttribute(kernel18, cudaFuncAttributePreferredSharedMemoryCarveout, cudaSharedmemCarveoutMaxL1);
            cudaDeviceSynchronize();
            printf("block_num:%d %d nthread:%d\n", block_num, block_num / 2, threads_num);
            kernel18<<<block_num / 2, threads_per_block>>>(vec_a, vec_b, vec_d, vec_rhs, parent_index, stride, max_level_per_thread, min_level_per_thread, firstnode, lastnode, norder, ncell, threads_num);
            err = cudaGetLastError();
            if (err != cudaSuccess)
                printf("%s\n", cudaGetErrorString(err));
            else
                printf("success\n");
        }

        cudaFree(stride);
        free(stride_host);
    }
	//cudaMemcpy(count_host,condition_count,sizeof(int)*2,cudaMemcpyDeviceToHost);
    //printf("%d %d\n",count_host[0],count_host[1]);

    if (solver_type != 12)
    {
        cudaMemcpy(rhs_host, vec_rhs, sizeof(double)*len, cudaMemcpyDeviceToHost);
        cudaMemcpy(d_host, vec_d, sizeof(double)*len, cudaMemcpyDeviceToHost);
    }
	d_gt = (float*)malloc(sizeof(float)*len);
	rhs_gt = (float*)malloc(sizeof(float)*len);
	double err = 0;
	FILE* result_gt;
	result_gt = fopen(result_filename, "r");
	for (int i = 0; i < len; i++)
	{
		fscanf(result_gt, "%f", &rhs_gt[i]);
	}
	for (int i = 0; i < len; i++)
	{
		fscanf(result_gt, "%f", &d_gt[i]);
	}
    if (solver_type == 16 && order3)
    {
        node_permute(rhs_gt, len, order3);
        node_permute(d_gt, len, order3);
    }
	for (int i = 0; i < len; i++)
	{
        if (rhs_host[i]!=rhs_host[i])
        {
            printf("compute error:%d\n", i);
            break;
        }
        if (rhs_gt[i]!=rhs_gt[i])
            printf("gt error:%d\n", i);
		err += (rhs_gt[i] - rhs_host[i])*(rhs_gt[i] - rhs_host[i]);
	}
	printf("error: %f\n", err);
    fclose(result_gt);
	free(a_host);
    free(d_gt);
    free(rhs_gt);
	free(b_host);
	free(d_host);
	free(rhs_host);
	free(parent_host);
	free(firstnode_host);
	free(lastnode_host);
    if (order3)
    {
        delete[] order3;
        order3 = NULL;
    }
    if (solver_type == 0)
    {
	    free(cellsize_host);
	    free(stride_host);
    }
    else if (solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        free(next_host);
        free(prev_host);
        free(max_level_host);
        free(min_level_host);
    }
	
	cudaFree(vec_a);
	cudaFree(vec_b);
	cudaFree(vec_d);
	cudaFree(vec_rhs);
	cudaFree(parent_index);
	cudaFree(firstnode);
	cudaFree(lastnode);
    
    if (solver_type == 0)
    {
        cudaFree(stride);
	    cudaFree(cellsize);
    }
    else if (solver_type == 17 || solver_type == 16 || solver_type == 18)
    {
        //cudaFree(next_node);
        //cudaFree(prev_node);
        cudaFree(max_level_per_thread);
        cudaFree(min_level_per_thread);
    }

	return 0;
}












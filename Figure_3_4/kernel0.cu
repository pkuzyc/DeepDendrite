#include "solver_kernels.cuh"

__global__ void solve_kernel0(double* vec_a,
	double* vec_b,
	double* vec_d,
	double* vec_rhs,
	int* parent_index,
	int nstride, int* stride, int* firstnode, int* lastnode, int* cellsize, int ncell)
{
	unsigned int tid;
	unsigned int thread_stride;
	int i, istride, ip, icell, icellsize;
	double p;
	thread_stride = blockDim.x * gridDim.x;
	tid = blockIdx.x * blockDim.x + threadIdx.x;
	while (tid<ncell)
	{
		//triang_kernel(vec_a, vec_b, vec_d, vec_rhs, parent_index, tid, cellsize[tid], nstride, s_stride, lastnode, condition_count);
		//triang_kernel2(vec_a, vec_b, vec_d, vec_rhs, parent_index, tid, cellsize[tid], nstride, stride, lastnode, condition_count);
		//bksub_kernel(vec_a, vec_b, vec_d, vec_rhs, parent_index, tid, cellsize[tid], nstride, s_stride, firstnode);
		//elimination(vec_a, vec_b, vec_d, vec_rhs, parent_index, tid, cellsize[tid], nstride, stride, firstnode, lastnode);
		//elimination2(vec_a, vec_b, vec_d, vec_rhs, parent_index, cellsize[tid], tid, node_start_index, children_node, nstride, firstnode,lastnode, stride);

		//triang
		icell = tid;
		icellsize = cellsize[icell];
		i = lastnode[icell];
		for (istride = nstride - 1; istride >= 0; --istride)
		{
			if (istride < icellsize)
			{
				ip = parent_index[i];
				p = vec_a[i] / vec_d[i];
				vec_d[ip] -= p * vec_b[i];
				vec_rhs[ip] -= p * vec_rhs[i];
				i -= stride[istride];
			}
		}

		//bksub
		i = firstnode[icell];
		vec_rhs[icell] /= vec_d[icell];
		for (istride = 0; istride < icellsize; ++istride)
		{
			ip = parent_index[i];
			vec_rhs[i] -= vec_b[i] * vec_rhs[ip];
			vec_rhs[i] /= vec_d[i];
			i += stride[istride + 1];
		}

		tid += thread_stride;
	}
}

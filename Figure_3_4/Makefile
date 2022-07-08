kernel: main.o kernel0.o kernel16.o kernel17.o kernel18.o
	nvcc -O2 -Xptxas -fscm=ca -o kernel main.o -arch=compute_80 -code=sm_80,compute_80 kernel0.o kernel16.o kernel17.o kernel18.o

kernel0.o: kernel0.cu solver_kernels.cuh	
	nvcc -O2 -arch=compute_80 -code=sm_80,compute_80 -g -c kernel0.cu

kernel16.o: kernel16.cu solver_kernels.cuh
	nvcc -O2 -arch=compute_80 -code=sm_80,compute_80 -g -c kernel16.cu

kernel17.o: kernel17.cu solver_kernels.cuh
	nvcc -O2 -o kernel17.o -arch=compute_80 -code=sm_80,compute_80 -Xptxas -dlcm=ca -g -c kernel17.cu

kernel18.o: kernel18.cu solver_kernels.cuh
	nvcc -O2 -o kernel18.o -arch=compute_80 -code=sm_80,compute_80 -Xptxas -dlcm=ca -g -c kernel18.cu

main.o: main.cu solver_kernels.cuh
	nvcc -O2 -arch=compute_80 -code=sm_80,compute_80 -g -c main.cu

clean:
	rm -f kernel *.o

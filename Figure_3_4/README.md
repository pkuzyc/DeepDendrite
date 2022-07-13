Code for the results of Figure3 and Figure4.

Dependency (our experimental setup):

NVIDIA Tesla A100 GPU 
CUDA >= 11.4
Nsight System 2021.3.2.4

This folder contains the source code for solving linear equations with our DHS method and the CoreNEURON method.

Folder "ca1": experimental data of the CA1 model. 500cell_cp1: 500 cells' data for CoreNEURON method; 500cell_cp3_16: 500 cells' data for DHS-16; 1500cell_cp1: 1500 cells' data for CoreNEURON method; 1500cell_cp3_16: 1500 cells' data for DHS-16.

Folder "source_data_of_figures": the run time data for various models used in the figures.

kernel0.cu: the method used in CoreNEURON. kernel16.cu: DHS method without GPU memory boosting. kernel17.cu: DHS method with GPU memory boosting.

Compile command: make

After compiling, a executable program "kernel" will be generated. The program "kernel" is used to run the solving equations phase with different methods. The usage of the program:

	./kernel /path/to/data gpu_id solver_type
	/path/to/data: the path to the model data 
	gpu_id: the ID of the GPU to use
	solver_type: the method used for solving equations. 0: use CoreNEURON method; 16: use DHS method without GPU memory boosting; 17: use DHS method with GPU memory boosting.
	

Evalute the solving equations times on 500 CA1 neurons:

(1) For the CoreNEURON method: nsys profile --force-overwrite=true -o nsys_report --stats=true ./kernel ./ca1/500cell_cp1 0 0

(2) For DHS-16 without memory boosting: nsys profile --force-overwrite=true -o nsys_report --stats=true ./kernel ./ca1/500cell_cp3_16 0 16

(3) For DHS-16 with memory boosting: nsys profile --force-overwrite=true -o nsys_report --stats=true ./kernel ./ca1/500cell_cp3_16 0 17


"test_with_sys.py": Used to evaluate the solving equations time for various models with different cell numbers. For each model it will test the run time three times and use the average as the final run time.
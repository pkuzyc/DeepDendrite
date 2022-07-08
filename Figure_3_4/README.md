Code for the results of Figure3 and Figure4.

Dependency:

NVIDIA Tesla A100 GPU
CUDA >= 11.0
Nsight System

This folder contains the source code for solving linear equations with our DHS method and the CoreNEURON method.

kernel0.cu: the method used in CoreNEURON. kernel16.cu: DHS method without GPU memory boosting. kernel17.cu: DHS method with GPU memory boosting.

Compile command: make

Evalute the solving equations times on 500 CA1 neurons:

(1) For the CoreNEURON method: nsys profile --force-overwrite=true -o nsys_report --stats=true ./kernel ./ca1/500cell_cp1 0 0

(2) For DHS-16 without memory boosting: nsys profile --force-overwrite=true -o nsys_report --stats=true ./kernel ./ca1/500cell_cp3_16 0 16

(3) For DHS-16 with memory boosting: nsys profile --force-overwrite=true -o nsys_report --stats=true ./kernel ./ca1/500cell_cp3_16 0 17



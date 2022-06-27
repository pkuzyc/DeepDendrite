# Soruce code of DeepDendrite and the demos in the paper.  
src: source code of DeepDendrite and the modified NEURON simulator.  
NOTICE: To use perform AI tasks with DeepDendrite, we should use our modified NEURON simulator.  
Figure5: Model used in Figure5 in the paper, i.e. spine-level human pyramidal models.  
Figure6: Model used in Figure6 in the paper, i.e. image classification on MNIST dataset.  

## 1. Compile NEURON simulator (modified from NEURON 7.6):
  Dependencies:    
  
    CMake >= 3.10.2
    MPI >= 3.0
    Python >= 3.6  

  Compile commands:  

    cd src/nrn_modify   
    ./configure --prefix ../../install --without-iv --with-paranrn --with-nrnpython=`which python`  
    make -j8  
    make install

## 2. Compile DeepDendrite:
  Dependencies (the same as CoreNEURON):  
  
    CMake >= 3.10.2
    MPI >= 3.0
    PGI OpenACC Compiler or NVIDIA HPC SDK
    CUDA Toolkit >= 10.1
  
  Compile commands: 
  
    cd src/DeepDendrite  
    modify the install.sh file to load correct modulefile of PGI or NVHPC SDK  
    source install.sh

## 3. Run the demo of spine-level model:
  Commands:
    
    cd Figure5/run_samples
    python3 run.py
  For more details please see Figure5/run_samples/README

## 4. Run the demo of image classification task:
  Commands:
    
    cd Figure6/run_samples
    python3 run.py (for training and testing)
    python3 run_test.py (only for testing, with pre-trained model weights) 
  For more details please see Figure6/run_samples/README

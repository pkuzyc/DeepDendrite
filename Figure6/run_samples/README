Code to train HPC-Net with MNIST dataset on DeepDendrite
run.py perform both training and testing. 
run_test.py perform testing with pre-trained weights that are saved after 60 epochs training.

run.py perform training and testing on HPC-Net as follow:

1. Prepare dataset for DeepDendrite. Convert the orginal image file (mnist.npz ) to binary stimuli files (the training set "stim_img_train" and testing set "stim_img_test").

2. Generate HPC-Net for training. Here we set batchsize to 16, and save corresponding model files to ./coredat/hpcnet_train_batch16.

3. Train HPC-Net with the training set ("stim_img_train" generated in step1). Here we save the synaptic weights after training 30 epochs. The weights are saved in the file "./coredat/hpcnet_train_batch16/30epochs_weights"

4. Convert the saved weights from binary file to .npy format. Save as "./coredat/hpcnet_train_batch16/30epochs_weights.npy"

5. Generate HPC-Net for testing. Save models in ./coredat/hpcnet_test_batch16.

6. Generate the file "./coredat/hpcnet_test_batch16/weights_param_val", which is used for setting synaptic weights when testing. 

7. Test HPC-Net with the test set ("stim_img_test") and the trained weights ("weights_param_val" generated in step6). Save the model outputs to "./coredat/hpcnet_test_batch16/test_oututs_soma_v"

8. Convert the output of HPCNet in testing from binary file to .npy format ("./coredat/hpcnet_test_batch16/test_outputs_soma_v.npy").

9. Calculate the classification accuracy.


run_test.py perform testing on HPC-Net as follow:
1. Generate HPC-Net for testing. Save models in ./coredat/hpcnet_test_batch16.

2. Generate the file "./coredat/hpcnet_test_batch16/weights_param_val" from the pre-trained weights file "./60epoch_weights.npy" 

3. Test HPC-Net with the test set ("stim_img_test") and the trained weights ("weights_param_val" generated in step2). Save the model outputs to "./coredat/hpcnet_test_batch16/test_oututs_soma_v"

4. Convert the output of HPCNet in testing from binary file to .npy format ("./coredat/hpcnet_test_batch16/test_outputs_soma_v.npy").

5. Calculate the classification accuracy.



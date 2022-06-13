import os
import sys

os.system("module purge")
cmd = "../../install/x86_64/bin/nrnivmodl ./mod"
if not os.path.isdir("./x86_64"):
    os.system(cmd)

# generate binary dataset file for DeepDendrite
# @param1: original dataset file (*.npz)
# @param2: fold to save binary dataset file
# stim_img_train and stim_img_test are generated
# stim_img_train: training set, stim_img_test: testing test
cmd = "python3 gen_bin_data.py ../data/img_data/mnist.npz ../data/bin_data/mnist"
os.system(cmd)

# generate HPC-Net for training
# @param1: batch_size, here we set 16
# @param2: the fold to save model
cmd = "./x86_64/special ./batch_train_netstim.py 16 ./coredat/hpcnet_train_batch16"
if not os.path.isdir("./coredat/hpcnet_train_batch16"):
    os.system(cmd)

# train HPC-Net with DeepDendrite, save the weights after training
# to ./coredat/hpcnet_train_batch16/30epochs_weights  
# here we save the weights at 5625005 ms, i.e. after 30 epochs training
# (60000 training samples in total, batchsize = 16, 50 ms for each sample,
# the simulation time for 30 epochs is 60000 / 16 * 50 * 30 = 5625000 ms)
#
# parameters (for more detais please use -h option to see):
# -d: the model dir to simulate
# -e: simulation time (ms)
# -dt: timestep in simlation
# --nonv-gap: copy values for specific variables according to the file "gap_params" in the model dir
# --input-file training samples, here are the "stim_img_train" file
# --need-record: record values of specific variables according to the file "param2rec" in the model dir
#   param2rec file format in the training model dir:
#   the first line: param_number record_time_number record_time1 record_time2 ...
#   each line in the following contains the variable to record
#   variable representation: 0 section_name position mech_name mech_order var_name
#
# --rec-file: the file to save recorded variables
# --training: 1: training, 0: not training
# --testing: 1: testing, 0: not testing
# --batchsize: the batchsize in training or testing, it must be the same as the model
# --cell-permute: 3 indicates use DHS in simulation
# --cell-nthread: threads number each cell
# --gpu: simulate on GPU
cmd = "../../install/bin/deepdendrite -d ./coredat/hpcnet_train_batch16 -e 10000 -dt 0.25 -v 0 --nonv-gap 1 --need-record 1 --rec-file ./coredat/hpcnet_train_batch16/30epochs_weights --input-file ../data/bin_data/mnist/stim_img_train --training 1 --testing 0 --batchsize 16 --cell-permute 3 --cell-nthread 16 --gpu" 
os.system(cmd)

# convert the saved weights from binary file to .npy format
cmd = "python3 ./read_file_by_col.py --input_file ./coredat/hpcnet_train_batch16/30epochs_weights --output_file ./coredat/hpcnet_train_batch16/30epochs_weights.npy"
os.system(cmd)


# generate HPC-Net for testing 
# @param1: batch_size, here we set 16
# @param2: the fold to save model
cmd = "./x86_64/special ./batch_test_netstim.py 16 ./coredat/hpcnet_test_batch16"
if not os.path.isdir("./coredat/hpcnet_test_batch16"):
    os.system(cmd)

# generate file for setting values
# of all weights in testing, save to
# ./coredat/hpcnet_test_batch16/weights_param_val
cmd = "python3 gen_param_val_file.py ./coredat/hpcnet_train_batch16/30epochs_weights.npy ./coredat/hpcnet_test_batch16/weights ./coredat/hpcnet_test_batch16/weights_param_val"
os.system(cmd)

# test HPC-Net with DeepDendrite save the outputs to
# ./coredat/hpcnet_test_batch16/test_outputs_soma_v
#
#  --param-val param_val_file: set values for specific variables according to param_val_file
#  format for param_val_file:
#    the first line: parameter_number
#    each line in the following contains the variable and the value to set:
#    variable representation: 0 section_name position mech_name mech_order var_name
#    value: a float number
cmd = "../../install/bin/deepdendrite -d ./coredat/hpcnet_test_batch16 -e 31260 -dt 0.25 -v 0 --nonv-gap 1 --need-record 1 --rec-file ./coredat/hpcnet_test_batch16/test_outputs_soma_v --input-file ../data/bin_data/mnist/stim_img_test --param-val ./coredat/hpcnet_test_batch16/weights_param_val --training 0 --testing 1 --batchsize 16 --cell-permute 3 --cell-nthread 16 --gpu" 
os.system(cmd)

# convert the output of HPCNet in testing from binary file to .npy format
cmd = "python3 read_file_by_col.py --input_file ./coredat/hpcnet_test_batch16/test_outputs_soma_v --output_file ./coredat/hpcnet_test_batch16/test_outputs_soma_v.npy -t"
os.system(cmd)

# calculate the classification accuracy
cmd = "python3 ./cal_core_acc.py ./coredat/hpcnet_test_batch16/test_outputs_soma_v.npy ../data/img_data/mnist_test_labels.npy"
os.system(cmd)




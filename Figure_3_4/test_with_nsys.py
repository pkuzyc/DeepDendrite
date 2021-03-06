import numpy as np
import pandas as pd
import os
import pdb

def str2int(str_time):
    res = 0
    for s in str_time:
        if s == ',':
            continue
        res = res * 10 + int(s)
    return res

def get_runtime(filename, kernel_type):
    f = open(filename, 'r')
    run_time = 0
    if kernel_type == 0:
        target = 'solve_kernel0'
    elif kernel_type == 16:
        target = 'kernel16'
    elif kernel_type == 17:
        target = 'kernel17'
    for line in f.readlines():
        if line.find(target) > -1:
            line_split = line.split()
            run_time = str2int(line_split[1])
            run_time = float(run_time) * 1.0e-6
            print (run_time)
            #if run_time.find('ms') > -1:
            #    run_time = float(run_time[:run_time.find('ms')])
            #elif run_time.find('us') > -1:
            #    run_time = float(run_time[:run_time.find('us')]) / 1000.0
            break

    f.close()
    return run_time

#model_name = [250, 500, 750, 1000, 1250, 1500]
model_name = ['ca1'] # model dir
#model_name = ['l5pc_spine']
#model_name = ['l5pc']
#model_name = ['purkinje_pub']
#cell_numbers = [500, 1000, 1500, 2000, 2500, 3000]
cell_numbers = [500, 1500] # cell numbers of the test model
#cell_numbers = [250,500,750]
#cell_numbers = [500]
#str_split_num = ['1split', '4split', '8split', '16split']
#str_split_num = ['16split_noopt', '16split_opt']
split_num = [0, 16]
str_split_num = [str(i) + 'split' for i in split_num]
#split_num = [32, 32]
#split_num = ['0split']
#kernel_names = ['kernel16','kernel17']
run_time = np.zeros((len(split_num), len(cell_numbers)))

data_dir = './'
t = 0
index = 0
test_times = 3
single_times = np.zeros(test_times)

# evaluate the run time of CoreNEURON method and DHS with memory boosting
writer = pd.ExcelWriter("run_time_with_memopt.xlsx")
for i in range(len(model_name)): 
    neuron_dir = os.path.join(data_dir, model_name[i])
    for col in range(len(cell_numbers)):
        #for row in range(len(kernel_names)):
        for row, split in enumerate(split_num):
            #pdb.set_trace()
            #if (kernel_names[row] == 'kernel0'):
            if split == 0:
                model_dir = os.path.join(neuron_dir, "%dcell_cp1"%cell_numbers[col])
                kernel_type = 0
            else:
                model_dir = os.path.join(neuron_dir, "%dcell_cp3_%d"%(cell_numbers[col], split))
                kernel_type = 17
                #if kernel_names[row] == 'kernel16':
                #    kernel_type = 16
                #elif kernel_names[row] == 'kernel17':
                #    kernel_type = 17
            #cmd = 'sudo /usr/local/cuda/bin/nvprof --log-file ./log.txt ./kernel ' + model_dir + ' 1 ' + str(kernel_type)
            for j in range(test_times):
                cmd = 'nsys profile --force-overwrite=true -o nsys_report ./kernel_noopt ' + model_dir + ' 0 ' + str(kernel_type)
                print(cmd)
                os.system(cmd)
                cmd = 'nsys stats nsys_report.qdrep > rumtime' 
                os.system(cmd)
                single_times[j] = get_runtime('./rumtime', kernel_type)
                #print model_dir, row, col

                os.system('rm -f ./runtime')
                print('\n\n')

            run_time[row, col] = np.mean(single_times) 

    df = pd.DataFrame(run_time, str_split_num, cell_numbers)

    df.to_excel(writer, model_name[i])
writer.save()

# evaluate the run time of DHS without memory boosting
writer = pd.ExcelWriter("run_time_without_memopt.xlsx")
for i in range(len(model_name)): 
    neuron_dir = os.path.join(data_dir, model_name[i])
    for col in range(len(cell_numbers)):
        #for row in range(len(kernel_names)):
        for row, split in enumerate(split_num):
            #pdb.set_trace()
            #if (kernel_names[row] == 'kernel0'):
            if split == 0:
                continue
            else:
                model_dir = os.path.join(neuron_dir, "%dcell_tp_%d"%(cell_numbers[col], split))
                kernel_type = 16
                #if kernel_names[row] == 'kernel16':
                #    kernel_type = 16
                #elif kernel_names[row] == 'kernel17':
                #    kernel_type = 17
            #cmd = 'sudo /usr/local/cuda/bin/nvprof --log-file ./log.txt ./kernel ' + model_dir + ' 1 ' + str(kernel_type)
            for j in range(test_times):
                cmd = 'nsys profile --force-overwrite=true -o nsys_report ./kernel ' + model_dir + ' 0 ' + str(kernel_type)
                print(cmd)
                os.system(cmd)
                cmd = 'nsys stats nsys_report.qdrep > rumtime' 
                os.system(cmd)
                single_times[j] = get_runtime('./rumtime', kernel_type)
                #print model_dir, row, col

                os.system('rm -f ./runtime')
                print('\n\n')

            run_time[row - 1, col] = np.mean(single_times) 

    df = pd.DataFrame(run_time, str_split_num, cell_numbers)

    df.to_excel(writer, model_name[i])
writer.save()


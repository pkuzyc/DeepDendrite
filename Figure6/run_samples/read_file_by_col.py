import os
import struct
import sys
import numpy as np
import getopt

def read_data_from_file(file_name):
    f = open(file_name, "rb")
    line = f.readline()
    line = line.split()
    nparam = int(line[0])
    rec_len = int(line[1])
    nrec = int(line[2])
    nlast = int(line[3])
    print(nparam, rec_len, nrec, nlast)

    all_data = np.zeros((nparam, rec_len * nrec + nlast), dtype = 'float32')
    for i in range(nrec + 1):
        if i == nrec:
            if nlast == 0:
                break
            data_len = nlast
        else:
            data_len = rec_len
        bin_data = f.read(nparam * data_len * 4)
        fmt = '%df'%(nparam * rec_len)
        data = struct.unpack(fmt, bin_data)
        for istep in range(data_len):
            all_data[:, istep] = data[istep * nparam : (istep + 1) * nparam]

    return all_data

def show_spike():
    ncell = int(sys.argv[-2])
    ncluster = int(sys.argv[-1])
    spike_times = []
    for i in range(ncell):
        spike_times.append([])
    spike_arr = np.loadtxt("out.dat")
    for i in range(spike_arr.shape[0]):
        spike_times[int(spike_arr[i, 1])].append(spike_arr[i, 0])

    gid = 0
    for icluster in range(ncluster):
        print("cluster:%d" %icluster)
        for icell in range(int(ncell / ncluster)):
            print("\t%d:%s"%(gid, spike_times[gid]))
            gid += 1

def format_trans(data, nout):
    nparam, rec_len = data.shape
    batchsize = int(nparam / nout)
    idx = 0
    ret = np.zeros((nout, rec_len * batchsize))
    for irec in range(rec_len):
        for ib in range(batchsize):
            ret[:, idx] = data[ib * 10 : ib * 10 + 10, irec]
            idx += 1
    return ret

if __name__ == '__main__':
    input_file = ""
    output_file = ""
    trans = False
    opts, args = getopt.getopt(sys.argv[1:], "i:o:t", ["input_file=", "output_file=", "--trans"])
    for opt, arg in opts:
        if opt in ("-t", "--trans"):
            trans = True
        elif opt in ("-i", "--input_file"):
            input_file = arg
        elif opt in ("-o", "--output_file"):
            output_file = arg
    
    print(input_file, output_file)
    rec_data = read_data_from_file(input_file)
    if trans:
        save_data = format_trans(rec_data, 10)
    else:
        save_data = rec_data
    print (rec_data.shape)
    print(rec_data.min(), rec_data.max())
    print (save_data.shape)
    print(save_data.min(), save_data.max())
    np.save(output_file, save_data)
    



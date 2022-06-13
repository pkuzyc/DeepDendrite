import numpy as np
import struct
import os
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
    nepoch = 30
    ntest = 10000
    nout = 10
    #batchsize = 4
    opts, args = getopt.getopt(sys.argv[1:], "t", ["--trans"])
    for opt, arg in opts:
        if opt in ("-t", "--trans"):
            trans = True
    all_test_res = np.zeros((nepoch, nout, ntest))
    for i in range(nepoch):
        filename = "./"%i
        test_res = read_data_from_file(filename)
        if trans == True:
            save_data = format_trans(test_res, 10)
        else:
            save_data = test_res
        n_batch_out, niter = test_res.shape
        #all_test_res[i - 30] = test_res
        all_test_res[i] = save_data
    np.save("./test_outputs_0.5cm_2gpas/output_weights_1to30epoch.npy", all_test_res)


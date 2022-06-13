import numpy as np

from cell import *

from neuron import h
from commonutils import *
import pdb
import struct
import os
import sys


h.Random().Random123_globalindex(1234)

h.load_file('stdgui.hoc')

pc = h.ParallelContext()

# set number of threads
pc.nthread(1, 0)



def save_and_update_weights(network_list, batch_size):
    sum_dw_in2hd = np.zeros(network_list[0].dw_in2hd.shape)
    sum_db_in2hd = np.zeros(network_list[0].db_in2hd.shape)
    sum_dw_h2out = np.zeros(network_list[0].dw_h2out.shape)
    sum_db_h2out = np.zeros(network_list[0].db_h2out.shape)

    for network in network_list:
        network.save_delta_w()
        sum_dw_in2hd = sum_dw_in2hd + network.dw_in2hd
        sum_db_in2hd = sum_db_in2hd + network.db_in2hd
        sum_dw_h2out = sum_dw_h2out + network.dw_h2out
        sum_db_h2out = sum_db_h2out + network.db_h2out

    new_in2hd_w = network_list[0].in2hd_w + sum_dw_in2hd / batch_size
    new_in2hd_b = network_list[0].in2hd_b + sum_db_in2hd / batch_size
    new_h2out_w = network_list[0].h2out_w + sum_dw_h2out / batch_size
    new_h2out_b = network_list[0].h2out_b + sum_db_h2out / batch_size
    for network in network_list:
        network.in2hd_w = np.copy(new_in2hd_w)
        network.in2hd_b = np.copy(new_in2hd_b)
        network.h2out_w = np.copy(new_h2out_w)
        network.h2out_b = np.copy(new_h2out_b)
        network.load_weights()

def train_batch(network_list, x_train, y_train, epoches, batch_size):
    assert (len(network_list) == batch_size)
    h.dt = 0.1
    h.tstop = 50
    h.t = 0
    for network in network_list:
        network.is_train()

    h.finitialize(0.0)
    h.fcurrent()

    idx_start = 0
    nsample = x_train.shape[0]

    syns = [network_list[3].synlist_h2out[i * 64] for i in range(4)]
    for syn in syns:
        seg = syn.get_segment()
        mech_id = network_list[3].seg2synlist[seg].index(syn)
        print ("%s %f BP_Syn %d w"%(seg.sec, seg.x, mech_id))
    for epoch in range(epoches):
        iter = 0
        idx_start = 0
        while idx_start < nsample:
            if idx_start + batch_size >= nsample:
                diff = idx_start + batch_size - nsample
                x = np.concatenate((x_train[idx_start:], x_train[:diff]), axis = 0)
                y = np.concatenate((y_train[idx_start:], y_train[:diff]), axis = 0)
            else:
                x = x_train[idx_start : idx_start + batch_size]
                y = y_train[idx_start : idx_start + batch_size]
            idx_start = idx_start + batch_size
            for i in range(batch_size):
                network_list[i].set_stim(x[i], y[i])

            h.finitialize(0.0)
            h.fcurrent()

            timeit(None, int(pc.id()))
            pc.psolve(int(h.tstop))
            timeit("epoch:%d iter:%d"%(epoch, iter), int(pc.id()))

            save_and_update_weights(network_list, batch_size)
            iter += 1


def test(network, x_test, y_test):
    h.dt = 0.1
    network.is_test()

    # procedure for doing and plotting each simulation
    def test_single(img, tgt):
        network.set_stim(img, tgt)

        h.t = 0
        h.tstop = 50

        t = h.Vector().record(h._ref_t)

        # setup recording
        v_outs = []
        for i in range(network.N_out):
            if (i % pc.nhost()) == pc.id():
                idx = i // pc.nhost()
                v_out = h.Vector()
                v_out.record(network.outputs[idx].soma(0.5)._ref_v)
                v_outs.append(v_out)

        h.finitialize(0.0)
        network.load_weights()
        h.fcurrent()

        
        pc.psolve(int(h.tstop))
        # h.cvode.active(True)

        v_outs = np.mean(np.array(v_outs)[:, 20:], axis=1)
        v_outs_gather = pc.py_gather(v_outs, 0)
        if pc.id() == 0:
            v_outs_flat = []
            for i in range(len(v_outs_gather[0])):
                for j in range(pc.nhost()):
                    try:
                        v_outs_flat.append(v_outs_gather[j][i])
                    except IndexError:
                        pass
            pred = np.argmax(v_outs_flat)
            return pred
        return -1

    # run the experiments, store the results
    i = 0
    preds = []
    for x, y in zip(x_test, y_test):
        # if pc.id() == 0:
        #     print("#%02d" %i)
        i = i+1
        # if y == 9:
        #     continue
        preds.append(test_single(x, y))

    if pc.id() == 0:
        i = 0
        for pred, tgt in zip(preds, y_test):
            if pred == tgt:
                i = i + 1
        print('acc: %f' %(i / len(y_test)))

    return preds

def normalize(x_train, x_test):
    vmin, vmax = x_train.min(), x_train.max()
    norm_train = (x_train - vmin) / (vmax - vmin)
    norm_test = (x_test - vmin) / (vmax - vmin)
    return nrom_train, norm_test

def gen_rec_weights(network_list):
    out_lines = []
    network = network_list[0]
    for syn in network.synlist_in2hd + network.synlist_in2hd_bias + network.synlist_h2out + network.synlist_h2out_bias:
        seg = syn.get_segment()
        mech_id = network.seg2synlist[seg].index(syn)
        out_lines.append("0 %s %f BP_Syn %d w\n"%(seg.sec, seg.x, mech_id))
    return out_lines
    

if __name__ == '__main__':

    with np.load('../data/img_data/mnist.npz', allow_pickle=True) as f:
        x_train, y_train = f['x_train'] / 255, f['y_train']
        x_test, y_test = f['x_test'] / 255, f['y_test']


    batch_size = int(sys.argv[-2])
    epochs = 30

    ran = h.Random(1234)
    ran.Random123(0, 1)

    rank = pc.id()
    nhost = int(pc.nhost())
    pc.barrier()
    timeit(None, rank)
    network_list = []
    gid_start = 0
    for i in range(batch_size):
        print("network%d"%i)
        network = Network_parallel(gid_start, seed=1234)
        network_list.append(network)
        gid_start = gid_start + network.N_in + network.N_hidden + network.N_out + 11 + network.N_in

    timeit("create network", rank)

    h.cvode.cache_efficient(1)


    pc.setup_transfer()

    pc.set_maxstep(10)

    h.stdinit()

    out_g = []
    out_in2hd_syn = []
    out_h2out_syn = []
    out_w = []
    out_dw = []
    out_stim = []
    out_gap = []
    for network in network_list:
        gen_files(network, out_g, out_in2hd_syn, out_h2out_syn, out_w, out_dw, out_stim)
        out_gap += network.gap_out_lines
    for network in network_list:
        network.is_train()
        network.load_weights()

    for network in network_list:
        network.in2hd_w = np.copy(network_list[0].in2hd_w)
        network.in2hd_b = np.copy(network_list[0].in2hd_b)
        network.h2out_w = np.copy(network_list[0].h2out_w)
        network.h2out_b = np.copy(network_list[0].h2out_b)
        network.load_weights()

    bbcorewrite_folder = sys.argv[-1]
    if rank == 0 and not os.path.isdir(bbcorewrite_folder):
        os.mkdir(bbcorewrite_folder)
    pc.barrier()
    with open(os.path.join(bbcorewrite_folder, "gap_params"), "w") as f:
        f.writelines(str(len(out_gap)) + "\n")
        f.writelines(out_gap)
    with open(os.path.join(bbcorewrite_folder, "in2hd_g"), "w") as f:
        f.writelines(str(len(out_g)) + "\n")
        f.writelines(out_g)
    with open(os.path.join(bbcorewrite_folder, "in2hd_syn"), "w") as f:
        f.writelines(str(len(out_in2hd_syn)) + '\n')
        f.writelines(out_in2hd_syn)
    with open(os.path.join(bbcorewrite_folder, "h2out_syn"), "w") as f:
        f.writelines(str(len(out_h2out_syn)) + '\n')
        f.writelines(out_h2out_syn)
    with open(os.path.join(bbcorewrite_folder, "weights"), "w") as f:
        f.writelines(str(len(out_w)) + '\n')
        f.writelines(out_w)
    with open(os.path.join(bbcorewrite_folder, "delta_weights"), "w") as f:
        f.writelines(str(len(out_dw)) + '\n')
        f.writelines(out_dw)
    with open(os.path.join(bbcorewrite_folder, "stim_amp"), "w") as f:
        f.writelines(str(len(out_stim)) + '\n')
        f.writelines(out_stim)

    for i in range(batch_size):
        network_list[i].set_stim(x_train[i], y_train[i])
    pc.nrnbbcore_write(bbcorewrite_folder)

    rec_weights = gen_rec_weights(network_list)
    # save weights at time 5625005 ms, when 
    # batch_size = 16, 5625005 ms indicates
    # the 30th epoch
    weights_save_time = 5625005 
    with open(os.path.join(bbcorewrite_folder, "param2rec"), "w") as f:
        f.writelines("%d %d %d\n"%(len(rec_weights), 1, weights_save_time))
        f.writelines(rec_weights)

    timeit("write data", rank)

    cmd = 'mv ./secnamelist0.txt ./map_node2sec0 ' + bbcorewrite_folder
    os.system(cmd)
    
    if pc.id() == 0:
        print("ok")

    if pc.nhost() > 1:
        pc.barrier()

    pc.done()
    h.quit()

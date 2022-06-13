########
#
# This code generates a shorter version of figure 8A in Eyal et al 2017
# It simulates synchronous activation of synapses and the probability to spike as function of the number of activated synapses.
# Two cases are shown, the case of distributed activation and the case of clustered activation
# This is a short version, because in the paper much more cases and seeds per case where tested
#  
#
# AUTHOR: Guy Eyal, the Hebrew University
# CONTACT: guy.eyal@mail.huji.ac.il
#
########
import os
import sys

#os.system('nrnivmodl ../ActiveMechanisms/')

import numpy as np
from neuron import h, gui
import matplotlib.pyplot as plt
import math
import progressbar
from commonutils import *
from cell import *

pc = h.ParallelContext()

seed = 100 
MAX_JITTER = 0 #synchronous activation
cluster_type = "sync_clusters"

NUMBER_OF_RUNS_DIST = 50
DISTRIB_SYNS_RANGE = list(range(0,101,25))+[110,120,125,130]+list(range(140,241,25))
NUMBER_OF_RUNS_CLUSTER = 50
CLUSTERS_RANGE = [0,2,4,5,6,7,8,10,12]
print(len(DISTRIB_SYNS_RANGE), len(CLUSTERS_RANGE))

COLOR_CLUSTER = np.array([239,67,182])/256.0
COLOR_DISTRIBUTED = np.array([10,153,20])/256.0

model = "cell0603_11_model_937"
morph = "2013_03_06_cell11_1125_H41_06.ASC"
spine_density = float(sys.argv[-1])
h.load_file("import3d.hoc")
h.load_file(model + ".hoc")

np.random.seed(seed)

class config_params():
    pass

config = config_params()

config.CLUSTER_TYPE = None
config.TAU_1_AMPA = 0.3
config.TAU_2_AMPA = 1.8
# properties of the model from figure 4:
config.TAU_1_NMDA = 8.019 
config.TAU_2_NMDA = 34.9884
config.N_NMDA = 0.28011
config.GAMMA_NMDA = 0.0765685
config.AMPA_W = 0.00073027
config.NMDA_W = 0.00131038
config.NMDA_W_BLOCKED = 0
config.E_SYN = 0

config.Spike_time = 131.4 + 400
config.SPINE_HEAD_X = 1
config.CLUSTER_L = 20
config.CLUSTER_SIZE = 20




h.steps_per_ms = 25
h.dt = 1.0/h.steps_per_ms
h.celsius = 37
h.v_init = -86

Stim1 = h.NetStim()
Stim1.interval=10000 
Stim1.start= config.Spike_time
Stim1.noise=0
Stim1.number=1

config.stim = Stim1

#h.tstop = config.Spike_time+150
h.tstop = 1000


#from Figure8Functions import *

rd = h.Random(seed)

spike_counts_distributed = {}
timeit(None, pc.id())
max_v_distributed = {}
print("simulating the distributed case:")
#bar = progressbar.ProgressBar(maxval=len(DISTRIB_SYNS_RANGE*NUMBER_OF_RUNS_DIST), widgets=[' [', progressbar.Timer(), '] ',
#        progressbar.Bar(), ' (', progressbar.ETA(), ') ',    ])
#bar.start()

cell_list = []
nc_list = []
stim_list = []

all_syn_list = []
all_con_list = []

all_spine_head_list = []
all_spine_neck_list = []


ncell = NUMBER_OF_RUNS_DIST * len(DISTRIB_SYNS_RANGE) + NUMBER_OF_RUNS_CLUSTER * len(CLUSTERS_RANGE)
print("ncell:%d spine_density:%f"%(ncell, spine_density))
nstim_background = 400
freq_background = 1.0
tstart_background = 10
tend_background = 1000
for jx,number_of_synapses in enumerate(DISTRIB_SYNS_RANGE):
    spike_prob_list = []
    spike_counts = []
    max_v_list = []
    for rep in range(NUMBER_OF_RUNS_DIST):
        #HCell = setup_cell()
        cell = HPC(model, morph, spine_density)
        all_spike_times = gen_spike_trains(freq_background, tstart_background, tend_background, h.dt, nstim_background)
        cell.attach_background_noise(all_spike_times, rd, config)
        print(cell.HCell)
        if spine_density <= 0:
            synaptic_locations = fill_synapses_vectors(cell.HCell,number_of_synapses,rd,config)
            add_synapses_on_list_of_segments(synaptic_locations,cell.HCell,cell.synlist,cell.conlist,config)
        else:
            cell.add_full_spine(cell.HCell, 0.25, 1.35, 2.8, cell.HCell.soma[0].Ra)
            synaptic_segments = cell.fill_synapses_vectors_with_spine(number_of_synapses, rd, config)
            cell.add_synapses_on_list_of_segments(synaptic_segments, cell.synlist, cell.conlist, config)
        #head_list, neck_list = add_full_spine(HCell, 1.0)
        #all_spine_head_list.append(head_list)
        #all_spine_neck_list.append(neck_list)
        # randomly distributes number_of_synapses on the model
        #SynList = []
        #ConList = []
        # Add human L2/L3 synapses on spine heads connected to synaptic_locations  
        #add_synapses_on_list_of_segments(synaptic_locations,cell.HCell,SynList,ConList,config)
        # To allow case where the synapses are activated a-synchronically
        configure_synaptic_delayes(MAX_JITTER,cell.conlist,rd,config,cluster_type = None)

        #all_syn_list.append(SynList)
        #all_con_list.append(ConList)

        gid = jx * NUMBER_OF_RUNS_DIST + rep
        pc.set_gid2node(gid, pc.id())
        nc = h.NetCon(cell.HCell.soma[0](0.5)._ref_v, None, sec = cell.HCell.soma[0])
        h.pop_section()
        pc.cell(gid, nc)
        cell_list.append(cell)
        nc_list.append(nc)
        # Run the simulation
        #num_of_spikes,max_v = run_exp(HCell,config)
        #spike_counts.append(num_of_spikes)
        #max_v_list.append(max_v)

        #bar.update(jx*NUMBER_OF_RUNS_DIST+rep)
        #sys.stdout.flush()

    #spike_counts_distributed[number_of_synapses] = spike_counts
    #max_v_distributed[number_of_synapses] = max_v


#pc.set_gid2node(0, pc.id())
#nc = h.NetCon(HCell.soma[0](0.5)._ref_v, None, sec = HCell.soma[0])
#nc.threshold = -10
#pc.cell(0, nc)
#bbcorewrite_folder = 'coredat'
#if not os.path.exists(bbcorewrite_folder):
#    os.makedirs(bbcorewrite_folder)
#pc.barrier()

ndistrib = NUMBER_OF_RUNS_DIST * len(DISTRIB_SYNS_RANGE)

spike_counts_clusters = {}
max_v_clusters = {}
#print("simulating the clustered case:")
#bar = progressbar.ProgressBar(maxval=len(CLUSTERS_RANGE*NUMBER_OF_RUNS_CLUSTER), widgets=[' [', progressbar.Timer(), '] ',
#        progressbar.Bar(), ' (', progressbar.ETA(), ') ',    ])
#bar.start()

for jx,number_of_clusters in enumerate(CLUSTERS_RANGE):
    spike_counts = []
    max_v_list = []
    for rep in range(NUMBER_OF_RUNS_CLUSTER):
        #HCell = setup_cell()
        cell = HPC(model, morph, spine_density)
        all_spike_times = gen_spike_trains(freq_background, tstart_background, tend_background, h.dt, nstim_background)
        cell.attach_background_noise(all_spike_times, rd, config)
        print(cell.HCell)
        if spine_density <= 0:
            synaptic_locations = fill_clustered_synapses_vectors(cell.HCell,number_of_clusters,rd,config)
            add_synapses_on_list_of_segments(synaptic_locations,cell.HCell,cell.synlist,cell.conlist,config)
        else:
            cell.add_full_spine(cell.HCell, 0.25, 1.35, 2.8, cell.HCell.soma[0].Ra)
            synaptic_segments = cell.fill_clustered_synapses_vectors_with_spine(number_of_clusters, rd, config)
            cell.add_synapses_on_list_of_segments(synaptic_segments, cell.synlist, cell.conlist, config)
        #head_list, neck_list = add_full_spine(HCell, 1.0)
        #all_spine_head_list.append(head_list)
        #all_spine_neck_list.append(neck_list)
        # randomly distributes number_of_clusteres on the model
        #SynList = []
        #ConList = []
        # Add human L2/L3 synapses on spine heads connected to synaptic_locations  
        # To allow case where the synapses are activated a-synchronically
        configure_synaptic_delayes(MAX_JITTER,cell.conlist,rd,config,cluster_type = "sync_clusters")
        
        #all_syn_list.append(SynList)
        #all_con_list.append(ConList)


        gid = ndistrib + jx * NUMBER_OF_RUNS_CLUSTER + rep
        pc.set_gid2node(gid, pc.id())
        nc = h.NetCon(cell.HCell.soma[0](0.5)._ref_v, None, sec = cell.HCell.soma[0])
        h.pop_section()
        pc.cell(gid, nc)
        cell_list.append(cell)
        nc_list.append(nc)

        # Run the simulation
        #num_of_spikes,max_v = run_exp(HCell,config)
        #spike_counts.append(num_of_spikes)
        #max_v_list.append(max_v)

        #bar.update(jx*NUMBER_OF_RUNS_CLUSTER+rep)
        #sys.stdout.flush()

    #spike_counts_clusters[number_of_clusters] = spike_counts
    #max_v_clusters[number_of_clusters] = max_v_list

pc.barrier()
timeit("create models", pc.id())

if spine_density <= 0:
    bbcorewrite_folder = './coredat/coredat_%dcell_%d_%.1fhz_back_noise_nospine'%(ncell, nstim_background, freq_background)
else:
    bbcorewrite_folder = './coredat/coredat_%dcell_%d_%.1fhz_back_noise_%.1fspine'%(ncell, nstim_background, freq_background, spine_density)
if not os.path.exists(bbcorewrite_folder):
    os.makedirs(bbcorewrite_folder)

print(bbcorewrite_folder)
h.cvode.cache_efficient(1)
pc.set_maxstep(10)
h.stdinit()
pc.nrnbbcore_write(bbcorewrite_folder)
pc.barrier()
timeit("write data", pc.id())

spike_record()


h.finitialize(h.v_init)
pc.barrier()
timeit("initialize", pc.id())

#pc.psolve(config.Spike_time+150)
#pc.psolve(1000.0)
pc.barrier()
timeit("run", pc.id())

#spikeout(bbcorewrite_folder)
h.quit()



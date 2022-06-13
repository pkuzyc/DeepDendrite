import numpy as np
import sys
import matplotlib.pyplot as plt

class config_params():
    pass

COLOR_CLUSTER = np.array([239,67,182])/256.0
COLOR_DISTRIBUTED = np.array([10,153,20])/256.0
NUMBER_OF_RUNS_DIST = 50
DISTRIB_SYNS_RANGE = list(range(0,101,25))+[110,120,125,130]+list(range(140,241,25))
NUMBER_OF_RUNS_CLUSTER = 50
CLUSTERS_RANGE = [0,2,4,5,6,7,8,10,12]
config = config_params()
config.CLUSTER_SIZE = 20

spike_counts_distributed = {}
spike_counts_clusters = {}
for nsyn in DISTRIB_SYNS_RANGE:
    spike_counts_distributed[nsyn] = [0] * NUMBER_OF_RUNS_DIST
for nclu in CLUSTERS_RANGE:
    spike_counts_clusters[nclu] = [0] * NUMBER_OF_RUNS_CLUSTER

spk_file = sys.argv[-2]
save_name = sys.argv[-1]
ndistrib = NUMBER_OF_RUNS_DIST * len(DISTRIB_SYNS_RANGE)
#spikes = np.loadtxt("/home/zyc/GPU-Neuron/install/bin/out.dat")
spikes = np.loadtxt(spk_file)
for i in range(spikes.shape[0]):
    neuronid = int(spikes[i, 1])
    if neuronid < ndistrib:
        range_id = neuronid // NUMBER_OF_RUNS_DIST
        try_id = neuronid % NUMBER_OF_RUNS_DIST
        spike_counts_distributed[DISTRIB_SYNS_RANGE[range_id]][try_id] += 1
    else:
        neuronid -= ndistrib
        range_id = neuronid // NUMBER_OF_RUNS_CLUSTER
        try_id = neuronid % NUMBER_OF_RUNS_CLUSTER
        spike_counts_clusters[CLUSTERS_RANGE[range_id]][try_id] += 1


sp_prob_dist = []
sp_prob_dist_std = []
for n_s in DISTRIB_SYNS_RANGE:
    sp_prob_arr = (np.array(spike_counts_distributed[n_s])>0).astype(int)
    sp_prob_dist.append(np.mean(sp_prob_arr))
    sp_prob_dist_std.append(np.std(sp_prob_arr))

print(sp_prob_dist)
print(sp_prob_dist_std)

sp_prob_cluster = []
sp_prob_cluster_std = []
for n_s in CLUSTERS_RANGE:
    sp_prob_arr = (np.array(spike_counts_clusters[n_s])>0).astype(int)
    sp_prob_cluster.append(np.mean(sp_prob_arr))
    sp_prob_cluster_std.append(np.std(sp_prob_arr))

print(sp_prob_cluster)
print(sp_prob_cluster_std)

#np.savez("./spk_prob.npz", dist_prob = np.array(sp_prob_dist), dist_std = np.array(sp_prob_dist_std), cluster_prob = np.array(sp_prob_cluster), cluster_std = np.array(sp_prob_cluster_std))

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax2 = ax1.twiny()

lower_bound = [max(0,m-s) for (m,s) in zip(sp_prob_cluster,sp_prob_cluster_std)] 
higher_bound = [min(1,m+s) for (m,s) in zip(sp_prob_cluster,sp_prob_cluster_std)] 
ax1.fill_between(np.array(CLUSTERS_RANGE)*config.CLUSTER_SIZE, lower_bound, higher_bound, facecolor=COLOR_CLUSTER, alpha=0.5)

lower_bound = [max(0,m-s) for (m,s) in zip(sp_prob_dist,sp_prob_dist_std)] 
higher_bound = [min(1,m+s) for (m,s) in zip(sp_prob_dist,sp_prob_dist_std)] 
ax1.fill_between(DISTRIB_SYNS_RANGE, lower_bound, higher_bound, facecolor=COLOR_DISTRIBUTED, alpha=0.5)

ax1.plot(np.array(CLUSTERS_RANGE)*config.CLUSTER_SIZE,sp_prob_cluster,c=COLOR_CLUSTER,marker = 'o',ls='-',lw = 2)
ax1.plot(DISTRIB_SYNS_RANGE,sp_prob_dist,c=COLOR_DISTRIBUTED,marker = None,ls='-',lw = 2)


ax1.set_xticks(range(0,241,20))

ax1.set_xlim(0,240)
ax1.set_ylim(0,1)


ax1.set_ylabel('spike probability')
ax1.set_xlabel('number of synapses')

ax2.set_xticks(range(0,13,1))
ax2.set_xlabel("number of clusters")

plt.savefig(save_name)

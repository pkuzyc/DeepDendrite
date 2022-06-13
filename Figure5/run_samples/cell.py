import numpy as np
from neuron import h, gui
import queue
import pdb

def random_synapse(HCell,rd,total_L, total_basal_L):
    """
    returns a random location in HCell - a neuron model
    rd -- NEURON random object
    total_L -- total dendritic length
    total_basal_L -- total basal length
    they are used here to choose a synaptic location out of the uniform distribution of dendritic locations
    that give the same probability to any point on the dendritic tree 
    note that just choosing segments randomly would ignore the segments physical length and would bias
    more synapses on shorter segments
    """

    synaptic_loc = rd.uniform(0,total_L)
    if synaptic_loc<total_basal_L:
        return basal_random_synapse(HCell,synaptic_loc)
    else:
        return apical_random_synapse(HCell,synaptic_loc-total_basal_L)


def basal_random_synapse(HCell,synaptic_loc):
    ''' returns a random location in the basal tree of this cell'''
    len0 = 0
    len1 = 0
    for sec in HCell.basal:
        len1 += sec.L
        if len1 >= synaptic_loc:
            x = (synaptic_loc-len0)/sec.L
            return sec,x
        h.pop_section()
        len0 = len1


def apical_random_synapse(HCell,synaptic_loc):
    ''' returns a random location in the apical tree of this cell'''
    len0 = 0
    len1 = 0
    for sec in HCell.apical:
        len1 += sec.L
        if len1 >= synaptic_loc:
            x = (synaptic_loc-len0)/sec.L
            return sec,x
        h.pop_section()
        len0 = len1

# Choose random number_of_synapses locations on HCell
# config is an object with the configurations required for this figure
def fill_synapses_vectors(HCell,number_of_synapses,rd,config):

    total_basal_L = sum([sec.L for sec in HCell.basal])
    total_L = sum([sec.L for sec in HCell.basal]) + sum([sec.L for sec in HCell.apical]) 
    syn_segments = []
    for i in range(number_of_synapses):
        sec,x = random_synapse(HCell,rd,total_L,total_basal_L)
        syn_segments.append(sec(x))

    return syn_segments

 

def fill_clustered_synapses_vectors(HCell,number_of_clusters,rd,config):
    '''
    Chooses random number_of_clusters locations on HCell
    The center of each cluster is chosen randomly, and the synapses within a cluster
    are distributed within config.CLUSTER_L which is 20 um in this work
    '''
    total_basal_L = sum([sec.L for sec in HCell.basal])
    total_L = sum([sec.L for sec in HCell.basal]) + sum([sec.L for sec in HCell.apical]) 

    syn_segments = []
    for i in range(number_of_clusters):
        sec,X_center = random_synapse(HCell,rd,total_L,total_basal_L)
        for i in range(config.CLUSTER_SIZE):
            if sec.L<config.CLUSTER_L:
                x = rd.uniform(0,1) 
            elif X_center<config.CLUSTER_L/sec.L:
                x = rd.uniform(0,config.CLUSTER_L/sec.L) 
            elif X_center>(1-config.CLUSTER_L/sec.L):
                x = rd.uniform(1-config.CLUSTER_L/sec.L,1)
            else: # the standard case
                x = rd.uniform(X_center-config.CLUSTER_L/2.0/sec.L,X_center+config.CLUSTER_L/2.0/sec.L)

            syn_segments.append(sec(x))

    return syn_segments

# The cluster locations shown in Figure 8C
def fill_clustered_synapses_demo(HCell,number_of_clusters,rd,config):
    segments_for_figure_7 = [(HCell.apic[38],0.85),(HCell.apic[69],0.5),(HCell.apic[90],0.7),
                            (HCell.dend[84],0.75),(HCell.dend[43],0.2),(HCell.dend[41],0.65)]
    syn_segments = []
    for i in range(number_of_clusters):
        sec,X_center = segments_for_figure_7[i]
        for i in range(config.CLUSTER_SIZE):
            if sec.L<config.CLUSTER_L:
                x = rd.uniform(0,1) 
            elif X_center<config.CLUSTER_L/sec.L:
                x = rd.uniform(0,config.CLUSTER_L/sec.L) 
            elif X_center>(1-config.CLUSTER_L/sec.L):
                x = rd.uniform(1-config.CLUSTER_L/sec.L,1)
            else: # the standard case
                x = rd.uniform(X_center-config.CLUSTER_L/2.0/sec.L,X_center+config.CLUSTER_L/2.0/sec.L)

            syn_segments.append(sec(x))

    return syn_segments


# Add spines to the input model, where the spines are connected to the segments in list_of_segments
def add_spines_on_segments(HCell,list_of_segments):

    Xs_vec = h.Vector()
    secs_sref_list = h.List()

    for seg in list_of_segments:
        sec = seg.sec
        sref = h.SectionRef(sec=sec)
        secs_sref_list.append(sref)
        Xs_vec.append(min(seg.x,1))

    HCell.add_few_spines(secs_sref_list,Xs_vec,0.25,1.35,2.8,HCell.soma[0].Ra)


# Add synapses on the top of human spine models that will be connected to the segments in list_of_segments
# The synaptic properties are defined in the config object and here are as in Figure 5.
def add_synapses_on_list_of_segments(list_of_segments,HCell,SynList,ConList,config):
    HCell.delete_spine()
    if len(list_of_segments) == 0:
        return

    add_spines_on_segments(HCell,list_of_segments)
    num_of_synapses = len(list_of_segments)
    for j in range(num_of_synapses):

            SynList.append(h.Exp2Syn(config.SPINE_HEAD_X,sec=HCell.spine[(j*2)+1]))
            ConList.append(h.NetCon(config.stim,SynList[-1]))
            SynList[-1].e = config.E_SYN
            SynList[-1].tau1 = config.TAU_1_AMPA
            SynList[-1].tau2 = config.TAU_2_AMPA
            ConList[-1].weight[0]= config.AMPA_W


    for j in range(num_of_synapses):
        
        SynList.append(h.NMDA(config.SPINE_HEAD_X,sec=HCell.spine[(j*2)+1]))
        ConList.append(h.NetCon(config.stim,SynList[-1]))
        SynList[-1].e=config.E_SYN
        SynList[-1].tau_r_NMDA = config.TAU_1_NMDA
        SynList[-1].tau_d_NMDA = config.TAU_2_NMDA
        SynList[-1].n_NMDA = config.N_NMDA
        SynList[-1].gama_NMDA = config.GAMMA_NMDA
        ConList[-1].weight[0] = config.NMDA_W




# Configure synaptic delays. 
# All figures in Eyal et al are based on sync activation, 
# But one may insert jittering as in Farinella et al., 2014
def configure_synaptic_delayes(jitter_time,ConList,rd,config,cluster_type = None):
    number_of_synapses = len(ConList)/2
    if jitter_time == 0: # no delay all the synapses are activated simultanouesly
        for i in range(int(number_of_synapses*2)):
            ConList[i].delay = jitter_time

    else:
        if cluster_type is None or cluster_type == "async_clusters" : #the synapses are activated a-synchronically
            for i in range(number_of_synapses):
                ConList[i].delay = rd.uniform(0,jitter_time)
                ConList[number_of_synapses+i].delay = ConList[i]

        if cluster_type == "sync_clusters": # synapses within a cluster are activated synchronically
            number_of_clusters = number_of_synapses/config.CLUSTER_SIZE
            for i in range(number_of_clusters):
                delay = rd.uniform(0,jitter_time)
                for j in range(config.CLUSTER_SIZE):
                    ConList[i*config.CLUSTER_SIZE+j].delay = delay
                    ConList[number_of_synapses+i*config.CLUSTER_SIZE+j].delay = delay
 
def gen_spike_trains(freq, tstart, tend, dt, num_trains):
    nbin = int((tend - tstart) / dt)
    spike_mat = np.random.rand(num_trains, nbin) < freq / 1000.0 * dt
    all_spike_times = []
    for i in range(num_trains):
        spike_index = np.where(spike_mat[i, :] == 1)[0]
        spike_time = spike_index * dt + tstart
        all_spike_times.append(spike_time)
    return all_spike_times

class HPC:
    def __init__(self, model, morph, spine_density):
        self.spine_density = spine_density
        self.HCell = self.setup_cell(model, morph)
        self.E_PAS = -86
        self.CM = 0.44
        self.RM = 48300
        self.RA = 261.97
        self.rng = np.random.default_rng(100)
        self.synlist = []
        self.conlist = []
        self.back_synlist = []
        self.head_list_by_seg = []
        self.neck_list_by_seg = []
        self.spine_num = 0
        if spine_density > 0:
            self.get_spine_seg(self.HCell)
            #self.add_full_spine(self.HCell, 0.25, 1.35, 2.8, self.HCell.soma[0].Ra)
        #"../Morphs/2013_03_06_cell11_1125_H41_06.ASC"

    def setup_cell(self, model, morph):
        h("objref HCell")
        h("HCell = new "+model+"()")
        HCell = h.HCell
        #HCell = h.cell0603_11_model_937()
        nl = h.Import3d_Neurolucida3()

        # Creating the model
        nl.quiet = 1
        nl.input(morph)
        imprt = h.Import3d_GUI(nl, 0)   
        imprt.instantiate(HCell)    
        HCell.indexSections(imprt)
        HCell.geom_nsec()   
        HCell.geom_nseg()
        HCell.delete_axon()
        HCell.insertChannel()
        if self.spine_density <= 0:
            HCell.init_biophys()
        else:
            HCell.init_rc()
        HCell.biophys()

        return HCell

    def add_single_spine(self, seg, neck_d = 0.25, neck_l = 1.35, head_area = 2.8, RA = 261.97, offset = False):
        listid, length, _ = self.map_seg2spine_listid[seg]
        sec = seg.sec
        if offset == True:
            x_interval = 1.0 / sec.nseg
            minx = seg.x - x_interval / 2.0 + 1e-6
            maxx = seg.x + x_interval / 2.0 - 1e-6
            x = self.rng.uniform(minx, maxx)
            seg2add = sec(x)
        else:
            seg2add = seg
        neck = h.Section(name = str(self.HCell) + '.spine_neck[%d]'%self.spine_num)
        neck.L = neck_l
        neck.diam = neck_d
        neck.nseg = 1
        neck.insert('pas')
        neck.g_pas = 1.0 / self.RM
        neck.e_pas = self.E_PAS
        neck.Ra = RA
        neck.cm = self.CM
        #neck.g_pas = sec.g_pas
        #neck.e_pas = sec.e_pas

        head_l = 2 * np.sqrt(head_area / 4 / np.pi)
        head_d = head_l
        head = h.Section(name = str(self.HCell) + '.spine_head[%d]'%self.spine_num)
        head.diam = head_d
        head.L = head_l
        head.nseg = 1
        head.insert('pas')
        head.g_pas = 1.0 / self.RM
        head.e_pas = self.E_PAS
        head.Ra = RA
        head.cm = self.CM
        #head.g_pas = sec.g_pas
        #head.e_pas = sec.e_pas
        
        if seg != seg2add:
            pdb.set_trace()
        head.connect(neck(1), 0)
        neck.connect(seg2add, 0)

        self.head_list_by_seg[listid].append(head)
        self.neck_list_by_seg[listid].append(neck)
        self.map_seg2spine_listid[seg][1] += 1 
        self.spine_num += 1
        #head_list.append(head)
        #neck_list.append(neck)
        return neck, head

    def get_spine_seg(self, HCell):
        step_dist = 60
        h.distance(0, 0.5, sec = HCell.soma[0])
        q = queue.Queue()
        q.put(HCell.soma[0])
        self.spine_seg_list = []
        self.map_seg2spine_listid = {} 
        #print(HCell.dend[76](0.9458), h.distance(HCell.dend[76](0.9458), sec = HCell.dend[76]))
        while not q.empty():
            section = q.get()
            sref = h.SectionRef(sec = section)
            push_sub_tree = False
            for seg in section:
                dis = h.distance(seg.x, sec = section)
                if dis > step_dist:
                    push_sub_tree = True
                    self.spine_seg_list.append(seg)
                h.pop_section()
            if push_sub_tree:
                sl = h.SectionList()
                sl.subtree(section)
                sl.remove(section)
                for sec in sl:
                    for seg in sec:
                        self.spine_seg_list.append(seg)
            else:
                for child in sref.child:
                    if str(child).find("axon") > -1:
                        continue
                    q.put(child)
            h.pop_section()

        h.pop_section()
        nspine_seg = len(self.spine_seg_list)
        #self.head_list_by_seg = [[]] * nspine_seg
        #self.neck_list_by_seg = [[]] * nspine_seg
        for i, seg in enumerate(self.spine_seg_list):
            self.map_seg2spine_listid[seg] = [i, 0, 0] #[spine_list_id, spine_list_length, start_idx]
            self.head_list_by_seg.append([])
            self.neck_list_by_seg.append([])

    def add_full_spine(self, HCell, neck_d = 0.25, neck_l = 1.35, head_area = 2.8, RA = 261.97):
        

        head_l = 2 * np.sqrt(head_area / 4 / np.pi)
        head_d = head_l
        nseg = 0
        #print(len(spine_seg_list))
        for seg in self.spine_seg_list:
            sec = seg.sec
            nspine = int(sec.L / sec.nseg * self.spine_density)
            nspine -= self.map_seg2spine_listid[seg][1]
            if nspine <= 0:
                continue
            #head_list = []
            #neck_list = []
            for i in range(nspine):
                self.add_single_spine(seg, neck_d, neck_l, head_area, RA, True)
                
        print("%d spines"%self.spine_num)

    # Add synapses on the top of human spine models that will be connected to the segments in list_of_segments
    # The synaptic properties are defined in the config object and here are as in Figure 5.
    def add_synapses_on_list_of_segments(self, list_of_segments, SynList, ConList, config):
        #HCell.delete_spine()
        if len(list_of_segments) == 0:
            return

        #add_spines_on_segments(HCell,list_of_segments)
        num_of_synapses = len(list_of_segments)
        for j in range(num_of_synapses):

            seg = list_of_segments[j]
            SynList.append(h.Exp2Syn(seg.x, sec = seg.sec))
            ConList.append(h.NetCon(config.stim,SynList[-1]))
            SynList[-1].e = config.E_SYN
            SynList[-1].tau1 = config.TAU_1_AMPA
            SynList[-1].tau2 = config.TAU_2_AMPA
            ConList[-1].weight[0]= config.AMPA_W
            h.pop_section()


        for j in range(num_of_synapses):
            
            seg = list_of_segments[j]
            SynList.append(h.NMDA(seg.x, sec = seg.sec))
            ConList.append(h.NetCon(config.stim,SynList[-1]))
            SynList[-1].e=config.E_SYN
            SynList[-1].tau_r_NMDA = config.TAU_1_NMDA
            SynList[-1].tau_d_NMDA = config.TAU_2_NMDA
            SynList[-1].n_NMDA = config.N_NMDA
            SynList[-1].gama_NMDA = config.GAMMA_NMDA
            ConList[-1].weight[0] = config.NMDA_W
            h.pop_section()

    def attach_background_noise(self, all_spike_times, rd, config):
        self.back_stim_list = []
        nspike = len(all_spike_times)
        if self.spine_density > 0:
            syn_segments = self.fill_synapses_vectors_with_spine(nspike, rd, config)
        else:
            syn_segments = fill_synapses_vectors(self.HCell, nspike, rd, config)
        #print(syn_segments[:20])
        for i in range(nspike):
            spikes = all_spike_times[i]
            vs = h.VecStim()
            v = h.Vector(spikes.shape[0])
            v.from_python(spikes)
            vs.play(v)

            seg = syn_segments[i]
            ampa = h.Exp2Syn(seg, sec = seg.sec)
            ampa.e = config.E_SYN
            ampa.tau1 = config.TAU_1_AMPA
            ampa.tau2 = config.TAU_2_AMPA
            nc_ampa = h.NetCon(vs, ampa)
            nc_ampa.weight[0]= config.AMPA_W
            h.pop_section()

            nmda = h.NMDA(seg, sec = seg.sec)
            nmda.e=config.E_SYN
            nmda.tau_r_NMDA = config.TAU_1_NMDA
            nmda.tau_d_NMDA = config.TAU_2_NMDA
            nmda.n_NMDA = config.N_NMDA
            nmda.gama_NMDA = config.GAMMA_NMDA
            nc_nmda = h.NetCon(vs, nmda)
            nc_nmda.weight[0] = config.NMDA_W * np.random.uniform(1.2, 2.5)
            h.pop_section()

            self.back_stim_list.append((v, vs, nc_ampa, nc_nmda, ampa, nmda, spikes))


    def stim_on_spine(self, seg, neck_d = 0.25, neck_l = 1.35, head_area = 2.8, RA = 261.97):
        head_l = 2 * np.sqrt(head_area / 4 / np.pi)
        head_d = head_l
        if seg not in self.spine_seg_list:
            return None, seg
        spine_list_id, length, spine_list_start = self.map_seg2spine_listid[seg]
        if spine_list_start < length:
            neck = self.neck_list_by_seg[spine_list_id][spine_list_start]
            head = self.head_list_by_seg[spine_list_id][spine_list_start]
            self.map_seg2spine_listid[seg][2] += 1
        else:
            neck, head = self.add_single_spine(seg, neck_d, neck_l, head_area)
            #self.map_seg2spine_listid[seg][1] += 1
            self.map_seg2spine_listid[seg][2] += 1
        #self.nstim_spine += 1
        return neck, head
    # Choose random number_of_synapses locations on HCell
    # config is an object with the configurations required for this figure
    def fill_synapses_vectors_with_spine(self, number_of_synapses, rd, config):

        HCell = self.HCell
        total_basal_L = sum([sec.L for sec in HCell.basal])
        total_L = sum([sec.L for sec in HCell.basal]) + sum([sec.L for sec in HCell.apical]) 
        syn_segments = []
        for i in range(number_of_synapses):
            sec,x = random_synapse(HCell,rd,total_L,total_basal_L)
            seg = sec(x)
            if seg not in self.spine_seg_list:
                #print("no spine", seg)
                syn_segments.append(seg)
            else:
                #spine_list_id, nspine = self.map_seg2spine_listid[seg]
                #neck, head = self.add_single_spine(seg)
                neck, head = self.stim_on_spine(seg)
                syn_segments.append(head(config.SPINE_HEAD_X))
                #self.head_list_by_seg[spine_list_id].append(head)
                #self.neck_list_by_seg[spine_list_id].append(neck)
                #try:
                #    spine_list_id, spine_start_id = self.map_seg2spine_listid[seg]
                #    head = self.head_list_by_seg[spine_list_id][spine_start_id]
                #except IndexError:
                #    #pdb.set_trace()
                #    print("IndexError", spine_list_id, spine_start_id)
                #    self.add_single_spine(seg, spine_list_id)
                #    head = self.head_list_by_seg[spine_list_id][spine_start_id]
                #syn_segments.append(head(config.SPINE_HEAD_X))
                #self.map_seg2spine_listid[seg][1] += 1

        return syn_segments

    def fill_clustered_synapses_demo(self,number_of_clusters,rd,config):
        HCell = self.HCell
        segments_for_figure_7 = [(HCell.apic[38],0.85),(HCell.apic[69],0.5),(HCell.apic[90],0.7),
                                (HCell.dend[84],0.75),(HCell.dend[43],0.2),(HCell.dend[41],0.65)]
        syn_segments = []
        for i in range(number_of_clusters):
            sec,X_center = segments_for_figure_7[i]
            for i in range(config.CLUSTER_SIZE):
                if sec.L<config.CLUSTER_L:
                    x = rd.uniform(0,1) 
                elif X_center<config.CLUSTER_L/sec.L:
                    x = rd.uniform(0,config.CLUSTER_L/sec.L) 
                elif X_center>(1-config.CLUSTER_L/sec.L):
                    x = rd.uniform(1-config.CLUSTER_L/sec.L,1)
                else: # the standard case
                    x = rd.uniform(X_center-config.CLUSTER_L/2.0/sec.L,X_center+config.CLUSTER_L/2.0/sec.L)

                #syn_segments.append(sec(x))
                seg = sec(x)
                if seg not in self.spine_seg_list:
                    #print("no spine", seg)
                    syn_segments.append(seg)
                else:
                    #spine_list_id, nspine = self.map_seg2spine_listid[seg]
                    neck, head = self.add_single_spine(seg)
                    syn_segments.append(head(config.SPINE_HEAD_X))
                    #self.map_seg2spine_listid[seg][1] += 1
                    #self.head_list_by_seg[spine_list_id].append(head)
                    #self.neck_list_by_seg[spine_list_id].append(neck)

        return syn_segments

    def fill_clustered_synapses_vectors_with_spine(self, number_of_clusters, rd, config):
        '''
        Chooses random number_of_clusters locations on HCell
        The center of each cluster is chosen randomly, and the synapses within a cluster
        are distributed within config.CLUSTER_L which is 20 um in this work
        '''
        HCell = self.HCell
        total_basal_L = sum([sec.L for sec in HCell.basal])
        total_L = sum([sec.L for sec in HCell.basal]) + sum([sec.L for sec in HCell.apical]) 

        syn_segments = []
        for i in range(number_of_clusters):
            sec,X_center = random_synapse(HCell,rd,total_L,total_basal_L)
            for i in range(config.CLUSTER_SIZE):
                if sec.L<config.CLUSTER_L:
                    x = rd.uniform(0,1) 
                elif X_center<config.CLUSTER_L/sec.L:
                    x = rd.uniform(0,config.CLUSTER_L/sec.L) 
                elif X_center>(1-config.CLUSTER_L/sec.L):
                    x = rd.uniform(1-config.CLUSTER_L/sec.L,1)
                else: # the standard case
                    x = rd.uniform(X_center-config.CLUSTER_L/2.0/sec.L,X_center+config.CLUSTER_L/2.0/sec.L)

                #syn_segments.append(sec(x))
                seg = sec(x)
                if seg not in self.spine_seg_list:
                    #print("no spine", seg)
                    syn_segments.append(seg)
                else:
                    #spine_list_id, nspine = self.map_seg2spine_listid[seg]
                    #neck, head = self.add_single_spine(seg)
                    neck, head = self.stim_on_spine(seg)
                    syn_segments.append(head(config.SPINE_HEAD_X))
                    #self.map_seg2spine_listid[seg][1] += 1
                    #self.head_list_by_seg[spine_list_id].append(head)
                    #self.neck_list_by_seg[spine_list_id].append(neck)
                    #try:
                    #    spine_list_id, spine_start_id = self.map_seg2spine_listid[sec(x)]
                    #    head = self.head_list_by_seg[spine_list_id][spine_start_id]
                    #except IndexError:
                    #    #pdb.set_trace()
                    #    print("IndexError", spine_list_id, spine_start_id)
                    #    self.add_single_spine(seg, spine_list_id)
                    #    head = self.head_list_by_seg[spine_list_id][spine_start_id]
                    #syn_segments.append(head(config.SPINE_HEAD_X))
                    #self.map_seg2spine_listid[sec(x)][1] += 1

        return syn_segments

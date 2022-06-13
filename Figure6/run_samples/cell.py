import numpy as np

from neuron import h
import pdb


# generate variable lists for DeepDendrite
def gen_files(network, out_g, out_in2hd_syn, out_h2out_syn, out_w, out_dw, out_stim):
    for i in range(10):
        for syn in network.synlist_in2hd:
            seg = syn.get_segment()
            mech_id = network.seg2synlist[seg].index(syn)
            out_g.append("0 %s %f BP_Syn %d g_%d\n"%(seg.sec, seg.x, mech_id, i))
    for i in range(10):
        for syn in network.synlist_in2hd_bias:
            seg = syn.get_segment()
            mech_id = network.seg2synlist[seg].index(syn)
            out_g.append("0 %s %f BP_Syn %d g_%d\n"%(seg.sec, seg.x, mech_id, i))

    for i in range(10):
        for syn in network.synlist_in2hd + network.synlist_in2hd_bias:
            seg = syn.get_segment()
            mech_id = network.seg2synlist[seg].index(syn)
            out_in2hd_syn.append("0 %s %f BP_Syn %d tgt_%d\n"%(seg.sec, seg.x, mech_id, i))

    for i, syn in enumerate(network.synlist_h2out):
        if i > 0 and i % 64 == 0:

            syn_bias = network.synlist_h2out_bias[i // 64 - 1]
            seg = syn_bias.get_segment()
            mech_id = network.seg2synlist[seg].index(syn_bias)
            out_h2out_syn.append("0 %s %f BP_Syn %d u_tgt\n"%(seg.sec, seg.x, mech_id))
        seg = syn.get_segment()
        mech_id = network.seg2synlist[seg].index(syn)
        out_h2out_syn.append("0 %s %f BP_Syn %d u_tgt\n"%(seg.sec, seg.x, mech_id))

    syn_bias = network.synlist_h2out_bias[-1]
    seg = syn_bias.get_segment()
    mech_id = network.seg2synlist[seg].index(syn_bias)
    out_h2out_syn.append("0 %s %f BP_Syn %d u_tgt\n"%(seg.sec, seg.x, mech_id))

    for syn in network.synlist_in2hd + network.synlist_in2hd_bias + network.synlist_h2out + network.synlist_h2out_bias:
        seg = syn.get_segment()
        mech_id = network.seg2synlist[seg].index(syn)
        out_w.append("0 %s %f BP_Syn %d w\n"%(seg.sec, seg.x, mech_id))

    for syn in network.synlist_in2hd + network.synlist_in2hd_bias + network.synlist_h2out + network.synlist_h2out_bias:
        seg = syn.get_segment()
        mech_id = network.seg2synlist[seg].index(syn)
        out_dw.append("0 %s %f BP_Syn %d delta_w\n"%(seg.sec, seg.x, mech_id))

    for i, ns in enumerate(network.stimlist):
        stim_name = str(ns)
        i1 = stim_name.find("[")
        i2 = stim_name.find("]")
        order = int(stim_name[i1 + 1 : i2])
        out_stim.append("1 NetStim %d interval\n"%order)
        out_stim.append("1 NetStim %d start\n"%order)

def gen_rec_vols_file(neuron_list):
    rec_seg_list = []
    vertex_list = []
    indices_list = []
    offset = 0
    ipoint = 0
    for icell, cell in enumerate(neuron_list):
        soma = [cell.soma[0]]
        dends = [sec for sec in cell.dend]
        apics = [sec for sec in cell.apic]
        for sec in soma + dends + apics:
            npt3d = int(h.n3d(sec = sec))
            nseg = sec.nseg
            npt_each_seg = npt3d // nseg 
            for i in range(npt3d):
                x = sec.x3d(i)
                y = sec.y3d(i)
                z = sec.z3d(i)
                diam = sec.diam3d(i)
                iseg = i // npt_each_seg
                if iseg >= offset + nseg:
                    iseg = offset + nseg - 1
                vertex_list.append((x, y, z, diam, offset + iseg, icell))
                if i > 0:
                    indices_list.append((ipoint - 1, ipoint))
                ipoint += 1
                h.pop_section()
                h.pop_section()
                h.pop_section()
            for seg in sec:
                rec_seg_list.append(seg)
                offset += 1
            h.pop_section()

    nvertices = len(vertex_list)
    parent = [-1] * nvertices
    rec_param_outlines = []
    vertex_outlines = []
    indices_outlines = []
    for idx1, idx2 in indices_list:
        parent[idx2] = idx1
    for i, vertex in enumerate(vertex_list):
        vertex_outlines.append("%f %f %f %f %d %d\n"%(vertex[0], vertex[1], vertex[2], vertex[3], vertex[4], parent[i]))
    with open("vertex", "w") as f:
        f.write("%d\n"%len(vertex_outlines))
        f.writelines(vertex_outlines)

# generate gap junction variable file for DeepDendrite
def non_v_gap(syn, param, sid, gap_src_params, out_lines, gap_mechs):
    tgt_seg = syn.get_segment()
    src_str = gap_src_params[sid]
    tgt_str = "%s %f BP_Syn %s\n"%(tgt_seg.sec, tgt_seg.x, param)
    gap_mechs.append(syn)
    out_lines.append(src_str + " " + tgt_str)

HIDDEN_CM = 1.5

class Network_parallel:
    def __init__(self, gid_start, N_in=784, N_hidden=64, N_out=10, seed=1234):
        self.gids = []

        self.rng = np.random.default_rng(seed=seed)
        self.N_in = N_in
        self.N_hidden = N_hidden
        self.N_out = N_out
        self.sid_offset = 0
        self.gid_start = gid_start

        self.pc = h.ParallelContext()
        self.nhost = self.pc.nhost()
        self.ihost = self.pc.id()

        self._set_network()

    def _set_network(self):
        self._create_cells()
        self._connect_cells()
        self._init_stim()
    
    def _setup_hpc(self, model, morph):
        cell = getattr(h, model)()
        nl = h.Import3d_Neurolucida3()
        nl.quiet = 1
        nl.input(morph)
        imprt = h.Import3d_GUI(nl, 0)   
        imprt.instantiate(cell)    
        cell.indexSections(imprt)
        cell.geom_nsec()
        cell.geom_nseg()
        cell.delete_axon()
        cell.insertChannel()
        cell.init_rc()
        cell.biophys()
        return cell

    def _create_cells(self):
        self.inputs = []
        self.nclist_in = []
        self.synlist_ns2in = []
        h.load_file("import3d.hoc")
        h.load_file("PointNeuron.hoc")
        h.load_file("PassiveHPC.hoc")
        for i in range(self.N_in):
            if (i % self.nhost) != self.ihost:
                continue
            gid = i + self.gid_start
            self.pc.set_gid2node(gid, self.ihost)
            self.gids.append(gid)
            inputNeuron = h.PointNeuron()
            for sec in inputNeuron.all:
                sec.Ra = 100
                sec.cm = 1
                sec.g_pas = 1e-4
            self.inputs.append(inputNeuron)
            nc = h.NetCon(inputNeuron.soma(0.5)._ref_v, None, sec=inputNeuron.soma)
            h.pop_section()
            self.nclist_in.append(nc)
            self.pc.cell(gid, nc)
            
            syn = h.ExpSyn(inputNeuron.soma(0.5))
            syn.tau = 0.5
            syn.e = 1
            self.synlist_ns2in.append(syn)

        self.hiddens = []
        self.nclist_h = []
        for i in range(self.N_hidden):
            if (i % self.nhost) != self.ihost:
                continue
            gid = self.N_in + i + self.gid_start
            self.pc.set_gid2node(gid, self.ihost)
            self.gids.append(gid)
            hiddenNeuron = self._setup_hpc("PassiveHPC", "2013_03_06_cell11_1125_H41_06.asc")
            for sec in hiddenNeuron.all:
                sec.cm = HIDDEN_CM
            self.hiddens.append(hiddenNeuron)
            nc = h.NetCon(hiddenNeuron.soma[0](0.5)._ref_v, None, sec=hiddenNeuron.soma[0])
            h.pop_section()
            self.nclist_h.append(nc)
            self.pc.cell(gid, nc)

        self.outputs = []
        self.nclist_out = []
        for i in range(self.N_out):
            if (i % self.nhost) != self.ihost:
                continue
            gid = self.N_in + self.N_hidden + i + self.gid_start
            self.pc.set_gid2node(gid, self.ihost)
            self.gids.append(gid)
            outputNeuron = h.PointNeuron()
            for sec in outputNeuron.all:
                sec.Ra = 100
                sec.cm = 1
                sec.g_pas = 1e-4
            self.outputs.append(outputNeuron)
            nc = h.NetCon(outputNeuron.soma(0.5)._ref_v, None, sec=outputNeuron.soma)
            h.pop_section()
            self.nclist_out.append(nc)
            self.pc.cell(gid, nc)

        self.inters = []
        self.nclist_inter = []
        gid = self.N_in + self.N_hidden + self.N_out + self.gid_start
        self.pc.set_gid2node(gid, self.ihost)
        self.gids.append(gid)
        interNeuron = h.PointNeuron()
        self.inters.append(interNeuron)
        nc = h.NetCon(interNeuron.soma(0.5)._ref_v, None, sec=interNeuron.soma)
        self.nclist_inter.append(nc)
        self.pc.cell(gid, nc)

    
    def _init_weights(self):
        self.num_proj_dend = 1
        self.total_dend = len(self.hiddens[0].dend)
        # weight matrix input-to-hidden dendrites
        limit = np.sqrt(6. / (self.N_in * self.num_proj_dend + self.N_hidden))
        self.in2hd_w = self.rng.uniform(-limit, limit, (self.N_in, self.N_hidden, self.num_proj_dend))
        self.in2hd_b = np.zeros((self.N_hidden,))#self.rng.uniform(0.0, 0.03, (self.N_hidden))
        self.dw_in2hd = np.zeros((self.N_in, self.N_hidden, self.num_proj_dend))
        self.db_in2hd = np.zeros((self.N_hidden,))

        # weight matrix hidden-to-output, all to all
        limit = np.sqrt(6. / (self.N_hidden + self.N_out))
        self.h2out_w = self.rng.uniform(-limit, limit, (self.N_hidden, self.N_out))
        self.h2out_b = np.zeros((self.N_out,))#self.rng.uniform(0.0, 0.01, (self.N_out))
        self.dw_h2out = np.zeros((self.N_hidden, self.N_out))
        self.db_h2out = np.zeros((self.N_out,))

    def _connect_cells(self):
        self.num_proj_dend = 1
        self.total_dend = len(self.hiddens[0].dend)
        # connection matrix input-to-hidden dendrites, 1 input connects num_proj_dend(3 default) pyramidal
        self.in2hd_conn = self.rng.integers(0, self.total_dend, (self.N_in, self.N_hidden, self.num_proj_dend))
        self.in2hd_conn_loc = self.rng.random((self.N_in, self.N_hidden, self.num_proj_dend))
        
        # weight matrix input-to-hidden dendrites
        limit = np.sqrt(6. / (self.N_in * self.num_proj_dend + self.N_hidden))
        self.in2hd_w = self.rng.uniform(-limit, limit, (self.N_in, self.N_hidden, self.num_proj_dend))
        self.in2hd_b = np.zeros((self.N_hidden,))#self.rng.uniform(0.0, 0.03, (self.N_hidden))
        self.dw_in2hd = np.zeros((self.N_in, self.N_hidden, self.num_proj_dend))
        self.db_in2hd = np.zeros((self.N_hidden,))

        # weight matrix hidden-to-output, all to all
        limit = np.sqrt(6. / (self.N_hidden + self.N_out))
        self.h2out_w = self.rng.uniform(-limit, limit, (self.N_hidden, self.N_out))
        self.h2out_b = np.zeros((self.N_out,))#self.rng.uniform(0.0, 0.01, (self.N_out))
        self.dw_h2out = np.zeros((self.N_hidden, self.N_out))
        self.db_h2out = np.zeros((self.N_out,))
        

        gap_src_params = []
        self.gap_out_lines = []
        gap_mechs = []
        src_mechs = []
        self.seg2synlist = {}
        # var_index
        for i in range(self.N_in):
            targid = i + self.gid_start
            if self.pc.gid_exists(targid):
                target = self.pc.gid2cell(targid)
                sid_v_gap = targid + self.sid_offset
                gap_src_params.append("%s 0.5 soma v"%target.soma)
                src_mechs.append(None)
                h.pop_section()
        for i in range(self.N_hidden):
            targid = self.N_in + i + self.gid_start
            if self.pc.gid_exists(targid):
                target = self.pc.gid2cell(targid)
                sid_v_gap = targid + self.sid_offset
                gap_src_params.append("%s 0.5 soma v"%target.soma[0])
                src_mechs.append(None)
                h.pop_section()
        for i in range(self.N_out):
            targid = self.N_in + self.N_hidden + i + self.gid_start
            if self.pc.gid_exists(targid):
                target = self.pc.gid2cell(targid)
                sid_v_gap = targid + self.sid_offset
                gap_src_params.append("%s 0.5 soma v"%target.soma)
                src_mechs.append(None)
                h.pop_section()

        # output to inter
        self.synlist_out2inter = []
        targid = self.N_in + self.N_hidden + self.N_out + self.gid_start
        if self.pc.gid_exists(targid):
            interNeuron = self.pc.gid2cell(targid)
            syn = h.BP_Syn(interNeuron.soma(0.5))
            syn.layer_flag = 2
            self.synlist_out2inter.append(syn)
            tgt_seg = syn.get_segment()
            if not tgt_seg in self.seg2synlist:
                self.seg2synlist[tgt_seg] = []
            self.seg2synlist[tgt_seg].append(syn)
            sid_start = self.N_in + self.N_hidden + self.gid_start

            for i in range(10):
                non_v_gap(syn, "u_" + str(i), sid_start + i + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)

            sid_start = self.N_in + self.N_hidden + self.N_out + self.gid_start
            gap_src_params.append("%s %f BP_Syn s_0"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_1, sid_start + 1 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_1"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_2, sid_start + 2 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_2"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_3, sid_start + 3 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_3"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_4, sid_start + 4 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_4"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_5, sid_start + 5 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_5"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_6, sid_start + 6 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_6"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_7, sid_start + 7 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_7"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_8, sid_start + 8 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_8"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()
            #self.pc.source_var(syn._ref_s_9, sid_start + 9 + self.sid_offset, sec = interNeuron.soma)
            gap_src_params.append("%s %f BP_Syn s_9"%(tgt_seg.sec, tgt_seg.x))
            h.pop_section()

        # input to hidden
        # transfer resistance matrix
        tmpcell = self._setup_hpc("PassiveHPC", "2013_03_06_cell11_1125_H41_06.asc")
        for sec in tmpcell.all:
            sec.cm = HIDDEN_CM
        impd = h.Impedance()
        impd.loc(0.5, sec=tmpcell.soma[0])
        # impd.loc(0.5, sec=tmpcell.soma)
        impd.compute(0)
        self.in2hd_transr = np.ones((self.N_in, self.N_hidden, self.num_proj_dend))
        for i in range(self.N_hidden):
            for j in range(self.N_in):
                for k in range(self.num_proj_dend):
                    dend_id = self.in2hd_conn[j, i, k]
                    loc = self.in2hd_conn_loc[j, i, k]
                    self.in2hd_transr[j, i, k] = impd.transfer(loc, sec=tmpcell.dend[dend_id])
        
        self.synlist_in2hd = []
        self.synlist_in2hd_bias = []
        for i in range(self.N_hidden):
            targid = self.N_in + i + self.gid_start
            if self.pc.gid_exists(targid):
                target = self.pc.gid2cell(targid)
                
                for j in range(self.N_in):
                    sid_v_gap = j + self.sid_offset + self.gid_start

                    for k in range(self.num_proj_dend):
                       
                        dend_id = self.in2hd_conn[j, i, k]
                        loc = self.in2hd_conn_loc[j, i, k]
                        syn = h.BP_Syn(target.dend[dend_id](loc))
                        syn.layer_flag = 0
                        self.synlist_in2hd.append(syn)
                        seg = syn.get_segment()
                        if not seg in self.seg2synlist:
                            self.seg2synlist[seg] = []
                        self.seg2synlist[seg].append(syn)

                        r = self.in2hd_transr[j, i, k]
                        r_mean = np.sqrt(np.max(self.in2hd_transr)*np.min(self.in2hd_transr))
                        syn.g = 1 / r_mean
                        syn.learning_rate = 0.005 * r / r_mean

                        non_v_gap(syn, "v_gap", sid_v_gap - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
                        non_v_gap(syn, "u_soma", targid - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
                
                # input bias
                syn = h.BP_Syn(target.soma[0](0.5))
                syn.layer_flag = 0
                self.synlist_in2hd_bias.append(syn)
                seg = syn.get_segment()
                if not seg in self.seg2synlist:
                    self.seg2synlist[seg] = []
                self.seg2synlist[seg].append(syn)

                r = impd.transfer(0.5, sec=tmpcell.soma[0])
                syn.g = 1 / r
                syn.learning_rate = 0.005


                syn.v_gap = 1.0
                non_v_gap(syn, "u_soma", targid - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)

        # hidden to output
        tmpcell = h.PointNeuron()
        for sec in tmpcell.all:
            sec.Ra = 100
            sec.cm = 1
            sec.g_pas = 1e-4
        impd = h.Impedance()
        impd.loc(0.5, sec=tmpcell.soma)
        impd.compute(0)
        r = impd.transfer(0.5, sec=tmpcell.soma)

        self.synlist_h2out = []
        self.synlist_h2out_bias = []
        for i in range(self.N_out):
            targid = self.N_in + self.N_hidden + i + self.gid_start
            sid_start = self.N_in + self.N_hidden + self.N_out + self.gid_start
            if self.pc.gid_exists(targid):
                target = self.pc.gid2cell(targid)

                for j in range(self.N_hidden):
                    sid_u_soma = self.N_in + self.N_hidden + i + self.gid_start + self.sid_offset 
                    sid_v_gap = self.N_in + j + self.gid_start + self.sid_offset

                    syn = h.BP_Syn(target.soma(0.5))
                    syn.layer_flag = 1
                    self.synlist_h2out.append(syn)
                    seg = syn.get_segment()
                    if not seg in self.seg2synlist:
                        self.seg2synlist[seg] = []
                    self.seg2synlist[seg].append(syn)

                    syn.g = 1 / r
                    syn.learning_rate = 0.005

                    tgt_seg = syn.get_segment()
                    non_v_gap(syn, "v_gap", sid_v_gap - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)

                    non_v_gap(syn, "s_u_soma", sid_start + i + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
                
                # hidden bias
                sid_u_soma = self.N_in + self.N_hidden + i + self.gid_start + self.sid_offset

                syn = h.BP_Syn(target.soma(0.5))
                syn.layer_flag = 1
                self.synlist_h2out_bias.append(syn)
                seg = syn.get_segment()
                if not seg in self.seg2synlist:
                    self.seg2synlist[seg] = []
                self.seg2synlist[seg].append(syn)

                syn.g = 1 / r
                syn.learning_rate = 0.005

                non_v_gap(syn, "s_u_soma", sid_start + i + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
                syn.v_gap = 1.0

        
        sid_start = self.N_in + self.N_hidden + self.N_out + self.gid_start
        for syn in self.synlist_in2hd + self.synlist_in2hd_bias:
            tgt_seg = syn.get_segment()
            #self.pc.target_var(syn, syn._ref_s_0, sid_start + 0 + self.sid_offset)
            non_v_gap(syn, "s_0", sid_start + 0 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)

            #self.pc.target_var(syn, syn._ref_s_1, sid_start + 1 + self.sid_offset)
            non_v_gap(syn, "s_1", sid_start + 1 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_2", sid_start + 2 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_3", sid_start + 3 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_4", sid_start + 4 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_5", sid_start + 5 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_6", sid_start + 6 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_7", sid_start + 7 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_8", sid_start + 8 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)
            non_v_gap(syn, "s_9", sid_start + 9 + self.sid_offset - self.gid_start, gap_src_params, self.gap_out_lines, gap_mechs)

        for synlist in self.seg2synlist.values():
            synlist.reverse()
        for i, line in enumerate(self.gap_out_lines):
            line_spt = line.split()
            tgt_mech = gap_mechs[i]
            tgt_seg = tgt_mech.get_segment()
            mech_id = self.seg2synlist[tgt_seg].index(tgt_mech)
            line_spt[2] += " 0"
            line_spt[6] = line_spt[6] + " " + str(mech_id)
            self.gap_out_lines[i] = " ".join(line_spt) + "\n"


    def _init_stim(self):
        self.stimlist = []
        self.ncstimlist = []
        for i in range(self.N_in):
            gid = self.N_in + self.N_hidden + self.N_out + 11 + i + self.gid_start
            targid = i + self.gid_start
            if self.pc.gid_exists(targid):
                target = self.pc.gid2cell(targid)

                stim = h.NetStim()
                self.stimlist.append(stim)
                stim.interval = 1e9
                stim.start = 1e9
                stim.number = 1e9
                # poisson
                stim.noise = 0
                
                self.pc.set_gid2node(gid, self.ihost)
                ncstim = h.NetCon(stim, self.synlist_ns2in[i // self.nhost])
                self.pc.cell(gid, ncstim)
                self.ncstimlist.append(ncstim)
                ncstim.delay = 1
                ncstim.weight[0] = 0.05

    def set_stim(self, img, tgt):
        for i in range(self.N_in):
            targid = i + self.gid_start
            if self.pc.gid_exists(targid):
                stim = self.stimlist[i // self.nhost]
                stim.interval = 5. / (img.flatten()[i] + 0.01)
                stim.start = 9. + stim.interval

        for syn in self.synlist_in2hd + self.synlist_in2hd_bias:
            syn.tgt_0 = 0.0
            syn.tgt_1 = 0.0
            syn.tgt_2 = 0.0
            syn.tgt_3 = 0.0
            syn.tgt_4 = 0.0
            syn.tgt_5 = 0.0
            syn.tgt_6 = 0.0
            syn.tgt_7 = 0.0
            syn.tgt_8 = 0.0
            syn.tgt_9 = 0.0

            if tgt == 0:
                syn.tgt_0 = 1.0
            elif tgt == 1:
                syn.tgt_1 = 1.0
            elif tgt == 2:
                syn.tgt_2 = 1.0
            elif tgt == 3:
                syn.tgt_3 = 1.0
            elif tgt == 4:
                syn.tgt_4 = 1.0
            elif tgt == 5:
                syn.tgt_5 = 1.0
            elif tgt == 6:
                syn.tgt_6 = 1.0
            elif tgt == 7:
                syn.tgt_7 = 1.0
            elif tgt == 8:
                syn.tgt_8 = 1.0
            elif tgt == 9:
                syn.tgt_9 = 1.0

        for i, syn in enumerate(self.synlist_h2out):
            if (i // self.N_hidden) * self.nhost + self.ihost == tgt:
                syn.u_tgt = 1.0
            else:
                syn.u_tgt = 0.0
        
        for i, syn in enumerate(self.synlist_h2out_bias):
            if i * self.nhost + self.ihost == tgt:
                syn.u_tgt = 1.0
            else:
                syn.u_tgt = 0.0

    def is_train(self):
        for syn in self.synlist_in2hd + self.synlist_in2hd_bias + self.synlist_h2out + self.synlist_h2out_bias:
            syn.lr_start = 20
            syn.lr_end = 1e9

    def is_test(self):
        for syn in self.synlist_in2hd + self.synlist_in2hd_bias + self.synlist_h2out + self.synlist_h2out_bias:
            syn.lr_start = 1e9
            syn.lr_end = 1e9
    
    def save_weights(self):
        for i, syn in enumerate(self.synlist_in2hd):
            x = (i // self.num_proj_dend) % self.N_in
            y = ((i // self.num_proj_dend) // self.N_in) * self.nhost + self.ihost
            z = i % self.num_proj_dend
            self.in2hd_w[x, y, z] = syn.w + syn.delta_w / (30. / h.dt)
        for i, syn in enumerate(self.synlist_in2hd_bias):
            self.in2hd_b[i * self.nhost + self.ihost] = syn.w + syn.delta_w / (30. / h.dt)
        for i, syn in enumerate(self.synlist_h2out):
            x = i % self.N_hidden
            y = (i // self.N_hidden) * self.nhost + self.ihost
            self.h2out_w[x, y] = syn.w + syn.delta_w / (30. / h.dt)
        for i, syn in enumerate(self.synlist_h2out_bias):
            self.h2out_b[i * self.nhost + self.ihost] = syn.w + syn.delta_w / (30. / h.dt)

    def save_delta_w(self):
        for i, syn in enumerate(self.synlist_in2hd):
            x = (i // self.num_proj_dend) % self.N_in
            y = ((i // self.num_proj_dend) // self.N_in) * self.nhost + self.ihost
            z = i % self.num_proj_dend
            self.dw_in2hd[x, y, z] = syn.delta_w / (30. / h.dt)
        for i, syn in enumerate(self.synlist_in2hd_bias):
            self.db_in2hd[i * self.nhost + self.ihost] = syn.delta_w / (30. / h.dt)
        for i, syn in enumerate(self.synlist_h2out):
            x = i % self.N_hidden
            y = (i // self.N_hidden) * self.nhost + self.ihost
            self.dw_h2out[x, y] = syn.delta_w / (30. / h.dt)
        for i, syn in enumerate(self.synlist_h2out_bias):
            self.db_h2out[i * self.nhost + self.ihost] = syn.delta_w / (30. / h.dt)
    
    def load_weights(self):
        for i, syn in enumerate(self.synlist_in2hd):
            x = (i // self.num_proj_dend) % self.N_in
            y = ((i // self.num_proj_dend) // self.N_in) * self.nhost + self.ihost
            z = i % self.num_proj_dend
            syn.w = self.in2hd_w[x, y, z]
        for i, syn in enumerate(self.synlist_in2hd_bias):
            syn.w = self.in2hd_b[i * self.nhost + self.ihost]
        for i, syn in enumerate(self.synlist_h2out):
            x = i % self.N_hidden
            y = (i // self.N_hidden) * self.nhost + self.ihost
            syn.w = self.h2out_w[x, y]
        for i, syn in enumerate(self.synlist_h2out_bias):
            syn.w = self.h2out_b[i * self.nhost + self.ihost]
        
        for i, syn in enumerate(self.synlist_in2hd):
            y = ((i // self.num_proj_dend) // self.N_in) * self.nhost + self.ihost
            syn.g_0 = self.h2out_w[y, 0]
            syn.g_1 = self.h2out_w[y, 1]
            syn.g_2 = self.h2out_w[y, 2]
            syn.g_3 = self.h2out_w[y, 3]
            syn.g_4 = self.h2out_w[y, 4]
            syn.g_5 = self.h2out_w[y, 5]
            syn.g_6 = self.h2out_w[y, 6]
            syn.g_7 = self.h2out_w[y, 7]
            syn.g_8 = self.h2out_w[y, 8]
            syn.g_9 = self.h2out_w[y, 9]
        
        for i, syn in enumerate(self.synlist_in2hd_bias):
            y = i * self.nhost + self.ihost
            syn.g_0 = self.h2out_w[y, 0]
            syn.g_1 = self.h2out_w[y, 1]
            syn.g_2 = self.h2out_w[y, 2]
            syn.g_3 = self.h2out_w[y, 3]
            syn.g_4 = self.h2out_w[y, 4]
            syn.g_5 = self.h2out_w[y, 5]
            syn.g_6 = self.h2out_w[y, 6]
            syn.g_7 = self.h2out_w[y, 7]
            syn.g_8 = self.h2out_w[y, 8]
            syn.g_9 = self.h2out_w[y, 9]

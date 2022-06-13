import errno
import os
from neuron import h

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:    # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

def voltageout(foldername, recordlist):
    for vec in recordlist:
        #print only last recorded
        print (vec.label(), vec.x[int(vec.size()) - 1])
        #vec.printf()


def celltypeinfo(type, nbranch, ncompart):

    r = h.Random()
    r.Random123(type, 2)
    nb = int(r.discunif(nbranch[0], nbranch[1]))
    secpar = h.Vector(nb)
    segvec = h.Vector(nb)
    r.discunif(ncompart[0], ncompart[1])
    for i in range(nb):
        segvec.x[i] = int(r.repick())

    # nb branches and every branch has 0, 1, or 2 children
    # ie. no integer in secpar appears more than twice
    x = [[0, 0]]
    for i in range(1, nb):
        a = int(r.discunif(0, len(x) - 1))
        secpar.x[i] = x[a][0]
        x[a][1] += 1
        if x[a][1] > 1:
            x[a][0] = i
            x[a][1] = 0
        else:
            x.append([i, 0])

    #print type, secpar.to_python()
    return secpar, segvec

# for nhost independent type for gid
# shuffle elements of vec
def shuffle(vec, ran):
    n = len(vec) - 1
    for i in range(n + 1):
        ix = int(ran.discunif(i, n))
        vec[i], vec[ix] = vec[ix], vec[i]
    return vec


def spike_record():
    global tvec, idvec
    pc = h.ParallelContext()
    tvec = h.Vector(1000000)
    idvec = h.Vector(1000000)
    pc.spike_record(-1, tvec, idvec)


def spikeout(folder):
    #to out<nhost>.dat file
    global tvec, idvec
    pc = h.ParallelContext()
    rank = int(pc.id())
    nhost = int(pc.nhost())

    pc.barrier()
    
    fname = folder + '/spk%d.std' % nhost

    if rank == 0:
        f = open(fname, 'w')
        f.close()
    for r in range(nhost):
        if r == rank:
            f = open(fname, 'a')
            for i in range(len(tvec)):
                f.write('%g %d\n' % (tvec.x[i], int(idvec.x[i])))
            f.close()
        pc.barrier()


    timeit("wrote %d spikes%s" % (int(pc.allreduce(tvec.size(), 1)),
                                  ("" if nhost == 1 else " (unsorted)")), rank)


def timeit(message, rank):
    global _timeit
    if message == None:
        _timeit = h.startsw()
    else:
        x = h.startsw()
        if rank == 0:
            print ('%gs %s' % ((x - _timeit), message))
        _timeit = x


# function to register section-segment mapping with bbcore write
def setup_nrnbbcore_register_mapping(rings):

    #for recording
    recordlist = []

    #vector for soma sections and segment
    somasec = h.Vector()
    somaseg = h.Vector()

    #vector for dendrite sections and segment
    densec = h.Vector()
    denseg = h.Vector()

    pc = h.ParallelContext()

    #all rings in the simulation
    for ring in rings:

        #every gid in the ring
        for gid in ring.gids:

            #clear previous vector if any
            somasec.size(0)
            somaseg.size(0)
            densec.size(0)
            denseg.size(0)

            #if gid exist on rank
            if (pc.gid_exists(gid)):

                #get cell instance
                cell = pc.gid2cell(gid)
                isec = 0

                #soma section, only pne
                for sec in [cell.soma]:
                    for seg in sec:
                        #get section and segment index
                        somasec.append(isec)
                        somaseg.append(seg.node_index())

                        #vector for recording
                        v = h.Vector()
                        v.record(seg._ref_v)
                        v.label("soma %d %d" % (isec, seg.node_index()))
                        recordlist.append(v)
                isec += 1

                #for sections in dendrite
                for sec in cell.den:
                    for seg in sec:
                        densec.append(isec)
                        denseg.append(seg.node_index())

                        #for recordings
                        v = h.Vector()
                        v.record(seg._ref_v)
                        v.label("dend %d %d" % (isec, seg.node_index()))
                        recordlist.append(v)
                    isec += 1

        #register soma section list
        pc.nrnbbcore_register_mapping(gid, "soma", somasec, somaseg)

        #register dend section list
        pc.nrnbbcore_register_mapping(gid, "dend", densec, denseg)

    return recordlist

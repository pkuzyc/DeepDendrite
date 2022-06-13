/*
Copyright (c) 2016, Blue Brain Project
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "coreneuron/utils/randoms/nrnran123.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include "coreneuron/nrnconf.h"
#include "coreneuron/nrnoc/multicore.h"
#include "coreneuron/nrniv/nrniv_decl.h"
#include "coreneuron/nrnoc/nrnoc_decl.h"
#include "coreneuron/nrniv/vrecitem.h"
#include "coreneuron/nrniv/multisend.h"
#include "coreneuron/utils/sdprintf.h"
#include "coreneuron/nrniv/nrn_assert.h"
#include "coreneuron/nrniv/nrnmutdec.h"
#include "coreneuron/nrniv/memory.h"
#include "coreneuron/nrniv/nrn_setup.h"
#include "coreneuron/nrniv/partrans.h"
#include "coreneuron/nrniv/nrnoptarg.h"
#include "coreneuron/nrniv/nrn_checkpoint.h"
#include "coreneuron/nrniv/node_permute.h"
#include "coreneuron/nrniv/cellorder.h"
#include "coreneuron/utils/reports/nrnsection_mapping.h"
#include "coreneuron/nrnoc/mech_mapping.hpp"

// callbacks into nrn/src/nrniv/nrnbbcore_write.cpp
#include "coreneuron/nrniv/nrn2core_direct.h"

int corenrn_embedded;
int corenrn_embedded_nthread;

void (*nrn2core_group_ids_)(int*);

void (*nrn2core_get_partrans_setup_info_)(int tid,
                                          int& ntar,
                                          int& nsrc,
                                          int& type,
                                          int& ix_vpre,
                                          int*& sid_target,
                                          int*& sid_src,
                                          int*& v_indices);

int (*nrn2core_get_dat1_)(int tid,
                          int& n_presyn,
                          int& n_netcon,
                          int*& output_gid,
                          int*& netcon_srcgid);

int (*nrn2core_get_dat2_1_)(int tid,
                            int& ngid,
                            int& n_real_gid,
                            int& nnode,
                            int& ndiam,
                            int& nmech,
                            int*& tml_index,
                            int*& ml_nodecount,
                            int& nidata,
                            int& nvdata,
                            int& nweight);

int (*nrn2core_get_dat2_2_)(int tid,
                            int*& v_parent_index,
                            double*& a,
                            double*& b,
                            double*& area,
                            double*& v,
                            double*& diamvec);

int (*nrn2core_get_dat2_mech_)(int tid,
                               size_t i,
                               int dsz_inst,
                               int*& nodeindices,
                               double*& data,
                               int*& pdata);

int (*nrn2core_get_dat2_3_)(int tid,
                            int nweight,
                            int*& output_vindex,
                            double*& output_threshold,
                            int*& netcon_pnttype,
                            int*& netcon_pntindex,
                            double*& weights,
                            double*& delays);

int (*nrn2core_get_dat2_corepointer_)(int tid, int& n);

int (*nrn2core_get_dat2_corepointer_mech_)(int tid,
                                           int type,
                                           int& icnt,
                                           int& dcnt,
                                           int*& iarray,
                                           double*& darray);

int (*nrn2core_get_dat2_vecplay_)(int tid, int& n);

int (*nrn2core_get_dat2_vecplay_inst_)(int tid,
                                       int i,
                                       int& vptype,
                                       int& mtype,
                                       int& ix,
                                       int& sz,
                                       double*& yvec,
                                       double*& tvec);

// file format defined in cooperation with nrncore/src/nrniv/nrnbbcore_write.cpp
// single integers are ascii one per line. arrays are binary int or double
// Note that regardless of the gid contents of a group, since all gids are
// globally unique, a filename convention which involves the first gid
// from the group is adequate. Also note that balance is carried out from a
// per group perspective and launching a process consists of specifying
// a list of group ids (first gid of the group) for each process.
//
// <firstgid>_1.dat
// n_presyn, n_netcon
// output_gids (npresyn) with -(type+1000*index) for those acell with no gid
// netcon_srcgid (nnetcon) -(type+1000*index) refers to acell with no gid
//                         -1 means the netcon has no source (not implemented)
// Note that the negative gids are only thread unique and not process unique.
// We create a thread specific hash table for the negative gids for each thread
// when <firstgid>_1.dat is read and then destroy it after <firstgid>_2.dat
// is finished using it.  An earlier implementation which attempted to
// encode the thread number into the negative gid
// (i.e -ith - nth*(type +1000*index)) failed due to not large enough
// integer domain size.
//
// <firstgid>_2.dat
// n_output n_real_output, nnode
// ndiam - 0 if no mechanism has dparam with diam semantics, or nnode
// nmech - includes artcell mechanisms
// for the nmech tml mechanisms
//   type, nodecount
// nidata, nvdata, nweight
// v_parent_index (nnode)
// actual_a, b, area, v (nnode)
// diam - if ndiam > 0. Note that only valid diam is for those nodes with diam semantics mechanisms
// for the nmech tml mechanisms
//   nodeindices (nodecount) but only if not an artificial cell
//   data (nodecount*param_size)
//   pdata (nodecount*dparam_size) but only if dparam_size > 0 on this side.
// output_vindex (n_presyn) >= 0 associated with voltages -(type+1000*index) for acell
// output_threshold (n_real_output)
// netcon_pnttype (nnetcon) <=0 if a NetCon does not have a target.
// netcon_pntindex (nnetcon)
// weights (nweight)
// delays (nnetcon)
// for the nmech tml mechanisms that have a nrn_bbcore_write method
//   type
//   icnt
//   dcnt
//   int array (number specified by the nodecount nrn_bbcore_write
//     to be intepreted by this side's nrn_bbcore_read method)
//   double array
// #VectorPlay_instances, for each of these instances
// 4 (VecPlayContinuousType)
// mtype
// index (from Memb_list.data)
// vecsize
// yvec
// tvec
//
// The critical issue requiring careful attention is that a coreneuron
// process reads many coreneuron thread files with a result that, although
// the conceptual
// total n_pre is the sum of all the n_presyn from each thread as is the
// total number of output_gid, the number of InputPreSyn instances must
// be computed here from a knowledge of all thread's netcon_srcgid after
// all thread's output_gids have been registered. We want to save the
// "individual allocation of many small objects" memory overhead by
// allocating a single InputPreSyn array for the entire process.
// For this reason cellgroup data are divided into two separate
// files with the first containing output_gids and netcon_srcgid which are
// stored in the nt.presyns array and nt.netcons array respectively
namespace coreneuron {
int nrn_setup_multiple = 1; /* default */
int nrn_setup_extracon = 0; /* default */
int is_training = 0;
int is_testing = 0;
int batch_size = 1;
int set_reset_v = 0;
static int maxgid;
static std::string global_datapath;
std::string input_filename;
std::string param_val_file;
// no gid in any file can be greater than maxgid. maxgid will be set so that
// maxgid * nrn_setup_multiple < 0x7fffffff

// nrn_setup_extracon extra connections per NrnThread.
// i.e. nrn_setup_extracon * nrn_setup_multiple * nrn_nthread
// extra connections on this process.
// The targets of the connections on a NrnThread are randomly selected
// (with replacement) from the set of ProbAMPANMDA_EMS on the thread.
// (This synapse type is not strictly appropriate to be used as
// a generalized synapse with multiple input streams since some of its
// range variables store quantities that should be stream specific
// and therefore should be stored in the NetCon weight vector. But it
// might be good enough for our purposes. In any case, we'd like to avoid
// creating new POINT_PROCESS instances with all the extra complexities
// involved in adjusting the data arrays.)
// The nrn_setup_extracon value is used to allocate the appropriae
// amount of extra space for NrnThread.netcons and NrnThread.weights
//
// The most difficult problem is to augment the rank wide inputpresyn_ list.
// We wish to randomly choose source gids for the extracon NetCons from the
// set of gids not in "multiple" instance of the model the NrnThread is a
// member of. We need to take into account the possibilty of multiple
// NrnThread in multiple "multiple" instances having extra NetCon with the
// same source gid. That some of the source gids may be already be
// associated with already existing PreSyn on this rank is a minor wrinkle.
// This is done between phase1 and phase2 during the call to
// determine_inputpresyn().

// If MATRIX_LAYOUT is 1 then a,b,d,rhs,v,area is not padded using NRN_SOA_PAD
// When MATRIX_LAYOUT is 0 then mechanism pdata index values into _actual_v
// and _actual_area data need to be updated.
#if !defined(LAYOUT)
#define LAYOUT 1
#define MATRIX_LAYOUT 1
#else
#define MATRIX_LAYOUT LAYOUT
#endif

#if !defined(NRN_SOA_PAD)
// for layout 0, every range variable array must have a size which
// is a multiple of NRN_SOA_PAD doubles
#define NRN_SOA_PAD 8
#endif
#if !defined(NRN_SOA_BYTE_ALIGN)
// for layout 0, every range variable array must be aligned by at least 16 bytes (the size of the
// simd memory bus)
#define NRN_SOA_BYTE_ALIGN (8 * sizeof(double))
#endif

#ifdef _OPENMP
static MUTDEC
#endif

    static size_t
    model_size(void);

/// Vector of maps for negative presyns
std::vector<std::map<int, PreSyn*> > neg_gid2out;
/// Maps for ouput and input presyns
std::map<int, PreSyn*> gid2out;
std::map<int, InputPreSyn*> gid2in;

/// InputPreSyn.nc_index_ to + InputPreSyn.nc_cnt_ give the NetCon*
std::vector<NetCon*> netcon_in_presyn_order_;

/// Only for setup vector of netcon source gids
std::vector<int*> netcon_srcgid;

/// Vector storing indexes (IDs) of different mechanisms of mod files between Neuron and CoreNeuron
extern std::vector<int> different_mechanism_type;

// Wrap read_phase1 and read_phase2 calls to allow using  nrn_multithread_job.
// Args marshaled by store_phase_args are used by phase1_wrapper
// and phase2_wrapper.
static void store_phase_args(int ngroup,
                             int* gidgroups,
                             int* imult,
                             FileHandler* file_reader,
                             const char* path,
                             const char* restore_path,
                             int byte_swap) {
    ngroup_w = ngroup;
    gidgroups_w = gidgroups;
    imult_w = imult;
    file_reader_w = file_reader;
    path_w = path;
    restore_path_w = restore_path;
    byte_swap_w = (bool)byte_swap;
}

/* read files.dat file and distribute cellgroups to all mpi ranks */
void nrn_read_filesdat(int& ngrp, int*& grp, int multiple, int*& imult, const char* filesdat) {
    patstimtype = nrn_get_mechtype("PatternStim");
    if (corenrn_embedded) {
        ngrp = corenrn_embedded_nthread;
        nrn_assert(multiple == 1);
        grp = new int[ngrp + 1];
        imult = new int[ngrp + 1];
        (*nrn2core_group_ids_)(grp);
        for (int i = 0; i <= ngrp; ++i) {
            imult[i] = 0;
        }
        return;
    }

    FILE* fp = fopen(filesdat, "r");

    if (!fp) {
        nrn_fatal_error("No input file with nrnthreads, exiting...");
    }

    char version[256];
    fscanf(fp, "%s\n", version);
    check_bbcore_write_version(version);

    int iNumFiles = 0;
    nrn_assert(fscanf(fp, "%d\n", &iNumFiles) == 1);

    // temporary strategem to figure out if model uses gap junctions while
    // being backward compatible
    if (iNumFiles == -1) {
        nrn_assert(fscanf(fp, "%d\n", &iNumFiles) == 1);
        nrn_have_gaps = 1;
        if (nrnmpi_myid == 0) {
            printf("Model uses gap junctions\n");
        }
    }

    if (nrnmpi_numprocs > iNumFiles && nrnmpi_myid == 0) {
        printf(
            "Info : The number of input datasets are less than ranks, some ranks will be idle!\n");
    }

    ngrp = 0;
    grp = new int[iNumFiles * multiple / nrnmpi_numprocs + 1];
    imult = new int[iNumFiles * multiple / nrnmpi_numprocs + 1];

    // irerate over gids in files.dat
    for (int iNum = 0; iNum < iNumFiles * multiple; ++iNum) {
        int iFile;

        nrn_assert(fscanf(fp, "%d\n", &iFile) == 1);
        if ((iNum % nrnmpi_numprocs) == nrnmpi_myid) {
            grp[ngrp] = iFile;
            imult[ngrp] = iNum / iNumFiles;
            ngrp++;
        }
        if ((iNum + 1) % iNumFiles == 0) {
            // re-read file for each multiple (skipping the two header lines)
            rewind(fp);
            fscanf(fp, "%*s\n");
            fscanf(fp, "%*d\n");
        }
    }

    fclose(fp);
}

static void read_phase1(int* output_gid, int imult, NrnThread& nt);

static void* direct_phase1(NrnThread* n) {
    NrnThread& nt = *n;
    int* output_gid;
    int valid =
        (*nrn2core_get_dat1_)(nt.id, nt.n_presyn, nt.n_netcon, output_gid, netcon_srcgid[nt.id]);
    if (valid) {
        read_phase1(output_gid, 0, nt);
    }
    return NULL;
}

void read_phase1(FileHandler& F, int imult, NrnThread& nt) {
    assert(!F.fail());
    nt.n_presyn = F.read_int();  /// Number of PreSyn-s in NrnThread nt
    nt.n_netcon = F.read_int();  /// Number of NetCon-s in NrnThread nt

    int* output_gid = F.read_array<int>(nt.n_presyn);
    // the extra netcon_srcgid will be filled in later
    netcon_srcgid[nt.id] = new int[nt.n_netcon + nrn_setup_extracon];
    F.read_array<int>(netcon_srcgid[nt.id], nt.n_netcon);
    F.close();

    read_phase1(output_gid, imult, nt);
}

static void read_phase1(int* output_gid, int imult, NrnThread& nt) {
    int zz = imult * maxgid;  // offset for each gid
    // offset the (non-negative) gids according to multiple
    // make sure everything fits into gid space.
    for (int i = 0; i < nt.n_presyn; ++i) {
        if (output_gid[i] >= 0) {
            nrn_assert(output_gid[i] < maxgid);
            output_gid[i] += zz;
        }
    }

    nt.presyns = new PreSyn[nt.n_presyn];
    nt.netcons = new NetCon[nt.n_netcon + nrn_setup_extracon];
    nt.presyns_helper = (PreSynHelper*)ecalloc(nt.n_presyn, sizeof(PreSynHelper));

    int* nc_srcgid = netcon_srcgid[nt.id];
    for (int i = 0; i < nt.n_netcon; ++i) {
        if (nc_srcgid[i] >= 0) {
            nrn_assert(nc_srcgid[i] < maxgid);
            nc_srcgid[i] += zz;
        }
    }

    for (int i = 0; i < nt.n_presyn; ++i) {
        int gid = output_gid[i];
        if (gid == -1)
            continue;

        // Note that the negative (type, index)
        // coded information goes into the neg_gid2out[tid] hash table.
        // See netpar.cpp for the netpar_tid_... function implementations.
        // Both that table and the process wide gid2out table can be deleted
        // before the end of setup

        MUTLOCK
        /// Put gid into the gid2out hash table with correspondent output PreSyn
        /// Or to the negative PreSyn map
        PreSyn* ps = nt.presyns + i;
        if (gid >= 0) {
            char m[200];
            if (gid2in.find(gid) != gid2in.end()) {
                sprintf(m, "gid=%d already exists as an input port", gid);
                hoc_execerror(
                    m,
                    "Setup all the output ports on this process before using them as input ports.");
            }
            if (gid2out.find(gid) != gid2out.end()) {
                sprintf(m, "gid=%d already exists on this process as an output port", gid);
                hoc_execerror(m, 0);
            }
            gid2out[gid] = ps;
            ps->gid_ = gid;
            ps->output_index_ = gid;
        } else {
            nrn_assert(neg_gid2out[nt.id].find(gid) == neg_gid2out[nt.id].end());
            neg_gid2out[nt.id][gid] = ps;
        }
        MUTUNLOCK

        if (gid < 0) {
            nt.presyns[i].output_index_ = -1;
        }
    }

    delete[] output_gid;

    if (nrn_setup_extracon > 0) {
        // very simplistic
        // Use this threads positive source gids - zz in nt.netcon order as the
        // source gids for extracon.
        // The edge cases are:
        // The 0th duplicate uses uses source gids for the last duplicate.
        // If there are fewer positive source gids than extracon, then keep
        // rotating through the nt.netcon .
        // If there are no positive source gids, use a source gid of -1.
        // Would not be difficult to modify so that random positive source was
        // used, and/or random connect to another duplicate.
        // Note that we increment the nt.n_netcon at the end of this function.
        int sidoffset;  // how much to increment the corresponding positive gid
        // like ring connectivity
        if (imult > 0) {
            sidoffset = -maxgid;
        } else if (nrn_setup_multiple > 1) {
            sidoffset = (nrn_setup_multiple - 1) * maxgid;
        } else {
            sidoffset = 0;
        }
        // set up the extracon srcgid_
        int* nc_srcgid = netcon_srcgid[nt.id];
        int j = 0;  // rotate through the n_netcon netcon_srcgid
        for (int i = 0; i < nrn_setup_extracon; ++i) {
            int sid = -1;
            for (int k = 0; k < nt.n_netcon; ++k) {
                // potentially rotate j through the entire n_netcon but no further
                sid = nc_srcgid[j];
                j = (j + 1) % nt.n_netcon;
                if (sid >= 0) {
                    break;
                }
            }
            if (sid < 0) {  // only connect to real cells.
                sid = -1;
            } else {
                sid += sidoffset;
            }
            nc_srcgid[i + nt.n_netcon] = sid;
        }
        // finally increment the n_netcon
        nt.n_netcon += nrn_setup_extracon;
    }
}

void netpar_tid_gid2ps(int tid, int gid, PreSyn** ps, InputPreSyn** psi) {
    /// for gid < 0 returns the PreSyn* in the thread (tid) specific map.
    *ps = NULL;
    *psi = NULL;
    std::map<int, PreSyn*>::iterator gid2out_it;
    if (gid >= 0) {
        gid2out_it = gid2out.find(gid);
        if (gid2out_it != gid2out.end()) {
            *ps = gid2out_it->second;
        } else {
            std::map<int, InputPreSyn*>::iterator gid2in_it;
            gid2in_it = gid2in.find(gid);
            if (gid2in_it != gid2in.end()) {
                *psi = gid2in_it->second;
            }
        }
    } else {
        gid2out_it = neg_gid2out[tid].find(gid);
        if (gid2out_it != neg_gid2out[tid].end()) {
            *ps = gid2out_it->second;
        }
    }
}

void determine_inputpresyn() {
    // allocate the process wide InputPreSyn array
    // all the output_gid have been registered and associated with PreSyn.
    // now count the needed InputPreSyn by filling the netpar::gid2in map
    gid2in.clear();

    // now have to fill the new table
    // do not need to worry about negative gid overlap since only use
    // it to search for PreSyn in this thread.

    std::vector<InputPreSyn*> inputpresyn_;
    std::map<int, PreSyn*>::iterator gid2out_it;
    std::map<int, InputPreSyn*>::iterator gid2in_it;

    for (int ith = 0; ith < nrn_nthread; ++ith) {
        NrnThread& nt = nrn_threads[ith];
        // associate gid with InputPreSyn and increase PreSyn and InputPreSyn count
        nt.n_input_presyn = 0;
        for (int i = 0; i < nt.n_netcon; ++i) {
            int gid = netcon_srcgid[ith][i];
            if (gid >= 0) {
                /// If PreSyn or InputPreSyn is already in the map
                gid2out_it = gid2out.find(gid);
                if (gid2out_it != gid2out.end()) {
                    /// Increase PreSyn count
                    ++gid2out_it->second->nc_cnt_;
                    continue;
                }
                gid2in_it = gid2in.find(gid);
                if (gid2in_it != gid2in.end()) {
                    /// Increase InputPreSyn count
                    ++gid2in_it->second->nc_cnt_;
                    continue;
                }

                /// Create InputPreSyn and increase its count
                InputPreSyn* psi = new InputPreSyn;
                ++psi->nc_cnt_;
                gid2in[gid] = psi;
                inputpresyn_.push_back(psi);
                ++nt.n_input_presyn;
            } else {
                gid2out_it = neg_gid2out[nt.id].find(gid);
                if (gid2out_it != neg_gid2out[nt.id].end()) {
                    /// Increase negative PreSyn count
                    ++gid2out_it->second->nc_cnt_;
                }
            }
        }
    }

    // now, we can opportunistically create the NetCon* pointer array
    // to save some memory overhead for
    // "large number of small array allocation" by
    // counting the number of NetCons each PreSyn and InputPreSyn point to.
    // Conceivably the nt.netcons could become a process global array
    // in which case the NetCon* pointer array could become an integer index
    // array. More speculatively, the index array could be eliminated itself
    // if the process global NetCon array were ordered properly but that
    // would interleave NetCon from different threads. Not a problem for
    // serial threads but the reordering would propagate to nt.pntprocs
    // if the NetCon data pointers are also replaced by integer indices.

    // First, allocate the pointer array.
    int n_nc = 0;
    for (int ith = 0; ith < nrn_nthread; ++ith) {
        n_nc += nrn_threads[ith].n_netcon;
    }
    netcon_in_presyn_order_.resize(n_nc);
    n_nc = 0;

    // fill the indices with the offset values and reset the nc_cnt_
    // such that we use the nc_cnt_ in the following loop to assign the NetCon
    // to the right place
    // for PreSyn
    int offset = 0;
    for (int ith = 0; ith < nrn_nthread; ++ith) {
        NrnThread& nt = nrn_threads[ith];
        for (int i = 0; i < nt.n_presyn; ++i) {
            PreSyn& ps = nt.presyns[i];
            ps.nc_index_ = offset;
            offset += ps.nc_cnt_;
            ps.nc_cnt_ = 0;
        }
    }
    // for InputPreSyn
    for (size_t i = 0; i < inputpresyn_.size(); ++i) {
        InputPreSyn* psi = inputpresyn_[i];
        psi->nc_index_ = offset;
        offset += psi->nc_cnt_;
        psi->nc_cnt_ = 0;
    }

    inputpresyn_.clear();

    // with gid to InputPreSyn and PreSyn maps we can setup the multisend
    // target lists.
    if (use_multisend_) {
#if NRN_MULTISEND
        nrn_multisend_setup();
#endif
    }

    // fill the netcon_in_presyn_order and recompute nc_cnt_
    // note that not all netcon_in_presyn will be filled if there are netcon
    // with no presyn (ie. netcon_srcgid[nt.id][i] = -1) but that is ok since they are
    // only used via ps.nc_index_ and ps.nc_cnt_;
    for (int ith = 0; ith < nrn_nthread; ++ith) {
        NrnThread& nt = nrn_threads[ith];
        for (int i = 0; i < nt.n_netcon; ++i) {
            NetCon* nc = nt.netcons + i;
            int gid = netcon_srcgid[ith][i];
            PreSyn* ps;
            InputPreSyn* psi;
            netpar_tid_gid2ps(ith, gid, &ps, &psi);
            if (ps) {
                netcon_in_presyn_order_[ps->nc_index_ + ps->nc_cnt_] = nc;
                ++ps->nc_cnt_;
                ++n_nc;
            } else if (psi) {
                netcon_in_presyn_order_[psi->nc_index_ + psi->nc_cnt_] = nc;
                ++psi->nc_cnt_;
                ++n_nc;
            }
        }
    }

    /// Resize the vector to its actual size of the netcons put in it
    netcon_in_presyn_order_.resize(n_nc);
}

void test_get_location()
{
    for (int i = 0; i < nrn_nthread; i++)
    {
        NrnThread *nt = nrn_threads + i;
        printf("%p\n", get_var_location_from_var_name(3, "i_cap", nt->tml->ml, 100));
    }
}

/// Clean up
void nrn_setup_cleanup() {
    for (int ith = 0; ith < nrn_nthread; ++ith) {
        if (netcon_srcgid[ith])
            delete[] netcon_srcgid[ith];
    }
    netcon_srcgid.clear();
    neg_gid2out.clear();
}

void str_split(std::string src, char c, std::string &s1, std::string &s2)
{
    int tar_pos = src.find(c);
	int len = src.length();
	s1.assign(src, 0, tar_pos);
	s2.assign(src, tar_pos + 1, len - tar_pos);
}

void gen_mapping(NrnThread &nt, const char* secname_file, const char* node2sec_file)
{
    int nsec, nnode;
    ThreadMapping *t_map = new ThreadMapping();
    std::ifstream f_secname, f_node2sec;

    f_secname.open(secname_file);
    f_secname >> nsec;
    std::string s;
    std::vector<std::string> vec_sec_name;
    printf("%s %s nsec:%d\n", secname_file, node2sec_file, nsec);
    for (int i = 0; i < nsec; i++)
    {
        f_secname >> s;
        //std::cout << "full sec:" << s << std::endl;
        vec_sec_name.push_back(s);
        std::string cell_name, sec_name;
        str_split(s, '.', cell_name, sec_name);
        //std::cout << "cell name:" << cell_name << " sec name:" << sec_name << std::endl;
        MyCellMapping* c_map = t_map->get_cell_map(cell_name);
        if (c_map == NULL)
        {
            MyCellMapping* cell_map = new MyCellMapping(cell_name);
            t_map->add_cell_mapping(cell_map);
        }
    }
    f_secname.close();

    f_node2sec.open(node2sec_file);
    f_node2sec >> nnode;
    int isec;
    printf("nnode:%d\n", nnode);
    for (int i = 0; i < nnode; i++)
    {
        f_node2sec >> isec;
        s = vec_sec_name[isec];
        std::string cell_name, sec_name;
        str_split(s, '.', cell_name, sec_name);
        MyCellMapping* c_map = NULL;
        c_map = t_map->get_cell_map(cell_name);
        assert(c_map != NULL);
        //printf("hassec:%s %d\n", sec_name.c_str(), c_map->has_section(sec_name));
        if (!c_map->has_section(sec_name))
            c_map->new_pair(sec_name);
        c_map->add_segment(sec_name, i);
    }
    f_node2sec.close();

    nt.mapping = (void*)t_map;

}

int get_nodeindex(NrnThread &nt, int mech_type, int node_id, int mech_order)
{
    Memb_list* ml = nt._ml_list[mech_type];
    int nodecount = ml->nodecount;
    int order = -1;
    //printf("c1:%d c2:%d c3:%d\n", c1, c2, c3);
    for (int i = 0; i < nodecount; i++)
    {
        if (ml->nodeindices[i] == node_id)
            order++;
        if (order == mech_order)
            return i;
    }
    return -1;
}

void fill_param_idx_ptrs(NrnThread &nt, 
                         std::string full_sec_name, 
                         float pos, 
                         std::string mod_name, 
                         int mech_order, 
                         std::string param_name, 
                         int* permute, 
                         bool* is_voltage, 
                         int* rec_mech_types,
                         int* rec_param_idx, 
                         double** rec_param_ptrs, 
                         int iparam)
{
    ThreadMapping *t_map = (ThreadMapping*)nt.mapping;
    std::string cell_name, sec_name;
    int rec_seg_id, mech_type;
    str_split(full_sec_name, '.', cell_name, sec_name);
    MyCellMapping *c_map = t_map->get_cell_map(cell_name);
    if (c_map == NULL)
        std::cout << "cell " << cell_name << "not exist" << std::endl;
        //printf("cell %s doesn't exist\n", cell_name.c_str());
    else if (!c_map->has_section(sec_name))
        std::cout << "section " << sec_name << "not exist" << std::endl;
        //printf("section %s doesn't exist\n", sec_name.c_str());
    else
    {
        rec_seg_id = c_map->get_segment(sec_name, pos);
        //printf("%d->", rec_seg_id);
        if (permute)
            rec_seg_id = permute[rec_seg_id];
        //printf("%d\n", rec_seg_id);
        if (param_name == "v")
        {
            if (is_voltage != NULL)
                is_voltage[iparam] = true;
            rec_param_idx[iparam] = rec_seg_id;
            rec_param_ptrs[iparam] = nt._actual_v + rec_seg_id;
        }
        else
        {
            if (is_voltage != NULL)
                is_voltage[iparam] = false;
            mech_type = nrn_get_mechtype(mod_name.c_str()); 
            int node_index = get_nodeindex(nt, mech_type, rec_seg_id, mech_order);
            //printf("mech_type:%d recseg:%d node_index:%d\n", mech_type, rec_seg_id, node_index);
            if (node_index <= -1)
            {
                std::cout << full_sec_name << pos << mod_name << param_name << std::endl;
            }
            assert(node_index > -1);
            if (mech_type == -1)
                std::cout << "mod " << mod_name << "doesn't exist" << std::endl;
                //printf("mod %s doesn't exist\n", mod_name.c_str());
            rec_mech_types[iparam] = mech_type;
            rec_param_idx[iparam] = get_var_index_from_var_name(mech_type, param_name.c_str(), nt._ml_list[mech_type], node_index);
            rec_param_ptrs[iparam] = get_var_location_from_var_name(mech_type, param_name.c_str(), nt._ml_list[mech_type], node_index);
        }
        
    }
}

void fill_param_idx_ptrs_art(NrnThread &nt,
                        std::string mod_name,
                        int mech_order,
                        std::string param_name,
                        int* rec_mech_types,
                        int* rec_param_idx,
                        double** rec_param_ptrs,
                        int iparam)
{
    int mech_type = nrn_get_mechtype(mod_name.c_str());
    if (mech_type == -1)
    {
        printf("mech %s not exist\n");
        exit(-1);
    }
    //printf("%s %d %s\n", mod_name.c_str(), mech_order, param_name.c_str());
    rec_mech_types[iparam] = mech_type;
    rec_param_idx[iparam] = get_var_index_from_var_name(mech_type, param_name.c_str(), nt._ml_list[mech_type], mech_order);
    rec_param_ptrs[iparam] = get_var_location_from_var_name(mech_type, param_name.c_str(), nt._ml_list[mech_type], mech_order);
}

               


void set_ptr_and_idx(const char* file, 
                    NrnThread &nt, 
                    int &len, 
                    int*& mech_types, 
                    int*& arr_idx, 
                    double**& arr_ptr,
                    int *permute)
{
    std::string full_sec_name, mod_name, param_name;
    std::ifstream f;
    float pos;
    int mech_order, is_art;
    
    f.open(file);
    f >> len;
    mech_types = new int[len];
    arr_idx = new int[len];
    arr_ptr = new double*[len];

    std::vector<std::string> vec_sec_name, vec_mod_name, vec_param_name;
    std::vector<float> vec_pos;
    std::vector<int> vec_mech_order, vec_is_art;
    vec_sec_name.resize(len);
    vec_mod_name.resize(len);
    vec_param_name.resize(len);
    vec_pos.resize(len);
    vec_mech_order.resize(len);
    vec_is_art.resize(len);

    for (int i = 0; i < len; i++)
    {
        f >> is_art;
        if (is_art == 1)
        {
            full_sec_name = "";
            pos = -1.0;
            f >> mod_name >> mech_order >> param_name;
        }
        else
        {
            f >> full_sec_name >> pos >> mod_name >> mech_order >> param_name;
        }
        vec_is_art[i] = is_art;
        vec_sec_name[i] = full_sec_name;
        vec_pos[i] = pos;
        vec_mod_name[i] = mod_name;
        vec_mech_order[i] = mech_order;
        vec_param_name[i] = param_name;
    }

    #pragma omp parallel for num_threads(32)
    for (int i = 0; i < len; i++)
    {
        if (vec_is_art[i] == 0)
        {
            fill_param_idx_ptrs(nt, vec_sec_name[i], vec_pos[i], vec_mod_name[i], vec_mech_order[i], vec_param_name[i], permute, NULL, mech_types, arr_idx, arr_ptr, i);
        }
        else
        {
            fill_param_idx_ptrs_art(nt, vec_mod_name[i], vec_mech_order[i], vec_param_name[i], mech_types, arr_idx, arr_ptr, i);
        }
    }
    f.close();
}

void set_param_val(NrnThread &nt, const char* secname_file, const char* node2sec_file, const char* param_file, int* permute)
{
    if (nt.mapping == NULL)
        gen_mapping(nt, secname_file, node2sec_file);
    int nparam, mech_order, is_art;
    char mod_name[50], param_name[20], full_sec_name[100];
    std::string str;
    float pos;
    double val;

    std::vector<std::string> vec_sec_name, vec_mod_name, vec_param_name;
    std::vector<float> vec_pos;
    std::vector<int> vec_mech_order, vec_is_art;
    std::vector<double> vec_val;
    
    FILE *fp = NULL;
    fp = fopen(param_file, "r");
    printf("param_file:%s\n", param_file);
    fscanf(fp, "%d", &nparam);
    vec_sec_name.resize(nparam);
    vec_mod_name.resize(nparam);
    vec_param_name.resize(nparam);
    vec_pos.resize(nparam);
    vec_mech_order.resize(nparam);
    vec_is_art.resize(nparam);
    vec_val.resize(nparam);
    for (int iparam = 0; iparam < nparam; iparam++)
    {
        fscanf(fp, "%d %s %f %s %d %s %lf", &is_art, full_sec_name, &pos, mod_name, &mech_order, param_name, &val);
        if (iparam < 50)
            printf("sec:%s pos:%f mod_name:%s mech_order:%d param:%s val:%f\n", full_sec_name, pos, mod_name, mech_order, param_name, val); 
        vec_is_art.push_back(is_art);
        str = full_sec_name;
        vec_sec_name[iparam] = str;
        vec_pos[iparam] = pos;
        str = mod_name;
        vec_mod_name[iparam] = str;
        vec_mech_order[iparam] = mech_order;
        str = param_name;
        vec_param_name[iparam] = str;
        vec_val[iparam] = val;
    }
    fclose(fp);

    int *mech_types, *arr_idx;
    double** arr_ptr;
    mech_types = new int[nparam];
    arr_idx = new int[nparam];
    arr_ptr = new double*[nparam];
    #pragma omp parallel for num_threads(32)
    for (int i = 0; i < nparam; i++)
    {
        fill_param_idx_ptrs(nt, vec_sec_name[i], vec_pos[i], vec_mod_name[i], vec_mech_order[i], vec_param_name[i], permute, NULL, mech_types, arr_idx, arr_ptr, i);
        *arr_ptr[i] = vec_val[i]; 
    }

    delete[] mech_types;
    delete[] arr_idx;
    delete[] arr_ptr;
}
 

void set_non_voltage_gap(NrnThread &nt, const char* secname_file, const char* node2sec_file, const char* gap_param_file, int* permute)
{
    if (nt.mapping == NULL)
        gen_mapping(nt, secname_file, node2sec_file);
    int nparam, src_mech_order, tgt_mech_order;
    float src_pos, tgt_pos;
    std::string src_full_sec_name, src_mod_name, src_param_name;
    std::string tgt_full_sec_name, tgt_mod_name, tgt_param_name;
    std::ifstream f_gap;

    f_gap.open(gap_param_file);
    std::cout << "file:" << gap_param_file << " open:" << f_gap.is_open() << std::endl;
    f_gap >> nparam;
    nt.nnon_v_gap = nparam;
    std::cout << "non voltage nparam: " << nparam << std::endl;

    nt.gap_src_mech_types = new int[nparam];
    nt.gap_tgt_mech_types = new int[nparam];
    nt.gap_src_param_idx = new int[nparam];
    nt.gap_tgt_param_idx = new int[nparam];
    nt.gap_src_param_ptrs = new double*[nparam];
    nt.gap_tgt_param_ptrs = new double*[nparam];
    nt.gap_src_is_v = new bool[nparam];
    nt.gap_tgt_is_v = new bool[nparam];

    std::vector<std::string> vec_src_sec_name, vec_src_mod_name, vec_src_param_name;
    std::vector<std::string> vec_tgt_sec_name, vec_tgt_mod_name, vec_tgt_param_name;
    std::vector<float> vec_src_pos, vec_tgt_pos;
    std::vector<int> vec_src_mech_order, vec_tgt_mech_order;
    vec_src_sec_name.resize(nparam);
    vec_src_mod_name.resize(nparam);
    vec_src_param_name.resize(nparam);
    vec_tgt_sec_name.resize(nparam);
    vec_tgt_mod_name.resize(nparam);
    vec_tgt_param_name.resize(nparam);
    vec_src_pos.resize(nparam);
    vec_tgt_pos.resize(nparam);
    vec_src_mech_order.resize(nparam);
    vec_tgt_mech_order.resize(nparam);
    for (int iparam = 0; iparam < nparam; iparam++)
    {
        f_gap >> src_full_sec_name >> src_pos >> src_mod_name >> src_mech_order >> src_param_name;
        vec_src_sec_name[iparam] = src_full_sec_name;
        vec_src_pos[iparam] = src_pos;
        vec_src_mod_name[iparam] = src_mod_name;
        vec_src_mech_order[iparam] = src_mech_order;
        vec_src_param_name[iparam] = src_param_name;

        f_gap >> tgt_full_sec_name >> tgt_pos >> tgt_mod_name >> tgt_mech_order >> tgt_param_name;
        vec_tgt_sec_name[iparam] = tgt_full_sec_name;
        vec_tgt_pos[iparam] = tgt_pos;
        vec_tgt_mod_name[iparam] = tgt_mod_name;
        vec_tgt_mech_order[iparam] = tgt_mech_order;
        vec_tgt_param_name[iparam] = tgt_param_name;
    }


    //Memb_list *ml = nt._ml_list[24];
    //for (size_t i = 0; i < ml->nodecount; i++)
    //{
    //    if (ml->nodeindices[i] == permute[858])
    //        printf("%d src:858 permute:%d\n", i, ml->nodeindices[i]);
    //    if (ml->nodeindices[i] == permute[2575])
    //        printf("%d src:2575 permute:%d\n", i, ml->nodeindices[i]);
    //    if (ml->nodeindices[i] == permute[2576])
    //        printf("%d src:2576 permute:%d\n", i, ml->nodeindices[i]);
    //}

    #pragma omp parallel for num_threads(64) 
    for (int iparam = 0; iparam < nparam; iparam++)
    {
        //f_gap >> src_full_sec_name >> src_pos >> src_mod_name >> src_mech_order >> src_param_name;
        fill_param_idx_ptrs(nt, vec_src_sec_name[iparam], vec_src_pos[iparam], vec_src_mod_name[iparam], vec_src_mech_order[iparam], vec_src_param_name[iparam], permute, nt.gap_src_is_v, nt.gap_src_mech_types, nt.gap_src_param_idx, nt.gap_src_param_ptrs, iparam);
        //f_gap >> tgt_full_sec_name >> tgt_pos >> tgt_mod_name >> tgt_mech_order >> tgt_param_name;
        fill_param_idx_ptrs(nt, vec_tgt_sec_name[iparam], vec_tgt_pos[iparam], vec_tgt_mod_name[iparam], vec_tgt_mech_order[iparam], vec_tgt_param_name[iparam], permute, nt.gap_tgt_is_v, nt.gap_tgt_mech_types, nt.gap_tgt_param_idx, nt.gap_tgt_param_ptrs, iparam);
    }
    f_gap.close();

}

void set_records(NrnThread &nt, const char* secname_file, const char* node2sec_file, const char* record_param_file, int* permute)
{

    int nparam, nseg, iseg, rec_seg_id, mech_type, ntime, mech_id;
    float pos;
    std::string full_sec_name, mod_name, param_name;
    std::ifstream f_rec;
    
    f_rec.open(record_param_file);
    if (!f_rec.good())
        return;
    if (nt.mapping == NULL)
        gen_mapping(nt, secname_file, node2sec_file);
    f_rec >> nparam >> ntime;
    printf("nparam:%d\n", nparam);
    nt.nrec_param = nparam;
    if (dt_io < dt)
    {
        printf("WARNING: dt_io is smaller than dt, set it to dt\n");
        dt_io = dt;
    }
    nt.rec_stride_len = (size_t)(dt_io / dt);
    //size_t nstep = (size_t)(tstop / dt) / 40;
    size_t nstep = (size_t)(tstop / dt_io);
    //size_t nstep = (size_t)(150 / dt);
    if (ntime == 0)
    {
        nt.rec_len = 1342177280 / nparam; // 5GB for record
        //nt.rec_len = 1073741824 / nparam;
        //nt.rec_len = 2621440 / nparam;
        nt.rec_len = nt.rec_len < nstep ? nt.rec_len : nstep;
    }
    else if (ntime == -1)
    {
        f_rec >> nt.mean_s_time >> nt.mean_e_time;
        nt.rec_len = 1;
        nt.rec_type = -1;
    }
    else
    {
        nt.rec_times = new float[ntime];
        for (int i = 0; i < ntime; i++)
        {
            f_rec >> nt.rec_times[i];
        }
        nt.rec_len = ntime;
        nt.rec_type = 1;
    }
    
    printf("rec_len:%d\n", nt.rec_len);
    nt.rec_mech_types = new int[nparam];
    nt.rec_param_index = new int[nparam];
    nt.rec_ptrs = new double*[nparam];
    nt.rec_vals = new float[nparam * nt.rec_len];
    nt.is_rec_v = new bool[nparam];
    ThreadMapping *t_map = (ThreadMapping*)nt.mapping;
    int is_art;
    for (int i = 0; i < nparam; i++)
    {
        nt.is_rec_v[i] = false;
        f_rec >> is_art;
        if (is_art == 1)
        {
            full_sec_name = "";
            pos = -1.0;
            f_rec >> mod_name >> mech_id >> param_name;
            fill_param_idx_ptrs_art(nt, mod_name, mech_id, param_name, nt.rec_mech_types, nt.rec_param_index, nt.rec_ptrs, i);
        }
        else
        {
            f_rec >> full_sec_name >> pos >> mod_name >> mech_id >> param_name;
            if (param_name == "v")
                nt.is_rec_v[i] = true;
            fill_param_idx_ptrs(nt, full_sec_name, pos, mod_name, mech_id, param_name, nt._permute, 
                                nt.is_rec_v, nt.rec_mech_types, nt.rec_param_index, nt.rec_ptrs, i);
        }
        
        
    }
    f_rec.close();

    size_t nrec, nlast;
    if (nt.rec_times == NULL)
    {
        nrec = nstep / nt.rec_len;
        nlast = nstep % nt.rec_len;
        nt.rec_last_len = nlast;
        printf("nrec:%d rec_len:%d nlast:%d\n", nrec, nt.rec_len, nlast);
    }
    else
    {
        nrec = 1;
        nlast = 0;
    }
    if (nt.fp_rec)
        fprintf(nt.fp_rec, "%d %d %d %d\n", nparam, nt.rec_len, nrec, nlast);

}

void nrn_setup(const char* filesdat,
               bool is_mapping_needed,
               int byte_swap,
               bool run_setup_cleanup) {
    /// Number of local cell groups
    int ngroup = 0;

    /// Array of cell group numbers (indices)
    int* gidgroups = NULL;

    /// Array of duplicate indices. Normally, with nrn_setup_multiple=1,
    //   they are ngroup values of 0.
    int* imult = NULL;

    double time = nrn_wtime();

    maxgid = 0x7fffffff / nrn_setup_multiple;
    nrn_read_filesdat(ngroup, gidgroups, nrn_setup_multiple, imult, filesdat);

    MUTCONSTRUCT(1)
    // temporary bug work around. If any process has multiple threads, no
    // process can have a single thread. So, for now, if one thread, make two.
    // Fortunately, empty threads work fine.
    // Allocate NrnThread* nrn_threads of size ngroup (minimum 2)
    // Note that rank with 0 dataset/cellgroup works fine
    nrn_threads_create(ngroup <= 1 ? 2 : ngroup);

#if 1 || CHKPNTDEBUG  // only required for NrnThreadChkpnt.file_id
    nrnthread_chkpnt = new NrnThreadChkpnt[nrn_nthread];
#endif

    if (nrn_nthread > 1) {
        // NetCvode construction assumed one thread. Need nrn_nthread instances
        // of NetCvodeThreadData. Here since possible checkpoint restore of
        // tqueue at end of phase2.
        nrn_p_construct();
    }

    if (use_solve_interleave) {
        create_interleave_info();
    }

    /// Reserve vector of maps of size ngroup for negative gid-s
    /// std::vector< std::map<int, PreSyn*> > neg_gid2out;
    neg_gid2out.resize(ngroup);

    // bug fix. gid2out is cumulative over all threads and so do not
    // know how many there are til after phase1
    // A process's complete set of output gids and allocation of each thread's
    // nt.presyns and nt.netcons arrays.
    // Generates the gid2out map which is needed
    // to later count the required number of InputPreSyn
    /// gid2out - map of output presyn-s
    /// std::map<int, PreSyn*> gid2out;
    gid2out.clear();

    netcon_srcgid.resize(nrn_nthread);
    for (int i = 0; i < nrn_nthread; ++i)
        netcon_srcgid[i] = NULL;

    FileHandler* file_reader = new FileHandler[ngroup];

    std::string datapath = nrnopt_get_str("--datpath");
    std::string restore_path = nrnopt_get_str("--restore");
    input_filename = nrnopt_get_str("--input-file");
    is_training = nrnopt_get_int("--training");
    is_testing = nrnopt_get_int("--testing");
    batch_size = nrnopt_get_int("--batchsize");
    param_val_file = nrnopt_get_str("--param-val");
    if (is_training && is_testing)
    {
        printf("Error! Training and Testing are both set to 1, changed them to 0\n");
        is_training = 0;
        is_testing = 0;
    }

    global_datapath = datapath;

    // if not restoring then phase2 files will be read from dataset directory
    if (!restore_path.length()) {
        restore_path = datapath;
    }

    /* nrn_multithread_job supports serial, pthread, and openmp. */
    store_phase_args(ngroup, gidgroups, imult, file_reader, datapath.c_str(), restore_path.c_str(),
                     byte_swap);

    // gap junctions
    if (nrn_have_gaps) {
        assert(nrn_setup_multiple == 1);
        nrn_partrans::transfer_thread_data_ = new nrn_partrans::TransferThreadData[nrn_nthread];
        nrn_partrans::setup_info_ = new nrn_partrans::SetupInfo[ngroup];
        if (!corenrn_embedded) {
            coreneuron::phase_wrapper<coreneuron::gap>();
        } else {
            nrn_assert(sizeof(nrn_partrans::sgid_t) == sizeof(int));
            for (int i = 0; i < ngroup; ++i) {
                nrn_partrans::SetupInfo& si = nrn_partrans::setup_info_[i];
                (*nrn2core_get_partrans_setup_info_)(i, si.ntar, si.nsrc, si.type, si.ix_vpre,
                                                     si.sid_target, si.sid_src, si.v_indices);
            }
        }
        nrn_partrans::gap_mpi_setup(ngroup);
    }

    omp_set_nested(true);

    if (!corenrn_embedded) {
        coreneuron::phase_wrapper<(coreneuron::phase)1>();  /// If not the xlc compiler, it should
                                                            /// be coreneuron::phase::one
    } else {
        nrn_multithread_job(direct_phase1);
    }


    // from the gid2out map and the netcon_srcgid array,
    // fill the gid2in, and from the number of entries,
    // allocate the process wide InputPreSyn array
    determine_inputpresyn();

    // read the rest of the gidgroup's data and complete the setup for each
    // thread.
    /* nrn_multithread_job supports serial, pthread, and openmp. */
    coreneuron::phase_wrapper<(coreneuron::phase)2>(corenrn_embedded);

    //for (int i = 0; i < nrn_nthread; i++)
    //    printf("i:%d thread:%p\n", i, nrn_threads + i);
    

    if (is_mapping_needed)
        coreneuron::phase_wrapper<(coreneuron::phase)3>();

    double mindelay = set_mindelay(nrnopt_get_dbl("--mindelay"));
    nrnopt_modify_dbl("--mindelay", mindelay);

    if (run_setup_cleanup)  // if run_setup_cleanup==false, user must call nrn_setup_cleanup() later
        nrn_setup_cleanup();

#if INTERLEAVE_DEBUG
    mk_cell_indices();
#endif

    /// Generally, tables depend on a few parameters. And if those parameters change,
    /// then the table needs to be recomputed. This is obviously important in NEURON
    /// since the user can change those parameters at any time. However, there is no
    /// c example for CoreNEURON so can't see what it looks like in that context.
    /// Boils down to setting up a function pointer of the function _check_table_thread(),
    /// which is only executed by StochKV.c.
    nrn_mk_table_check();  // was done in nrn_thread_memblist_setup in multicore.c

    delete[] file_reader;

    model_size();
    delete[] gidgroups;

    


    if (nrnmpi_myid == 0) {
        printf(" Setup Done   : %.2lf seconds \n", nrn_wtime() - time);
    }
}

void setup_ThreadData(NrnThread& nt) {
    for (NrnThreadMembList* tml = nt.tml; tml; tml = tml->next) {
        Memb_func& mf = memb_func[tml->index];
        Memb_list* ml = tml->ml;
        if (mf.thread_size_) {
            ml->_thread = (ThreadDatum*)ecalloc(mf.thread_size_, sizeof(ThreadDatum));
            if (mf.thread_mem_init_) {
                MUTLOCK (*mf.thread_mem_init_)(ml->_thread);
                MUTUNLOCK
            }
        } else {
            ml->_thread = NULL;
        }
    }
}

void read_phasegap(FileHandler& F, int imult, NrnThread& nt) {
    nrn_assert(imult == 0);
    nrn_partrans::SetupInfo& si = nrn_partrans::setup_info_[nt.id];
    si.ntar = 0;
    si.nsrc = 0;

    if (F.fail()) {
        return;
    }

    int chkpntsave = F.checkpoint();
    F.checkpoint(0);

    si.ntar = F.read_int();
    si.nsrc = F.read_int();
    si.type = F.read_int();
    si.ix_vpre = F.read_int();
    si.sid_target = F.read_array<int>(si.ntar);
    si.sid_src = F.read_array<int>(si.nsrc);
    si.v_indices = F.read_array<int>(si.nsrc);

    F.checkpoint(chkpntsave);
    printf("nsrc:%d ntar:%d type:%d ix_vpre:%d\n", si.nsrc, si.ntar, si.type, si.ix_vpre);

#if 0
  printf("%d read_phasegap tid=%d type=%d %s ix_vpre=%d nsrc=%d ntar=%d\n",
    nrnmpi_myid, nt.id, si.type, memb_func[si.type].sym, si.ix_vpre,
    si.nsrc, si.ntar);
  for (int i=0; i < si.nsrc; ++i) {
    printf("sid_src %d %d\n", si.sid_src[i], si.v_indices[i]);
  }
  for (int i=0; i <si. ntar; ++i) {
    if (i >= 151178)
        printf("sid_tar %d %d\n", si.sid_target[i], i);
  }
#endif
}

int nrn_soa_padded_size(int cnt, int layout) {
    return soa_padded_size<NRN_SOA_PAD>(cnt, layout);
}

static size_t nrn_soa_byte_align(size_t i) {
    if (LAYOUT == 0) {
        size_t dbl_align = NRN_SOA_BYTE_ALIGN / sizeof(double);
        size_t rem = i % dbl_align;
        if (rem) {
            i += dbl_align - rem;
        }
        assert((i * sizeof(double)) % NRN_SOA_BYTE_ALIGN == 0);
    }
    return i;
}

// file data is AoS. ie.
// organized as cnt array instances of mtype each of size sz.
// So input index i refers to i_instance*sz + i_item offset
// Return the corresponding SoA index -- taking into account the
// alignment requirements. Ie. i_instance + i_item*align_cnt.

int nrn_param_layout(int i, int mtype, Memb_list* ml) {
    int layout = nrn_mech_data_layout_[mtype];
    if (layout == 1) {
        return i;
    }
    assert(layout == 0);
    int sz = nrn_prop_param_size_[mtype];
    int cnt = ml->nodecount;
    int i_cnt = i / sz;
    int i_sz = i % sz;
    return nrn_i_layout(i_cnt, cnt, i_sz, sz, layout);
}

int nrn_i_layout(int icnt, int cnt, int isz, int sz, int layout) {
    if (layout == 1) {
        return icnt * sz + isz;
    } else if (layout == 0) {
        int padded_cnt = nrn_soa_padded_size(cnt, layout);  // may want to factor out to save time
        return icnt + isz * padded_cnt;
    }
    assert(0);
    return 0;
}

// from i to (icnt, isz)
void nrn_inverse_i_layout(int i, int& icnt, int cnt, int& isz, int sz, int layout) {
    if (layout == 1) {
        icnt = i / sz;
        isz = i % sz;
    } else if (layout == 0) {
        int padded_cnt = nrn_soa_padded_size(cnt, layout);
        icnt = i % padded_cnt;
        isz = i / padded_cnt;
    } else {
        assert(0);
    }
}

template <typename T>
inline void mech_layout(FileHandler& F, T* data, int cnt, int sz, int layout) {
    if (layout == 1) { /* AoS */
        if (corenrn_embedded) {
            return;
        }
        F.read_array<T>(data, cnt * sz);
    } else if (layout == 0) { /* SoA */
        int align_cnt = nrn_soa_padded_size(cnt, layout);
        T* d;
        if (corenrn_embedded) {
            d = new T[cnt * sz];
            for (int i = 0; i < cnt; ++i) {
                for (int j = 0; j < sz; ++j) {
                    d[i * sz + j] = data[i * sz + j];
                }
            }
        } else {
            d = F.read_array<T>(cnt * sz);
        }
        for (int i = 0; i < cnt; ++i) {
            for (int j = 0; j < sz; ++j) {
                data[i + j * align_cnt] = d[i * sz + j];
            }
        }
        delete[] d;
    }
}

/* nrn_threads_free() presumes all NrnThread and NrnThreadMembList data is
 * allocated with malloc(). This is not the case here, so let's try and fix
 * things up first. */

void nrn_cleanup(bool clean_ion_global_map) {
    gid2in.clear();
    gid2out.clear();

    // clean ezOpt parser allocated memory (if any)
    nrnopt_delete();

    // clean ions global maps
    if (clean_ion_global_map) {
        for (int i = 0; i < nrn_ion_global_map_size; i++)
            free(nrn_ion_global_map[i]);
        free(nrn_ion_global_map);
        nrn_ion_global_map = NULL;
        nrn_ion_global_map_size = 0;
    }

    // clean NrnThreads
    for (int it = 0; it < nrn_nthread; ++it) {
        NrnThread* nt = nrn_threads + it;
        NrnThreadMembList* next_tml = NULL;
        for (NrnThreadMembList* tml = nt->tml; tml; tml = next_tml) {
            Memb_list* ml = tml->ml;

            ml->data = NULL;  // this was pointing into memory owned by nt
            free(ml->pdata);
            ml->pdata = NULL;
            free(ml->nodeindices);
            ml->nodeindices = NULL;
            if (ml->_permute) {
                delete[] ml->_permute;
                ml->_permute = NULL;
            }

            if (ml->_thread) {
                free(ml->_thread);
                ml->_thread = NULL;
            }

            NetReceiveBuffer_t* nrb = ml->_net_receive_buffer;
            if (nrb) {
                if (nrb->_size) {
                    free(nrb->_pnt_index);
                    free(nrb->_weight_index);
                    free(nrb->_nrb_t);
                    free(nrb->_nrb_flag);
                    free(nrb->_displ);
                    free(nrb->_nrb_index);
                }
                free(nrb);
            }

            NetSendBuffer_t* nsb = ml->_net_send_buffer;
            if (nsb) {
                if (nsb->_size) {
                    free(nsb->_sendtype);
                    free(nsb->_vdata_index);
                    free(nsb->_pnt_index);
                    free(nsb->_weight_index);
                    free(nsb->_nsb_t);
                    free(nsb->_nsb_flag);
                }
                free(nsb);
            }

            if (tml->dependencies)
                free(tml->dependencies);

            next_tml = tml->next;
            free(tml->ml);
            free(tml);
        }

        nt->_actual_rhs = NULL;
        nt->_actual_d = NULL;
        nt->_actual_a = NULL;
        nt->_actual_b = NULL;

        free(nt->_v_parent_index);
        nt->_v_parent_index = NULL;

        free(nt->_data);
        nt->_data = NULL;

        free(nt->_idata);
        nt->_idata = NULL;

        free(nt->_vdata);
        nt->_vdata = NULL;

        if (nt->_permute) {
            delete[] nt->_permute;
            nt->_permute = NULL;
        }

        if (nt->presyns_helper) {
            free(nt->presyns_helper);
            nt->presyns_helper = NULL;
        }

        if (nt->pntprocs) {
            delete[] nt->pntprocs;
            nt->pntprocs = NULL;
        }

        if (nt->presyns) {
            delete[] nt->presyns;
            nt->presyns = NULL;
        }

        if (nt->pnt2presyn_ix) {
            for (int i = 0; i < nrn_has_net_event_cnt_; ++i) {
                if (nt->pnt2presyn_ix[i]) {
                    free(nt->pnt2presyn_ix[i]);
                }
            }
            free(nt->pnt2presyn_ix);
        }

        if (nt->netcons) {
            delete[] nt->netcons;
            nt->netcons = NULL;
        }

        if (nt->weights) {
            delete[] nt->weights;
            nt->weights = NULL;
        }

        if (nt->_shadow_rhs) {
            free(nt->_shadow_rhs);
            nt->_shadow_rhs = NULL;
        }

        if (nt->_shadow_d) {
            free(nt->_shadow_d);
            nt->_shadow_d = NULL;
        }

        if (nt->_net_send_buffer_size) {
            free(nt->_net_send_buffer);
            nt->_net_send_buffer = NULL;
            nt->_net_send_buffer_size = 0;
        }

        if (nt->_watch_types) {
            free(nt->_watch_types);
            nt->_watch_types = NULL;
        }

        // mapping information is available only for non-empty NrnThread
        if (nt->mapping && nt->ncell) {
            //delete ((NrnThreadMappingInfo*)nt->mapping);
            delete ((ThreadMapping*)nt->mapping);
        }

        free(nt->_ml_list);

        if (nt->fp)
            fclose(nt->fp);
        if (nt->rec_mech_types) {
            delete[] nt->rec_mech_types;
            nt->rec_mech_types = NULL;
        }
        if (nt->rec_param_index) {
            delete[] nt->rec_param_index;
            nt->rec_param_index = NULL;
        }
        if (nt->rec_ptrs) {
            delete[] nt->rec_ptrs;
            nt->rec_ptrs = NULL;
        }
        if (nt->rec_vals) {
            delete[] nt->rec_vals;
            nt->rec_vals = NULL;
        }
        if (nt->rec_times) {
            delete[] nt->rec_times;
            nt->rec_times = NULL;
        }
        if (nt->is_rec_v) {
            delete[] nt->is_rec_v;
            nt->is_rec_v = NULL;
        }
        if (nt->gap_src_mech_types) {
            delete[] nt->gap_src_mech_types;
            nt->gap_src_mech_types = NULL;
        }
        if (nt->gap_tgt_mech_types) {
            delete[] nt->gap_tgt_mech_types;
            nt->gap_tgt_mech_types = NULL;
        }
        if (nt->gap_src_param_idx) {
            delete[] nt->gap_src_param_idx;
            nt->gap_src_param_idx = NULL;
        }
        if (nt->gap_tgt_param_idx) {
            delete[] nt->gap_tgt_param_idx;
            nt->gap_tgt_param_idx = NULL;
        }
        if (nt->gap_src_param_ptrs) {
            delete[] nt->gap_src_param_ptrs;
            nt->gap_src_param_ptrs = NULL;
        }
        if (nt->gap_tgt_param_ptrs) {
            delete[] nt->gap_tgt_param_ptrs;
            nt->gap_tgt_param_ptrs = NULL;
        }

        if (nt->in2hd_mech_types) {
            delete[] nt->in2hd_mech_types;
            nt->in2hd_mech_types = NULL;
        }
        if (nt->in2hd_idx) {
            delete[] nt->in2hd_idx;
            nt->in2hd_idx = NULL;
        }
        if (nt->in2hd_ptrs) {
            delete[] nt->in2hd_ptrs;
            nt->in2hd_ptrs = NULL;
        }
        if (nt->hd2out_mech_types) {
            delete[] nt->hd2out_mech_types;
            nt->hd2out_mech_types = NULL;
        }
        if (nt->hd2out_idx) {
            delete[] nt->hd2out_idx;
            nt->hd2out_idx = NULL;
        }
        if (nt->hd2out_ptrs) {
            delete[] nt->hd2out_ptrs;
            nt->hd2out_ptrs = NULL;
        }
        if (nt->stim_mech_types) {
            delete[] nt->stim_mech_types;
            nt->stim_mech_types = NULL;
        }
        if (nt->stim_idx) {
            delete[] nt->stim_idx;
            nt->stim_idx = NULL;
        }
        if (nt->stim_ptrs) {
            delete[] nt->stim_ptrs;
            nt->stim_ptrs = NULL;
        }
        if (nt->imgs) {
            delete[] nt->imgs;
            nt->imgs = NULL;
        }
        if (nt->labels) {
            delete[] nt->labels;
            nt->labels = NULL;
        }
        if (nt->w_mech_types) {
            delete[] nt->w_mech_types;
            nt->w_mech_types = NULL;
        }
        if (nt->w_idx) {
            delete[] nt->w_idx;
            nt->w_idx = NULL;
        }
        if (nt->w_ptrs) {
            delete[] nt->w_ptrs;
            nt->w_ptrs = NULL;
        }
        if (nt->dw_mech_types) {
            delete[] nt->dw_mech_types;
            nt->dw_mech_types = NULL;
        }
        if (nt->dw_idx) {
            delete[] nt->dw_idx;
            nt->dw_idx = NULL;
        }
        if (nt->dw_ptrs) {
            delete[] nt->dw_ptrs;
            nt->dw_ptrs = NULL;
        }

        if (nt->in2hd_g_mech_types) {
            delete[] nt->in2hd_g_mech_types;
            nt->in2hd_g_mech_types = NULL;
        }
        if (nt->in2hd_g_idx) {
            delete[] nt->in2hd_g_idx;
            nt->in2hd_g_idx = NULL;
        }
        if (nt->in2hd_g_ptrs) {
            delete[] nt->in2hd_g_ptrs;
            nt->in2hd_g_ptrs = NULL;
        }
        if (nt->weights_buffer) {
            delete[] nt->weights_buffer;
            nt->weights_buffer = NULL;
        }
        if (nt->resetv_each_cell) {
            delete[] nt->resetv_each_cell;
            nt->resetv_each_cell = NULL;
        }
        if (nt->resetv_each_node) {
            delete[] nt->resetv_each_node;
            nt->resetv_each_node = NULL;
        }
        if (nt->fp_rec)
            fclose(nt->fp_rec);
    }

#if NRN_MULTISEND
    nrn_multisend_cleanup();
#endif

    netcon_in_presyn_order_.clear();

    nrn_threads_free();

    if (pnttype2presyn) {
        free(pnttype2presyn);
    }
}

void read_phase2(FileHandler& F, int imult, NrnThread& nt) {
    bool direct = corenrn_embedded ? true : false;
    if (!direct) {
        assert(!F.fail());  // actually should assert that it is open
    }
    nrn_assert(imult >= 0);  // avoid imult unused warning
#if 1 || CHKPNTDEBUG
    NrnThreadChkpnt& ntc = nrnthread_chkpnt[nt.id];
    ntc.file_id = gidgroups_w[nt.id];
#endif
    NrnThreadMembList* tml;

    int n_outputgid, ndiam, nmech, *tml_index, *ml_nodecount;
    if (direct) {
        int nidata, nvdata;
        (*nrn2core_get_dat2_1_)(nt.id, n_outputgid, nt.ncell, nt.end, ndiam, nmech, tml_index,
                                ml_nodecount, nidata, nvdata, nt.n_weight);
        nt._nidata = nidata;
        nt._nvdata = nvdata;
    } else {
        n_outputgid = F.read_int();
        nt.ncell = F.read_int();
        nt.end = F.read_int();
        ndiam = F.read_int();  // 0 if not needed, else nt.end
        nmech = F.read_int();
        tml_index = new int[nmech];
        ml_nodecount = new int[nmech];
        int diff_mech_count = 0;
        for (int i = 0; i < nmech; ++i) {
            tml_index[i] = F.read_int();
            ml_nodecount[i] = F.read_int();
            auto mechanism_differs =
                std::find(different_mechanism_type.begin(), different_mechanism_type.end(),
                          tml_index[i]) != different_mechanism_type.end();
            if (mechanism_differs) {
                if (nrnmpi_myid == 0) {
                    printf("Error: %s is a different MOD file than used by NEURON!\n",
                           nrn_get_mechname(tml_index[i]));
                }
                diff_mech_count++;
            }
        }

        if (diff_mech_count > 0) {
            if (nrnmpi_myid == 0) {
                printf(
                    "Error : NEURON and CoreNEURON must use same mod files for compatibility, %d different mod file(s) found. Re-compile special and special-core!\n",
                    diff_mech_count);
            }
            nrn_abort(1);
        }

        nt._nidata = F.read_int();
        nt._nvdata = F.read_int();
        nt.n_weight = F.read_int();
    }

#if CHKPNTDEBUG
    ntc.n_outputgids = n_outputgid;
    ntc.nmech = nmech;
#endif
    nrn_assert(n_outputgid > 0);  // avoid n_outputgid unused warning

    /// Checkpoint in coreneuron is defined for both phase 1 and phase 2 since they are written
    /// together
    // printf("ncell=%d end=%d nmech=%d\n", nt.ncell, nt.end, nmech);
    // printf("nart=%d\n", nart);
    NrnThreadMembList* tml_last = NULL;
    nt._ml_list = (Memb_list**)ecalloc(n_memb_func, sizeof(Memb_list*));

#if CHKPNTDEBUG
    ntc.mlmap = new Memb_list_chkpnt*[n_memb_func];
    for (int i = 0; i < n_memb_func; ++i) {
        ntc.mlmap[i] = NULL;
    }
#endif

    int shadow_rhs_cnt = 0;
    nt.shadow_rhs_cnt = 0;

    nt.stream_id = 0;
    nt.compute_gpu = 0;

/* read_phase2 is being called from openmp region
 * and hence we can set the stream equal to current thread id.
 * In fact we could set gid as stream_id when we will have nrn threads
 * greater than number of omp threads.
 */
#if defined(_OPENMP)
    nt.stream_id = omp_get_thread_num();
#endif

    for (int i = 0; i < nmech; ++i) {
        tml = (NrnThreadMembList*)emalloc(sizeof(NrnThreadMembList));
        tml->ml = (Memb_list*)ecalloc(1, sizeof(Memb_list));
        tml->ml->_net_receive_buffer = NULL;
        tml->ml->_net_send_buffer = NULL;
        tml->ml->_permute = NULL;
        tml->next = NULL;
        tml->index = tml_index[i];
        if (memb_func[tml->index].alloc == NULL) {
            hoc_execerror(memb_func[tml->index].sym, "mechanism does not exist");
        }
        tml->ml->nodecount = ml_nodecount[i];
        if (!memb_func[tml->index].sym) {
            printf("%s (type %d) is not available\n", nrn_get_mechname(tml->index), tml->index);
            exit(1);
        }
        tml->ml->_nodecount_padded =
            nrn_soa_padded_size(tml->ml->nodecount, nrn_mech_data_layout_[tml->index]);
        if (memb_func[tml->index].is_point && nrn_is_artificial_[tml->index] == 0) {
            // Avoid race for multiple PointProcess instances in same compartment.
            if (tml->ml->nodecount > shadow_rhs_cnt) {
                shadow_rhs_cnt = tml->ml->nodecount;
            }
        }

        nt._ml_list[tml->index] = tml->ml;
#if CHKPNTDEBUG
        Memb_list_chkpnt* mlc = new Memb_list_chkpnt;
        ntc.mlmap[tml->index] = mlc;
#endif
        // printf("index=%d nodecount=%d membfunc=%s\n", tml->index, tml->ml->nodecount,
        // memb_func[tml->index].sym?memb_func[tml->index].sym:"None");
        if (nt.tml) {
            tml_last->next = tml;
        } else {
            nt.tml = tml;
        }
        tml_last = tml;
    }
    delete[] tml_index;
    delete[] ml_nodecount;

    if (shadow_rhs_cnt) {
        nt._shadow_rhs = (double*)ecalloc_align(nrn_soa_padded_size(shadow_rhs_cnt, 1),
                                                NRN_SOA_BYTE_ALIGN, sizeof(double));
        nt._shadow_d = (double*)ecalloc_align(nrn_soa_padded_size(shadow_rhs_cnt, 0),
                                              NRN_SOA_BYTE_ALIGN, sizeof(double));
        nt.shadow_rhs_cnt = shadow_rhs_cnt;
    }

    nt._data = NULL;    // allocated below after padding
    nt.mapping = NULL;  // section segment mapping

    if (nt._nidata)
        nt._idata = (int*)ecalloc(nt._nidata, sizeof(int));
    else
        nt._idata = NULL;
    // see patternstim.cpp
    int extra_nv = (&nt == nrn_threads) ? nrn_extra_thread0_vdata : 0;
    if (nt._nvdata + extra_nv)
        nt._vdata = (void**)ecalloc(nt._nvdata + extra_nv, sizeof(void*));
    else
        nt._vdata = NULL;
    // printf("_nidata=%d _nvdata=%d\n", nt._nidata, nt._nvdata);

    // The data format begins with the matrix data
    int ne = nrn_soa_padded_size(nt.end, MATRIX_LAYOUT);
    size_t offset = 6 * ne;

    if (ndiam) {
        // in the rare case that a mechanism has dparam with diam semantics
        // then actual_diam array added after matrix in nt._data
        // Generally wasteful since only a few diam are pointed to.
        // Probably better to move the diam semantics to the p array of the mechanism
        offset += ne;
    }

    // Memb_list.data points into the nt.data array.
    // Also count the number of Point_process
    int npnt = 0;
    for (tml = nt.tml; tml; tml = tml->next) {
        Memb_list* ml = tml->ml;
        int type = tml->index;
        int layout = nrn_mech_data_layout_[type];
        int n = ml->nodecount;
        int sz = nrn_prop_param_size_[type];
        offset = nrn_soa_byte_align(offset);
        ml->data = (double*)0 + offset;  // adjust below since nt._data not allocated
        offset += nrn_soa_padded_size(n, layout) * sz;
        if (pnt_map[type] > 0) {
            npnt += n;
        }
    }
    nt.pntprocs = new Point_process[npnt];  // includes acell with and without gid
    nt.n_pntproc = npnt;
    // printf("offset=%ld\n", offset);
    nt._ndata = offset;

    // now that we know the effect of padding, we can allocate data space,
    // fill matrix, and adjust Memb_list data pointers
    nt._data = (double*)ecalloc_align(nt._ndata, NRN_SOA_BYTE_ALIGN, sizeof(double));
    nt._actual_rhs = nt._data + 0 * ne;
    nt._actual_d = nt._data + 1 * ne;
    nt._actual_a = nt._data + 2 * ne;
    nt._actual_b = nt._data + 3 * ne;
    nt._actual_v = nt._data + 4 * ne;
    nt._actual_area = nt._data + 5 * ne;
    nt._actual_diam = ndiam ? nt._data + 6 * ne : NULL;
    for (tml = nt.tml; tml; tml = tml->next) {
        Memb_list* ml = tml->ml;
        ml->data = nt._data + (ml->data - (double*)0);
    }

    // matrix info
    nt._v_parent_index = (int*)ecalloc_align(nt.end, NRN_SOA_BYTE_ALIGN, sizeof(int));
    if (direct) {
        (*nrn2core_get_dat2_2_)(nt.id, nt._v_parent_index, nt._actual_a, nt._actual_b,
                                nt._actual_area, nt._actual_v, nt._actual_diam);
    } else {
        F.read_array<int>(nt._v_parent_index, nt.end);
        F.read_array<double>(nt._actual_a, nt.end);
        F.read_array<double>(nt._actual_b, nt.end);
        F.read_array<double>(nt._actual_area, nt.end);
        F.read_array<double>(nt._actual_v, nt.end);
        if (ndiam) {
            F.read_array<double>(nt._actual_diam, nt.end);
        }
    }
#if CHKPNTDEBUG
    ntc.parent = new int[nt.end];
    memcpy(ntc.parent, nt._v_parent_index, nt.end * sizeof(int));
    ntc.area = new double[nt.end];
    memcpy(ntc.area, nt._actual_area, nt.end * sizeof(double));
#endif

    int synoffset = 0;
    int* pnt_offset = new int[n_memb_func];

    // All the mechanism data and pdata.
    // Also fill in the pnt_offset
    // Complete spec of Point_process except for the acell presyn_ field.
    int itml = 0;
    int dsz_inst = 0;
    for (tml = nt.tml, itml = 0; tml; tml = tml->next, ++itml) {
        int type = tml->index;
        Memb_list* ml = tml->ml;
        int is_art = nrn_is_artificial_[type];
        int n = ml->nodecount;
        int szp = nrn_prop_param_size_[type];
        int szdp = nrn_prop_dparam_size_[type];
        int layout = nrn_mech_data_layout_[type];
        //printf("type:%d n:%d szp:%d szdp:%d layout:%d\n", type, n, szp, szdp, layout);

        if (!is_art && !direct) {
            ml->nodeindices = (int*)ecalloc_align(ml->nodecount, NRN_SOA_BYTE_ALIGN, sizeof(int));
        } else {
            ml->nodeindices = NULL;
        }

        
        if (szdp) {
            ml->pdata = (int*)ecalloc_align(nrn_soa_padded_size(n, layout) * szdp,
                                            NRN_SOA_BYTE_ALIGN, sizeof(int));
        }

        if (direct) {
            (*nrn2core_get_dat2_mech_)(nt.id, itml, dsz_inst, ml->nodeindices, ml->data, ml->pdata);
        } else {
            if (!is_art) {
                F.read_array<int>(ml->nodeindices, ml->nodecount);
                
            }
        }
        //if ((type == 15 || type == 24) && ml->nodeindices)
        //{
        //    for (int i = 0; i < n; i++)
        //        printf("%d ", ml->nodeindices[i]);
        //}
        //printf("\n");
        if (szdp) {
            ++dsz_inst;
        }

        mech_layout<double>(F, ml->data, n, szp, layout);

        if (szdp) {
            mech_layout<int>(F, ml->pdata, n, szdp, layout);
            //for (int i = 0; i < n; i++)
            //{
            //    printf("type:%d pdata %d:", type, i);
            //    for (int j = 0; j < szdp; j++)
            //    {
            //        printf("%d ", ml->pdata[nrn_i_layout(i, n, j, szdp, layout)]);
            //    }
            //    printf("\n");
            //}
            //printf("\n\n\n");
#if CHKPNTDEBUG  // Not substantive. Only for debugging.
            Memb_list_ckpnt* mlc = ntc.mlmap[type];
            mlc->pdata_not_permuted =
                (int*)coreneuron::ecalloc_align(n * szdp, NRN_SOA_BYTE_ALIGN, sizeof(int));
            if (layout == 1) {  // AoS just copy
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < szdp; ++j) {
                        mlc->pdata_not_permuted[i * szdp + j] = ml->pdata[i * szdp + j];
                    }
                }
            } else if (layout == 0) {  // SoA transpose and unpad
                int align_cnt = nrn_soa_padded_size(n, layout);
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < szdp; ++j) {
                        mlc->pdata_not_permuted[i * szdp + j] = ml->pdata[i + j * align_cnt];
                    }
                }
            }
#endif
        } else {
            ml->pdata = NULL;
        }
        if (pnt_map[type] > 0) {  // POINT_PROCESS mechanism including acell
            int cnt = ml->nodecount;
            Point_process* pnt = NULL;
            pnt = nt.pntprocs + synoffset;
            pnt_offset[type] = synoffset;
            synoffset += cnt;
            for (int i = 0; i < cnt; ++i) {
                Point_process* pp = pnt + i;
                pp->_type = type;
                pp->_i_instance = i;
                nt._vdata[ml->pdata[nrn_i_layout(i, cnt, 1, szdp, layout)]] = pp;
                pp->_tid = nt.id;
            }
        }
    }

    //Memb_list *ml = nt._ml_list[34];

    

    if (nrn_have_gaps == 1) {
        nrn_partrans::gap_thread_setup(nt);
    }

    // Some pdata may index into data which has been reordered from AoS to
    // SoA. The four possibilities are if semantics is -1 (area), -5 (pointer),
    // -9 (diam),
    // or 0-999 (ion variables). Note that pdata has a layout and the
    // type block in nt.data into which it indexes, has a layout.
    for (tml = nt.tml; tml; tml = tml->next) {
        int type = tml->index;
        int layout = nrn_mech_data_layout_[type];
        int* pdata = tml->ml->pdata;
        int cnt = tml->ml->nodecount;
        int szdp = nrn_prop_dparam_size_[type];
        int* semantics = memb_func[type].dparam_semantics;

        // ignore ARTIFICIAL_CELL (has useless area pointer with semantics=-1)
        if (nrn_is_artificial_[type]) {
            continue;
        }

        if (szdp) {
            if (!semantics)
                continue;  // temporary for HDFReport, Binreport which will be skipped in
                           // bbcore_write of HBPNeuron
            nrn_assert(semantics);
        }

        for (int i = 0; i < szdp; ++i) {
            int s = semantics[i];
            if (s == -1) {  // area
                int area0 = nt._actual_area - nt._data;
                for (int iml = 0; iml < cnt; ++iml) {
                    int* pd = pdata + nrn_i_layout(iml, cnt, i, szdp, layout);
                    int ix = *pd;  // relative to beginning of _actual_area
                    nrn_assert((ix >= 0) && (ix < nt.end));
                    *pd = area0 + ix;  // relative to nt._data
                }
            } else if (s == -9) {  // diam
                int diam0 = nt._actual_diam - nt._data;
                for (int iml = 0; iml < cnt; ++iml) {
                    int* pd = pdata + nrn_i_layout(iml, cnt, i, szdp, layout);
                    int ix = *pd;  // relative to beginning of _actual_diam
                    nrn_assert((ix >= 0) && (ix < nt.end));
                    *pd = diam0 + ix;  // relative to nt._data
                }
            } else if (s == -5) {  // pointer assumes a pointer to membrane voltage
                int v0 = nt._actual_v - nt._data;
                for (int iml = 0; iml < cnt; ++iml) {
                    int* pd = pdata + nrn_i_layout(iml, cnt, i, szdp, layout);
                    int ix = *pd;  // relative to _actual_v
                    nrn_assert((ix >= 0) && (ix < nt.end));
                    *pd = v0 + ix;  // relative to nt._data
                }
            } else if (s >= 0 && s < 1000) {  // ion
                int etype = s;
                int elayout = nrn_mech_data_layout_[etype];
                /* if ion is SoA, must recalculate pdata values */
                /* if ion is AoS, have to deal with offset */
                Memb_list* eml = nt._ml_list[etype];
                int edata0 = eml->data - nt._data;
                int ecnt = eml->nodecount;
                int esz = nrn_prop_param_size_[etype];
                for (int iml = 0; iml < cnt; ++iml) {
                    int* pd = pdata + nrn_i_layout(iml, cnt, i, szdp, layout);
                    int ix = *pd;  // relative to the ion data
                    nrn_assert((ix >= 0) && (ix < ecnt * esz));
                    /* Original pd order assumed ecnt groups of esz */
                    *pd = edata0 + nrn_param_layout(ix, etype, eml);
                }
            }
        }
        //if (szdp > 0)
        //{
        //    for (int i = 0; i < cnt; i++)
        //    {
        //        printf("type:%d pdata %d:\n", type, i);
        //        for (int j = 0; j < szdp; j++)
        //        {
        //            printf("secmatic:%d %d\n", semantics[j], tml->ml->pdata[nrn_i_layout(i, cnt, j, szdp, layout)]);
        //        }
        //        printf("\n\n");
        //    }
        //}
        
    }

    if (set_reset_v)
    {
        if (nt.ncell > 0)
        {
            char resetv_filename[300];
            sprintf(resetv_filename, "%s/%s", global_datapath.c_str(), "resetv");
            int ncell;
            FILE *fp = fopen(resetv_filename, "r");
            fscanf(fp, "%d", &ncell);
            printf("ncell:%d nt.ncell:%d\n", ncell, nt.ncell);
            assert(ncell == nt.ncell);
            nt.resetv_each_cell = new double[ncell];
            nt.resetv_each_node = new double[nt.end];
            for (int i = 0; i < ncell; i++)
            {
                int temp;
                fscanf(fp, "%d", &temp);
                //fscanf(fp, "%f", nt.resetv_each_cell + i);
                nt.resetv_each_cell[i] = temp * 1.0;
            }
            //fread(nt.resetv_each_cell, sizeof(double), ncell, fp);
            fclose(fp);
        }
    }
    
    /* if desired, apply the node permutation. This involves permuting
       at least the node parameter arrays for a, b, and area (and diam) and all
       integer vector values that index into nodes. This could have been done
       when originally filling the arrays with AoS ordered data, but can also
       be done now, after the SoA transformation. The latter has the advantage
       that the present order is consistent with all the layout values. Note
       that after this portion of the permutation, a number of other node index
       vectors will be read and will need to be permuted as well in subsequent
       sections of this function.
    */
    if (use_interleave_permute) {
        nt._permute = interleave_order(nt.id, nt.ncell, nt.end, nt._v_parent_index);
        //printf("%x\n", nt._permute);
    }
    if (nt._permute) {
        int* p = nt._permute;
        permute_data(nt._actual_a, nt.end, p);
        permute_data(nt._actual_b, nt.end, p);
        permute_data(nt._actual_area, nt.end, p);
        permute_data(nt._actual_v, nt.end,
                     p);  // need if restore or finitialize does not initialize voltage
        if (nt._actual_diam) {
            permute_data(nt._actual_diam, nt.end, p);
        }
        // index values change as well as ordering
        permute_ptr(nt._v_parent_index, nt.end, p);
        node_permute(nt._v_parent_index, nt.end, p);

#if 0
for (int i=0; i < nt.end; ++i) {
  printf("parent[%d] = %d\n", i, nt._v_parent_index[i]);
}
#endif

        // specify the ml->_permute and sort the nodeindices
        for (tml = nt.tml; tml; tml = tml->next) {
            if (tml->ml->nodeindices) {  // not artificial
                permute_nodeindices(tml->ml, p);
            }
        }

        // permute mechanism data, pdata (and values)
        for (tml = nt.tml; tml; tml = tml->next) {
            if (tml->ml->nodeindices) {  // not artificial
                permute_ml(tml->ml, tml->index, nt);
            }
        }

        // permute the Point_process._i_instance
        for (int i = 0; i < nt.n_pntproc; ++i) {
            Point_process& pp = nt.pntprocs[i];
            Memb_list* ml = nt._ml_list[pp._type];
            if (ml->_permute) {
                pp._i_instance = ml->_permute[pp._i_instance];
            }
        }
    }


    if (nrn_have_gaps == 1 && use_interleave_permute) {
        nrn_partrans::gap_indices_permute(nt);
    }

    /* here we setup the mechanism dependencies. if there is a mechanism dependency
     * then we allocate an array for tml->dependencies otherwise set it to NULL.
     * In order to find out the "real" dependencies i.e. dependent mechanism
     * exist at the same compartment, we compare the nodeindices of mechanisms
     * returned by nrn_mech_depend.
     */

    /* temporary array for dependencies */
    int* mech_deps = (int*)ecalloc(n_memb_func, sizeof(int));

    for (tml = nt.tml; tml; tml = tml->next) {
        /* initialize to null */
        tml->dependencies = NULL;
        tml->ndependencies = 0;

        /* get dependencies from the models */
        int deps_cnt = nrn_mech_depend(tml->index, mech_deps);

        /* if dependencies, setup dependency array */
        if (deps_cnt) {
            /* store "real" dependencies in the vector */
            std::vector<int> actual_mech_deps;

            Memb_list* ml = tml->ml;
            int* nodeindices = ml->nodeindices;

            /* iterate over dependencies */
            for (int j = 0; j < deps_cnt; j++) {
                /* memb_list of dependency mechanism */
                Memb_list* dml = nt._ml_list[mech_deps[j]];

                /* dependency mechanism may not exist in the model */
                if (!dml)
                    continue;

                /* take nodeindices for comparison */
                int* dnodeindices = dml->nodeindices;

                /* set_intersection function needs temp vector to push the common values */
                std::vector<int> node_intersection;

                /* make sure they have non-zero nodes and find their intersection */
                if ((ml->nodecount > 0) && (dml->nodecount > 0)) {
                    std::set_intersection(nodeindices, nodeindices + ml->nodecount, dnodeindices,
                                          dnodeindices + dml->nodecount,
                                          std::back_inserter(node_intersection));
                }

                /* if they intersect in the nodeindices, it's real dependency */
                if (!node_intersection.empty()) {
                    actual_mech_deps.push_back(mech_deps[j]);
                }
            }

            /* copy actual_mech_deps to dependencies */
            if (!actual_mech_deps.empty()) {
                tml->ndependencies = actual_mech_deps.size();
                tml->dependencies = (int*)ecalloc(actual_mech_deps.size(), sizeof(int));
                memcpy(tml->dependencies, &actual_mech_deps[0],
                       sizeof(int) * actual_mech_deps.size());
            }
        }
    }


    /* free temp dependency array */
    free(mech_deps);

    /// Fill the BA lists
    BAMech** bamap = new BAMech*[n_memb_func];
    for (int i = 0; i < BEFORE_AFTER_SIZE; ++i) {
        BAMech* bam;
        NrnThreadBAList *tbl, **ptbl;
        for (int ii = 0; ii < n_memb_func; ++ii) {
            bamap[ii] = (BAMech*)0;
        }
        for (bam = bamech_[i]; bam; bam = bam->next) {
            bamap[bam->type] = bam;
        }
        /* unnecessary but keep in order anyway */
        ptbl = nt.tbl + i;
        for (tml = nt.tml; tml; tml = tml->next) {
            if (bamap[tml->index]) {
                Memb_list* ml = tml->ml;
                tbl = (NrnThreadBAList*)emalloc(sizeof(NrnThreadBAList));
                tbl->next = (NrnThreadBAList*)0;
                tbl->bam = bamap[tml->index];
                tbl->ml = ml;
                *ptbl = tbl;
                ptbl = &(tbl->next);
            }
        }
    }
    delete[] bamap;

    // for fast watch statement checking
    // setup a list of types that have WATCH statement
    {
        int sz = 0;  // count the types with WATCH
        for (NrnThreadMembList* tml = nt.tml; tml; tml = tml->next) {
            if (nrn_watch_check[tml->index]) {
                ++sz;
            }
        }
        if (sz) {
            nt._watch_types = (int*)ecalloc(sz + 1, sizeof(int));  // NULL terminated
            sz = 0;
            for (NrnThreadMembList* tml = nt.tml; tml; tml = tml->next) {
                if (nrn_watch_check[tml->index]) {
                    nt._watch_types[sz++] = tml->index;
                }
            }
        }
    }

    // from nrn_has_net_event create pnttype2presyn.
    if (!pnttype2presyn) {
        pnttype2presyn = (int*)ecalloc(n_memb_func, sizeof(int));
    }
    for (int i = 0; i < n_memb_func; ++i) {
        pnttype2presyn[i] = -1;
    }
    for (int i = 0; i < nrn_has_net_event_cnt_; ++i) {
        pnttype2presyn[nrn_has_net_event_[i]] = i;
    }
    // create the nt.pnt2presyn_ix array of arrays.
    nt.pnt2presyn_ix = (int**)ecalloc(nrn_has_net_event_cnt_, sizeof(int*));
    for (int i = 0; i < nrn_has_net_event_cnt_; ++i) {
        Memb_list* ml = nt._ml_list[nrn_has_net_event_[i]];
        if (ml && ml->nodecount > 0) {
            nt.pnt2presyn_ix[i] = (int*)ecalloc(ml->nodecount, sizeof(int));
        }
    }

    // Real cells are at the beginning of the nt.presyns followed by
    // acells (with and without gids mixed together)
    // Here we associate the real cells with voltage pointers and
    // acell PreSyn with the Point_process.
    // nt.presyns order same as output_vindex order
    int *output_vindex, *pnttype, *pntindex;
    double *output_threshold, *delay;
    if (direct) {
        (*nrn2core_get_dat2_3_)(nt.id, nt.n_weight, output_vindex, output_threshold, pnttype,
                                pntindex, nt.weights, delay);
    }
    if (!direct) {
        output_vindex = F.read_array<int>(nt.n_presyn);
    }
#if CHKPNTDEBUG
    ntc.output_vindex = new int[nt.n_presyn];
    memcpy(ntc.output_vindex, output_vindex, nt.n_presyn * sizeof(int));
#endif
    if (nt._permute) {
        // only indices >= 0 (i.e. _actual_v indices) will be changed.
        node_permute(output_vindex, nt.n_presyn, nt._permute);
    }
    if (!direct) {
        output_threshold = F.read_array<double>(nt.ncell);
    }
#if CHKPNTDEBUG
    ntc.output_threshold = new double[nt.ncell];
    memcpy(ntc.output_threshold, output_threshold, nt.ncell * sizeof(double));
#endif
    for (int i = 0; i < nt.n_presyn; ++i) {  // real cells
        PreSyn* ps = nt.presyns + i;

        int ix = output_vindex[i];
        if (ix == -1 && i < nt.ncell) {  // real cell without a presyn
            continue;
        }
        if (ix < 0) {
            ix = -ix;
            int index = ix / 100;
            int type = ix - index * 100;
            Point_process* pnt = nt.pntprocs + (pnt_offset[type] + index);
            ps->pntsrc_ = pnt;
            // pnt->_presyn = ps;
            int ip2ps = pnttype2presyn[pnt->_type];
            if (ip2ps >= 0) {
                nt.pnt2presyn_ix[ip2ps][pnt->_i_instance] = i;
            }
            if (ps->gid_ < 0) {
                ps->gid_ = -1;
            }
        } else {
            assert(ps->gid_ > -1);
            ps->thvar_index_ = ix;  // index into _actual_v
            assert(ix < nt.end);
            ps->threshold_ = output_threshold[i];
        }
    }
    delete[] output_vindex;
    delete[] output_threshold;

    // initial net_send_buffer size about 1% of number of presyns
    // nt._net_send_buffer_size = nt.ncell/100 + 1;
    // but, to avoid reallocation complexity on GPU ...
    nt._net_send_buffer_size = nt.ncell;
    nt._net_send_buffer = (int*)ecalloc(nt._net_send_buffer_size, sizeof(int));

    // do extracon later as the target and weight info
    // is not directly in the file
    int nnetcon = nt.n_netcon - nrn_setup_extracon;

    int nweight = nt.n_weight;
    // printf("nnetcon=%d nweight=%d\n", nnetcon, nweight);
    // it may happen that Point_process structures will be made unnecessary
    // by factoring into NetCon.

    // Make NetCon.target_ point to proper Point_process. Only the NetCon
    // with pnttype[i] > 0 have a target.
    if (!direct) {
        pnttype = F.read_array<int>(nnetcon);
    }
    if (!direct) {
        pntindex = F.read_array<int>(nnetcon);
    }
#if CHKPNTDEBUG
    ntc.pnttype = new int[nnetcon];
    ntc.pntindex = new int[nnetcon];
    memcpy(ntc.pnttype, pnttype, nnetcon * sizeof(int));
    memcpy(ntc.pntindex, pntindex, nnetcon * sizeof(int));
#endif
    for (int i = 0; i < nnetcon; ++i) {
        int type = pnttype[i];
        if (type > 0) {
            int index = pnt_offset[type] + pntindex[i];  /// Potentially uninitialized pnt_offset[],
                                                         /// check for previous assignments
            Point_process* pnt = nt.pntprocs + index;
            NetCon& nc = nt.netcons[i];
            nc.target_ = pnt;
            nc.active_ = true;
        }
    }

    int extracon_target_type = -1;
    int extracon_target_nweight = 0;
    if (nrn_setup_extracon > 0) {
        // Fill in the extracon target_ and active_.
        // Simplistic.
        // Rotate through the pntindex and use only pnttype for ProbAMPANMDA_EMS
        // (which happens to have a weight vector length of 5.)
        // Edge case: if there is no such synapse, let the target_ be NULL
        //   and the netcon be inactive.
        // Same pattern as algorithm for extracon netcon_srcgid above in phase1.
        extracon_target_type = nrn_get_mechtype("ProbAMPANMDA_EMS");
        assert(extracon_target_type > 0);
        extracon_target_nweight = pnt_receive_size[extracon_target_type];
        int j = 0;
        for (int i = 0; i < nrn_setup_extracon; ++i) {
            int active = 0;
            for (int k = 0; k < nnetcon; ++k) {
                if (pnttype[j] == extracon_target_type) {
                    active = 1;
                    break;
                }
                j = (j + 1) % nnetcon;
            }
            NetCon& nc = nt.netcons[i + nnetcon];
            nc.active_ = active;
            if (active) {
                nc.target_ = nt.pntprocs + (pnt_offset[extracon_target_type] + pntindex[j]);
            } else {
                nc.target_ = NULL;
            }
        }
    }

    delete[] pntindex;

    // weights in netcons order in groups defined by Point_process target type.
    nt.n_weight += nrn_setup_extracon * extracon_target_nweight;
    if (!direct) {
        nt.weights = new double[nt.n_weight];
        F.read_array<double>(nt.weights, nweight);
    }

    int iw = 0;
    for (int i = 0; i < nnetcon; ++i) {
        NetCon& nc = nt.netcons[i];
        nc.u.weight_index_ = iw;
        if (pnttype[i] != 0) {
            iw += pnt_receive_size[pnttype[i]];
        } else {
            iw += 1;
        }
    }
    assert(iw == nweight);
    delete[] pnttype;

    // delays in netcons order
    if (!direct) {
        delay = F.read_array<double>(nnetcon);
    }
#if CHKPNTDEBUG
    ntc.delay = new double[nnetcon];
    memcpy(ntc.delay, delay, nnetcon * sizeof(double));
#endif
    for (int i = 0; i < nnetcon; ++i) {
        NetCon& nc = nt.netcons[i];
        nc.delay_ = delay[i];
    }
    delete[] delay;

    if (nrn_setup_extracon > 0) {
        // simplistic. delay is 1 and weight is 0.001
        for (int i = 0; i < nrn_setup_extracon; ++i) {
            NetCon& nc = nt.netcons[nnetcon + i];
            nc.delay_ = 1.0;
            nc.u.weight_index_ = nweight + i * extracon_target_nweight;
            nt.weights[nc.u.weight_index_] = 2.0;  // this value 2.0 is extracted from .dat files
        }
    }

    // BBCOREPOINTER information
    if (direct) {
        (*nrn2core_get_dat2_corepointer_)(nt.id, npnt);
    } else {
        npnt = F.read_int();
    }
#if CHKPNTDEBUG
    ntc.nbcp = npnt;
    ntc.bcpicnt = new int[npnt];
    ntc.bcpdcnt = new int[npnt];
    ntc.bcptype = new int[npnt];
#endif
    for (NrnThreadMembList* tml = nt.tml; tml; tml = tml->next) {
        int type = tml->index;
        if (!nrn_bbcore_read_[type]) {
            continue;
        }
        int* iArray = NULL;
        double* dArray = NULL;
        int icnt, dcnt;
        if (direct) {
            (*nrn2core_get_dat2_corepointer_mech_)(nt.id, type, icnt, dcnt, iArray, dArray);
        } else {
            type = F.read_int();
            icnt = F.read_int();
            dcnt = F.read_int();
            if (icnt) {
                iArray = F.read_array<int>(icnt);
            }
            if (dcnt) {
                dArray = F.read_array<double>(dcnt);
            }
        }
        if (!nrn_bbcore_write_[type] && nrn_checkpoint_arg_exists) {
            fprintf(
                stderr,
                "Checkpoint is requested involving BBCOREPOINTER but there is no bbcore_write function for %s\n",
                memb_func[type].sym);
            assert(nrn_bbcore_write_[type]);
        }
#if CHKPNTDEBUG
        ntc.bcptype[i] = type;
        ntc.bcpicnt[i] = icnt;
        ntc.bcpdcnt[i] = dcnt;
#endif
        int ik = 0;
        int dk = 0;
        Memb_list* ml = nt._ml_list[type];
        int dsz = nrn_prop_param_size_[type];
        int pdsz = nrn_prop_dparam_size_[type];
        int cntml = ml->nodecount;
        int layout = nrn_mech_data_layout_[type];
        for (int j = 0; j < cntml; ++j) {
            int jp = j;
            if (ml->_permute) {
                jp = ml->_permute[j];
            }
            double* d = ml->data;
            Datum* pd = ml->pdata;
            d += nrn_i_layout(jp, cntml, 0, dsz, layout);
            pd += nrn_i_layout(jp, cntml, 0, pdsz, layout);
            int aln_cntml = nrn_soa_padded_size(cntml, layout);
            (*nrn_bbcore_read_[type])(dArray, iArray, &dk, &ik, 0, aln_cntml, d, pd, ml->_thread,
                                      &nt, 0.0);
        }
        assert(dk == dcnt);
        assert(ik == icnt);
        if (ik) {
            delete[] iArray;
        }
        if (dk) {
            delete[] dArray;
        }
    }

    // VecPlayContinuous instances
    // No attempt at memory efficiency
    int n;
    if (direct) {
        (*nrn2core_get_dat2_vecplay_)(nt.id, n);
    } else {
        n = F.read_int();
    }
    nt.n_vecplay = n;
    if (n) {
        nt._vecplay = new void*[n];
    } else {
        nt._vecplay = NULL;
    }
#if CHKPNTDEBUG
    ntc.vecplay_ix = new int[n];
    ntc.vtype = new int[n];
    ntc.mtype = new int[n];
#endif
    for (int i = 0; i < n; ++i) {
        int vtype, mtype, ix, sz;
        double *yvec1, *tvec1;
        if (direct) {
            (*nrn2core_get_dat2_vecplay_inst_)(nt.id, i, vtype, mtype, ix, sz, yvec1, tvec1);
        } else {
            vtype = F.read_int();
            mtype = F.read_int();
            ix = F.read_int();
            sz = F.read_int();
        }
        nrn_assert(vtype == VecPlayContinuousType);
#if CHKPNTDEBUG
        ntc.vtype[i] = vtype;
#endif
#if CHKPNTDEBUG
        ntc.mtype[i] = mtype;
#endif
        Memb_list* ml = nt._ml_list[mtype];
#if CHKPNTDEBUG
        ntc.vecplay_ix[i] = ix;
#endif
        IvocVect* yvec = vector_new(sz);
        IvocVect* tvec = vector_new(sz);
        if (direct) {
            double* py = vector_vec(yvec);
            double* pt = vector_vec(tvec);
            for (int j = 0; j < sz; ++j) {
                py[j] = yvec1[j];
                pt[j] = tvec1[j];
            }
            delete[] yvec1;
            delete[] tvec1;
        } else {
            F.read_array<double>(vector_vec(yvec), sz);
            F.read_array<double>(vector_vec(tvec), sz);
        }
        ix = nrn_param_layout(ix, mtype, ml);
        if (ml->_permute) {
            ix = nrn_index_permute(ix, mtype, ml);
        }
        nt._vecplay[i] = new VecPlayContinuous(ml->data + ix, yvec, tvec, NULL, nt.id);
    }
    if (!direct) {
        // store current checkpoint state to continue reading mapping
        F.record_checkpoint();

        // If not at end of file, then this must be a checkpoint and restore tqueue.
        if (!F.eof()) {
            checkpoint_restore_tqueue(nt, F);
        }
    }
    // NetReceiveBuffering
    for (int i = 0; i < net_buf_receive_cnt_; ++i) {
        int type = net_buf_receive_type_[i];
        // Does this thread have this type.
        Memb_list* ml = nt._ml_list[type];
        if (ml) {  // needs a NetReceiveBuffer
            NetReceiveBuffer_t* nrb = (NetReceiveBuffer_t*)ecalloc(1, sizeof(NetReceiveBuffer_t));
            ml->_net_receive_buffer = nrb;
            nrb->_pnt_offset = pnt_offset[type];

            // begin with a size of 5% of the number of instances
            nrb->_size = ml->nodecount;
            // or at least 8
            if (nrb->_size < 8) {
                nrb->_size = 8;
            }
            // but not more than nodecount
            if (nrb->_size > ml->nodecount) {
                nrb->_size = ml->nodecount;
            }

            nrb->_pnt_index = (int*)ecalloc(nrb->_size, sizeof(int));
            nrb->_displ = (int*)ecalloc(nrb->_size + 1, sizeof(int));
            nrb->_nrb_index = (int*)ecalloc(nrb->_size, sizeof(int));
            nrb->_weight_index = (int*)ecalloc(nrb->_size, sizeof(int));
            nrb->_nrb_t = (double*)ecalloc(nrb->_size, sizeof(double));
            nrb->_nrb_flag = (double*)ecalloc(nrb->_size, sizeof(double));
        }
    }

    // NetSendBuffering
    for (int i = 0; i < net_buf_send_cnt_; ++i) {
        int type = net_buf_send_type_[i];
        // Does this thread have this type.
        Memb_list* ml = nt._ml_list[type];
        if (ml) {  // needs a NetSendBuffer
            NetSendBuffer_t* nsb = (NetSendBuffer_t*)ecalloc(1, sizeof(NetSendBuffer_t));
            ml->_net_send_buffer = nsb;

            // begin with a size equal to twice number of instances
            // at present there is no provision for dynamically increasing this.
            nsb->_size = ml->nodecount * 2;
            nsb->_cnt = 0;

            nsb->_sendtype = (int*)ecalloc(nsb->_size, sizeof(int));
            nsb->_vdata_index = (int*)ecalloc(nsb->_size, sizeof(int));
            nsb->_pnt_index = (int*)ecalloc(nsb->_size, sizeof(int));
            nsb->_weight_index = (int*)ecalloc(nsb->_size, sizeof(int));
            // when == 1, NetReceiveBuffer_t is newly allocated (i.e. we need to free previous copy
            // and recopy new data
            nsb->reallocated = 1;
            nsb->_nsb_t = (double*)ecalloc(nsb->_size, sizeof(double));
            nsb->_nsb_flag = (double*)ecalloc(nsb->_size, sizeof(double));
        }
    }

    delete[] pnt_offset;

    //if (nt.ncell > 0)
    //    nt.fp = fopen("v_rec.txt", "w");
    //if (nt.ncell > 0)
    //{
    //    std::cout << "data save to: " << rec_filename << std::endl;
    //    nt.fp_rec = fopen(rec_filename.c_str(), "wb");
    //}
    
    char secname_file[300], node2sec_file[300], record_param_file[300], gap_param_file[300];
    sprintf(secname_file, "%s/%s", global_datapath.c_str(), "secnamelist.txt");
    sprintf(node2sec_file, "%s/%s", global_datapath.c_str(), "map_node2sec");
    struct stat buffer;
    if (stat(secname_file, &buffer) != 0)
        sprintf(secname_file, "%s/secnamelist%d.txt", global_datapath.c_str(), nrnmpi_myid);
    if (stat(node2sec_file, &buffer) != 0)
        sprintf(node2sec_file, "%s/map_node2sec%d", global_datapath.c_str(), nrnmpi_myid);
        
    
    if (need_record)
    {
        if (nt.ncell > 0)
        {
            assert(!rec_filename.empty() || dt_io > dt);
            if (nrnmpi_numprocs > 1)
                sprintf(record_param_file, "%s/%s%d", global_datapath.c_str(), "param2rec", nrnmpi_myid);
            else
                sprintf(record_param_file, "%s/%s", global_datapath.c_str(), "param2rec");
            if (stat(record_param_file, &buffer) == 0)
            {
                std::string out_name;
                if (nrnmpi_numprocs > 1)
                    out_name = rec_filename + "_" + std::to_string(nrnmpi_myid);
                else
                    out_name = rec_filename;
                std::cout << "num_procs: " << nrnmpi_numprocs << " data save to: " << out_name << std::endl;
                nt.fp_rec = fopen(out_name.c_str(), "wb");
                nt.dt_io = dt_io;
                set_records(nt, secname_file, node2sec_file, record_param_file, nt._permute);
            }
        }
    }

    if (param_val_file.length() > 0)
    {
        set_param_val(nt, secname_file, node2sec_file, param_val_file.c_str(), nt._permute);
    }

    if (has_non_v_gap)
    {
        if (nt.ncell > 0)
        {
            sprintf(gap_param_file, "%s/%s", global_datapath.c_str(), "gap_params");
            set_non_voltage_gap(nt, secname_file, node2sec_file, gap_param_file, nt._permute);
        }
    }

    
    

    char filename[300];
    if (is_training || is_testing)
    {
        if (nt.ncell > 0)
        {
            nt.ninput = 784;
            nt.nhidden = 64;
            nt.nout = 10;
            nt.nproj = 1;
            if (nt.mapping == NULL)
                gen_mapping(nt, secname_file, node2sec_file);
            //char in2hd_syn_file[300], hd2out_syn_file[300], stim_file[300];
            sprintf(filename, "%s/%s", global_datapath.c_str(), "stim_amp");
            printf("%s\n", filename);
            set_ptr_and_idx(filename, nt, nt.stim_len, nt.stim_mech_types, nt.stim_idx, nt.stim_ptrs, nt._permute); 
           
            
        
            /*zyc manually set batch size, stimuli and labels*/
            nt.batch_size = batch_size;
            printf("batchsize:%d\n", nt.batch_size);
            nt.sim_time_per_img = 50.0;
            nt.passed_time = 0.0;
            nt.istim = 0;
            if (input_filename.empty())
            {
                printf("No specific input file, use default stim_img file\n");
                if (is_training)
                    sprintf(filename, "%s/%s", global_datapath.c_str(), "stim_img_train");
                if (is_testing)
                    sprintf(filename, "%s/%s", global_datapath.c_str(), "stim_img_test");
            }
            else
            {
                sprintf(filename, "%s", input_filename.c_str());
            }
            printf("Input file:%s\n", filename);
            FILE *fp = fopen(filename, "rb");
            fread(&nt.nimg, sizeof(int), 1, fp);
            fread(&nt.img_size, sizeof(int), 1, fp);
            printf ("nimg:%d img_size:%d\n", nt.nimg, nt.img_size);
            nt.imgs = new double[nt.nimg * nt.img_size];
            nt.labels = new int[nt.nimg];
            fread(nt.imgs, sizeof(double), nt.nimg * nt.img_size, fp);
            //for (int i = 0; i < 365; i++)
            //    printf("i:%d %f\n", i, nt.imgs[i]);
            fread(nt.labels, sizeof(int), nt.nimg, fp);
            fclose(fp);
            /*zyc manually set batch size*/
        }
    }

    if (is_training && nt.ncell > 0)
    {
        sprintf(filename, "%s/%s", global_datapath.c_str(), "weights");
        printf("%s\n", filename);
        set_ptr_and_idx(filename, nt, nt.nweights, nt.w_mech_types, nt.w_idx, nt.w_ptrs, nt._permute);
        nt.nweights_per_net = nt.nweights / nt.batch_size;
        
        sprintf(filename, "%s/%s", global_datapath.c_str(), "delta_weights");
        printf("%s\n", filename);
        set_ptr_and_idx(filename, nt, nt.nweights, nt.dw_mech_types, nt.dw_idx, nt.dw_ptrs, nt._permute);

        sprintf(filename, "%s/%s", global_datapath.c_str(), "in2hd_syn");
        printf("%s\n", filename);
        set_ptr_and_idx(filename, nt, nt.in2hd_len, nt.in2hd_mech_types, nt.in2hd_idx, nt.in2hd_ptrs, nt._permute);

        sprintf(filename, "%s/%s", global_datapath.c_str(), "h2out_syn");
        printf("%s\n", filename);
        set_ptr_and_idx(filename, nt, nt.hd2out_len, nt.hd2out_mech_types, nt.hd2out_idx, nt.hd2out_ptrs, nt._permute);

        sprintf(filename, "%s/%s", global_datapath.c_str(), "in2hd_g");
        printf("%s\n", filename);
        set_ptr_and_idx(filename, nt, nt.in2hd_g_len, nt.in2hd_g_mech_types, nt.in2hd_g_idx, nt.in2hd_g_ptrs, nt._permute);
    }

    if (is_testing && nt.ncell > 0)
    {
        //assert(!weights_filename.empty());
        //int weights_len = 0;
        //FILE *fp = fopen(weights_filename.c_str(), "rb");
        //fread(&weights_len, sizeof(int), 1, fp);
        //assert(weights_len == nt.nweights);
        //nt.weights_buffer = new float[nt.nweights];
        //fread(nt.weights_buffer, sizeof(float), nt.nweights, fp);
        //
        //for (int i = 0; i < nt.nweights; i++)
        //{
        //    *nt.w_ptrs[i] = nt.weights_buffer[i];
        //}

        printf("testing\n");
        if (nt.rec_times && nt.rec_type > -1)
        {
            delete[] nt.rec_times;
            nt.rec_times = NULL;
        
            if (nt.rec_vals)
            {
                delete[] nt.rec_vals;
                nt.rec_vals = NULL;
            }
            nt.rec_len = nt.nimg;
            nt.rec_times = new float[nt.nimg];
            nt.rec_vals = new float[nt.nrec_param * nt.rec_len];
            for (int i = 0; i < nt.nimg; i++)
            {
                nt.rec_times[i] = 40.0 + nt.sim_time_per_img * i;
            }
        }
        else if (nt.rec_type == -1)
        {
            if (nt.rec_times)
            {
                delete[] nt.rec_times;
                nt.rec_times = NULL;
            }
            nt.rec_len = nt.nimg / nt.batch_size;
            nt.rec_vals = new float[nt.nrec_param * nt.rec_len];
            nt.rec_times = new float[nt.rec_len];
            for (int i = 0; i < nt.rec_len; i++)
                nt.rec_times[i] = nt.mean_e_time + nt.sim_time_per_img * i;
            memset(nt.rec_vals, 0, sizeof(float) * nt.nrec_param * nt.rec_len);
        }
        if (nt.fp_rec)
            fclose(nt.fp_rec);
        nt.fp_rec = fopen(rec_filename.c_str(), "wb");
        fprintf(nt.fp_rec, "%d %d %d %d\n", nt.nrec_param, nt.rec_len, 1, 0);
    }
}

/** read mapping information for neurons */
void read_phase3(FileHandler& F, int imult, NrnThread& nt) {
    (void)imult;

    /** restore checkpoint state (before restoring queue items */
    F.restore_checkpoint();

    /** mapping information for all neurons in single NrnThread */
    NrnThreadMappingInfo* ntmapping = new NrnThreadMappingInfo();

    int count = 0;

    F.read_mapping_cell_count(&count);

    /** number of cells in mapping file should equal to cells in NrnThread */
    nrn_assert(count == nt.ncell);

    /** for every neuron */
    for (int i = 0; i < nt.ncell; i++) {
        int gid, nsec, nseg, nseclist;

        // read counts
        F.read_mapping_count(&gid, &nsec, &nseg, &nseclist);
        printf("gid:%d nsec:%d nseg:%d nseclist:%d\n", gid, nsec, nseg, nseclist);

        CellMapping* cmap = new CellMapping(gid);

        // read section-segment mapping for every section list
        for (int j = 0; j < nseclist; j++) {
            SecMapping* smap = new SecMapping();
            F.read_mapping_info(smap);
            cmap->add_sec_map(smap);
        }

        ntmapping->add_cell_mapping(cmap);
    }

    // make number #cells match with mapping size
    nrn_assert((int)ntmapping->size() == nt.ncell);

    // set pointer in NrnThread
    nt.mapping = (void*)ntmapping;
}

static size_t memb_list_size(NrnThreadMembList* tml) {
    size_t sz_ntml = sizeof(NrnThreadMembList);
    size_t sz_ml = sizeof(Memb_list);
    size_t szi = sizeof(int);
    size_t nbyte = sz_ntml + sz_ml;
    nbyte += tml->ml->nodecount * szi;
    nbyte += nrn_prop_dparam_size_[tml->index] * tml->ml->nodecount * sizeof(Datum);
#ifdef DEBUG
    int i = tml->index;
    printf("%s %d psize=%d ppsize=%d cnt=%d nbyte=%ld\n", memb_func[i].sym, i,
           nrn_prop_param_size_[i], nrn_prop_dparam_size_[i], tml->ml->nodecount, nbyte);
#endif
    return nbyte;
}

/// Approximate count of number of bytes for the gid2out map
size_t output_presyn_size(void) {
    if (gid2out.empty()) {
        return 0;
    }
    size_t nbyte =
        sizeof(gid2out) + sizeof(int) * gid2out.size() + sizeof(PreSyn*) * gid2out.size();
#ifdef DEBUG
    printf(" gid2out table bytes=~%ld size=%d\n", nbyte, gid2out.size());
#endif
    return nbyte;
}

size_t input_presyn_size(void) {
    if (gid2in.empty()) {
        return 0;
    }
    size_t nbyte =
        sizeof(gid2in) + sizeof(int) * gid2in.size() + sizeof(InputPreSyn*) * gid2in.size();
#ifdef DEBUG
    printf(" gid2in table bytes=~%ld size=%d\n", nbyte, gid2in->size());
#endif
    return nbyte;
}

size_t model_size(void) {
    size_t nbyte = 0;
    size_t szd = sizeof(double);
    size_t szi = sizeof(int);
    size_t szv = sizeof(void*);
    size_t sz_th = sizeof(NrnThread);
    size_t sz_ps = sizeof(PreSyn);
    size_t sz_psi = sizeof(InputPreSyn);
    size_t sz_nc = sizeof(NetCon);
    size_t sz_pp = sizeof(Point_process);
    NrnThreadMembList* tml;
    size_t nccnt = 0;

    for (int i = 0; i < nrn_nthread; ++i) {
        NrnThread& nt = nrn_threads[i];
        size_t nb_nt = 0;  // per thread
        nccnt += nt.n_netcon;

        // Memb_list size
        int nmech = 0;
        for (tml = nt.tml; tml; tml = tml->next) {
            nb_nt += memb_list_size(tml);
            ++nmech;
        }

        // basic thread size includes mechanism data and G*V=I matrix
        nb_nt += sz_th;
        nb_nt += nt._ndata * szd + nt._nidata * szi + nt._nvdata * szv;
        nb_nt += nt.end * szi;  // _v_parent_index

#ifdef DEBUG
        printf("ncell=%d end=%d nmech=%d\n", nt.ncell, nt.end, nmech);
        printf("ndata=%ld nidata=%ld nvdata=%ld\n", nt._ndata, nt._nidata, nt._nvdata);
        printf("nbyte so far %ld\n", nb_nt);
        printf("n_presyn = %d sz=%ld nbyte=%ld\n", nt.n_presyn, sz_ps, nt.n_presyn * sz_ps);
        printf("n_input_presyn = %d sz=%ld nbyte=%ld\n", nt.n_input_presyn, sz_psi,
               nt.n_input_presyn * sz_psi);
        printf("n_pntproc=%d sz=%ld nbyte=%ld\n", nt.n_pntproc, sz_pp, nt.n_pntproc * sz_pp);
        printf("n_netcon=%d sz=%ld nbyte=%ld\n", nt.n_netcon, sz_nc, nt.n_netcon * sz_nc);
        printf("n_weight = %d\n", nt.n_weight);
#endif

        // spike handling
        nb_nt += nt.n_pntproc * sz_pp + nt.n_netcon * sz_nc + nt.n_presyn * sz_ps +
                 nt.n_input_presyn * sz_psi + nt.n_weight * szd;
        nbyte += nb_nt;
#ifdef DEBUG
        printf("%d thread %d total bytes %ld\n", nrnmpi_myid, i, nb_nt);
#endif
    }

#ifdef DEBUG
    printf("%d netcon pointers %ld  nbyte=%ld\n", nrnmpi_myid, nccnt, nccnt * sizeof(NetCon*));
#endif
    nbyte += nccnt * sizeof(NetCon*);
    nbyte += output_presyn_size();
    nbyte += input_presyn_size();

#ifdef DEBUG
    printf("nrnran123 size=%ld cnt=%ld nbyte=%ld\n", nrnran123_state_size(),
           nrnran123_instance_count(), nrnran123_instance_count() * nrnran123_state_size());
#endif

    nbyte += nrnran123_instance_count() * nrnran123_state_size();

#ifdef DEBUG
    printf("%d total bytes %ld\n", nrnmpi_myid, nbyte);
#endif

    return nbyte;
}

}  // namespace coreneuron

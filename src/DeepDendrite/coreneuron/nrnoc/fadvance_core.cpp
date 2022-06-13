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

#include "coreneuron/coreneuron.h"
#include "coreneuron/nrnconf.h"
#include "coreneuron/nrnoc/multicore.h"
#include "coreneuron/nrnmpi/nrnmpi.h"
#include "coreneuron/nrnoc/nrnoc_decl.h"
#include "coreneuron/nrniv/nrn_acc_manager.h"
#include "coreneuron/utils/reports/nrnreport.h"
#include "coreneuron/utils/progressbar/progressbar.h"
#include "coreneuron/nrniv/profiler_interface.h"
#include "coreneuron/nrniv/output_spikes.h"
#include <math.h>

namespace coreneuron {

double deliver_time = 0.0;
double treeset_time = 0.0;
double solve_time = 0.0;
double lastpart_time = 0.0;
double update_time = 0.0;
double init_and_update_time = 0.0;
double play_continuous_time = 0.0;
double second_cur_time = 0.0;
double nonv_gap_time = 0.0;

static void* nrn_fixed_step_thread(NrnThread*);
static void* nrn_fixed_step_group_thread(NrnThread*);

void dt2thread(double adt) { /* copied from nrnoc/fadvance.c */
    if (adt != nrn_threads[0]._dt) {
        int i;
        for (i = 0; i < nrn_nthread; ++i) {
            NrnThread* nt = nrn_threads + i;
            nt->_t = t;
            nt->_dt = dt;
            if (secondorder) {
                nt->cj = 2.0 / dt;
            } else {
                nt->cj = 1.0 / dt;
            }
        }
    }
}

void gap_non_v_transfer(NrnThread *nt)
{
    //NrnThread* nt = &nrn_threads[ithread];
    //if (nt->ncell <= 0)
    //    continue;
    int nparam = nt->nnon_v_gap;
    //printf("nparam:%d\n", nparam);
    double **src_ptrs = nt->gap_src_param_ptrs;
    double **tgt_ptrs = nt->gap_tgt_param_ptrs;
    #pragma acc parallel loop present(src_ptrs[0:nparam], tgt_ptrs[0:nparam]) if (nt->compute_gpu) async(nt->stream_id)
    for (int iparam = 0; iparam < nparam; iparam++)
    {
        *tgt_ptrs[iparam] = *src_ptrs[iparam];
        //printf("src:%f tgt:%f\n", *src_ptrs[iparam], *tgt_ptrs[iparam]);
    }
}

void nrn_fixed_step_minimal() { /* not so minimal anymore with gap junctions */
    if (t != nrn_threads->_t) {
        dt2thread(-1.);
    } else {
        dt2thread(dt);
    }
    nrn_thread_table_check();
    nrn_multithread_job(nrn_fixed_step_thread);
    if (nrn_have_gaps) {
        nrnmpi_v_transfer();
        //if (has_non_v_gap)
        //{
        //    printf("before non v transfer\n");
        //    //gap_non_v_transfer();
        //}
        nrn_multithread_job(nrn_fixed_step_lastpart);
    }
#if NRNMPI
    if (nrn_threads[0]._stop_stepping) {
        nrn_spike_exchange(nrn_threads);
    }
#endif
    t = nrn_threads[0]._t;
}

/* better cache efficiency since a thread can do an entire minimum delay
integration interval before joining
*/
static int step_group_n;
static int step_group_begin;
static int step_group_end;
static progressbar* progress;

void initialize_progress_bar(int nstep) {
    if (nrnmpi_myid == 0) {
        printf("\n");
        progress = progressbar_new(" psolve", nstep);
    }
}

void update_progress_bar(int step, double time) {
    if (nrnmpi_myid == 0) {
        progressbar_update(progress, step, time);
    }
}

void finalize_progress_bar() {
    if (nrnmpi_myid == 0) {
        progressbar_finish(progress);
    }
}

void nrn_fixed_step_group_minimal(int n) {
    static int step = 0;
    dt2thread(dt);
    nrn_thread_table_check();
    step_group_n = n;
    step_group_begin = 0;
    step_group_end = 0;
    initialize_progress_bar(step_group_n);
    //printf("fixed_step_group_minimal\n");

    while (step_group_end < step_group_n) {
        //printf("begin:%d end:%d n:%d step:%d\n", step_group_begin, step_group_end, step_group_n, step);
        nrn_multithread_job(nrn_fixed_step_group_thread);
#if NRNMPI
        nrn_spike_exchange(nrn_threads);
#endif

#ifdef ENABLE_REPORTING
        nrn_flush_reports(nrn_threads[0]._t);
#endif
        if (stoprun) {
            break;
        }
        step++;
        step_group_begin = step_group_end;
        double actual_t = nrn_threads[0]._t;
        if (is_training || is_testing)
            actual_t = nrn_threads[0]._actual_t;
        update_progress_bar(step_group_end, nrn_threads[0]._t);
    }
    t = nrn_threads[0]._t;
    finalize_progress_bar();

    for (int ithread = 0; ithread < nrn_nthread; ithread++)
    {
        NrnThread* nt = &nrn_threads[ithread];
        if (need_record && nt->ncell > 0 && nt->nrec_param > 0)
        {
            if (nt->rec_times == NULL && nt->rec_type > -1)
            {
                if (nt->irec < (nt->rec_len * nt->rec_stride_len - 1) && (nt->irec > 0))
                {
                    assert(nt->irec / nt->rec_stride_len == nt->rec_last_len);
                    #pragma acc wait(nt->stream_id)
                    int num_all_vals = nt->rec_last_len * nt->nrec_param;
                    #pragma acc update host(nt->rec_vals[0:num_all_vals]) if (nt->compute_gpu)
                    printf("irec:%d all vals:%d\n", nt->irec, num_all_vals);
                    if (nt->fp_rec)
                    {
                        fwrite(nt->rec_vals, sizeof(float), num_all_vals, nt->fp_rec);
                    }
                }
            }
            else
            {
                #pragma acc wait(nt->stream_id)
                int num_all_vals = nt->rec_len * nt->nrec_param;
                #pragma acc update host(nt->rec_vals[0:num_all_vals]) if (nt->compute_gpu)
                printf("irec:%d all vals:%d\n", nt->irec, num_all_vals);
                if (nt->fp_rec)
                {
                    size_t n = fwrite(nt->rec_vals, sizeof(float), num_all_vals, nt->fp_rec);
                    printf("write size:%ld\n", n);
                    fclose(nt->fp_rec);
                    nt->fp_rec = NULL;
                }
            }
        }
        
    }
    //printf("fixed_step_group_minimal2\n");
}

void set_stim_and_labels(NrnThread* nth)
{
    //printf("set stim %d\n", nth->istim);
    if (nth->istim % 1000 == 0)
        printf("set stim istim:%d time:%f\n", nth->istim, nth->_actual_t);
    int batch_size = nth->batch_size;
    int stream_id = nth->stream_id;
    int i;
    int stim_len = nth->stim_len;
    int stim_len_per_net = stim_len / batch_size;
    double **stim_ptr = nth->stim_ptrs;
    double *stim_imgs = nth->imgs;
    int *labels = nth->labels;
    int img_len = nth->nimg * nth->img_size;
    double **in2hd_ptrs = nth->in2hd_ptrs;
    double **hd2out_ptrs = nth->hd2out_ptrs;
    #pragma acc parallel loop present(nth[0:1], stim_ptr[0:stim_len], stim_imgs[0:img_len]) if (nth->compute_gpu) async(stream_id)
    for (i = 0; i < stim_len; i++)
    {
        int inet = i * nth->batch_size / stim_len;
        int iimg = nth->istim + inet;
        if (iimg >= nth->nimg)
            iimg = iimg - nth->nimg;
        int ipixel = i % nth->img_size;
        *stim_ptr[i] = stim_imgs[iimg * nth->img_size + ipixel];
    }

    //update values on CPU
    if (nth->compute_gpu)
    {
        for (i = 0; i < stim_len; i++)
        {
            int inet = i * nth->batch_size / stim_len;
            int iimg = nth->istim + inet;
            if (iimg >= nth->nimg)
                iimg = iimg - nth->nimg;
            int ipixel = i % nth->img_size;
            *stim_ptr[i] = stim_imgs[iimg * nth->img_size + ipixel];
        }
        
    }

    if (is_training)
    {
        #pragma acc parallel loop present(nth[0:1], in2hd_ptrs[0:nth->in2hd_len], labels[0:nth->nimg]) if (nth->compute_gpu) async(stream_id)
        for (i = 0; i < nth->in2hd_len; i++)
        {
            *in2hd_ptrs[i] = 0;
            int len_per_net = nth->in2hd_len / nth->batch_size;
            int inet = i * nth->batch_size / nth->in2hd_len;
            int iimg = nth->istim + inet;
            if (iimg >= nth->nimg)
                iimg = iimg - nth->nimg;
            int idx_in_net = i % len_per_net;
            if (idx_in_net * 10 / len_per_net == labels[iimg])
                *in2hd_ptrs[i] = 1;
        }

        #pragma acc parallel loop present(nth[0:1], hd2out_ptrs[0:nth->hd2out_len], labels[0:nth->nimg]) if (nth->compute_gpu) async(stream_id)
        for (i = 0; i < nth->hd2out_len; i++)
        {
            *hd2out_ptrs[i] = 0;
            int len_per_net = nth->hd2out_len / nth->batch_size;
            int inet = i * nth->batch_size / nth->hd2out_len;
            int iimg = nth->istim + inet;
            if (iimg >= nth->nimg)
                iimg = iimg - nth->nimg;
            int idx_in_net = i % len_per_net;
            if (idx_in_net * 10 / len_per_net == labels[iimg])
                *hd2out_ptrs[i] = 1;
        }
    }
    

    nth->istim += nth->batch_size;
    if (nth->istim >= nth->nimg)
        nth->istim = nth->istim - nth->nimg;
    #pragma acc update device(nth->istim) if (nth->compute_gpu) async(stream_id)
    #pragma acc wait(stream_id)
}

void update_weights(NrnThread* nth)
{
    double **weights = nth->w_ptrs;
    double **delta_w = nth->dw_ptrs;
    double dt = nth->_dt;
    int nweights = nth->nweights;
    int batch_size = nth->batch_size;
    int nweights_per_net = nth->nweights_per_net;
    int i = 0, j = 0, stream_id = nth->stream_id;
    #pragma acc parallel loop present(nth[0:1], weights[0:nweights], delta_w[0:nweights]) if (nth->compute_gpu) async(stream_id)
    for (i = 0; i < nweights_per_net; i++)
    {
        double sum = 0.0;
        //#pragma acc loop seq 
        for (j = 0; j < batch_size; j++)
        {
            sum += *delta_w[j * nweights_per_net + i];
        }
        
        *weights[i] = *weights[i] + sum / (30.0 / dt) / batch_size;

        //#pragma acc loop 
        for (j = 1; j < batch_size; j++)
        {
            *weights[j * nweights_per_net + i] = *weights[i];
        }
    }

    double **in2hd_g_ptrs = nth->in2hd_g_ptrs;
    int nhidden = nth->nhidden;
    int nproj = nth->nproj;
    int ninput = nth->ninput;
    int nweights_in2hd = nth->ninput * nhidden * nproj;
    int nbias_in2hd = nhidden;
    int ng_in2hd = nweights_in2hd * 10;
    int ng_in2hd_bias = nbias_in2hd * 10;
    int ng_per_net = ng_in2hd + ng_in2hd_bias;
    int ng_in2hd_all = ng_in2hd * batch_size;
    int ng_in2hd_bias_all = ng_in2hd_bias * batch_size;
    int offset = nweights_in2hd + nbias_in2hd;
    int g_len = nth->in2hd_g_len;
    #pragma acc parallel loop present(nth[0:1], weights[0:nweights], in2hd_g_ptrs[0:g_len]) if (nth->compute_gpu) async(stream_id)
    for (i = 0; i < nth->in2hd_g_len; i++)
    {
        int i_in_net = i % ng_per_net;
        int h_id = 0, o_id = 0;
        if (i_in_net < ng_in2hd)
        {
           h_id = i_in_net % nweights_in2hd / (ninput * nproj);
           o_id = i_in_net / nweights_in2hd;
        }
        else
        {
            int j = i_in_net - ng_in2hd;
            h_id = j % nhidden;
            o_id = j / nhidden;
        }
        *in2hd_g_ptrs[i] = *weights[1*(offset + o_id * nhidden + h_id)];
    }
    //#pragma acc parallel loop present(nth[0:1], weights[0:nweights], in2hd_g_ptrs[0:ng_in2hd_all+ng_in2hd_bias_all]) if (nth->compute_gpu) async(stream_id)
    //for (i = 0; i < ng_in2hd_all; i++)
    //{
    //    int i_in_net = i % ng_in2hd;
    //    int h_id = i_in_net % (784 * 3 * 64) / (784 * 3);
    //    int o_id = i_in_net / (784 * 3 * 64);
    //    *in2hd_g_ptrs[i] = *weights[offset + o_id * 64 + h_id];
    //}
    ////printf("update_weights3\n");
    //
    //#pragma acc parallel loop present(nth[0:1], weights[0:nweights], in2hd_g_ptrs[0:ng_in2hd_all+ng_in2hd_bias_all]) if (nth->compute_gpu) async(stream_id)
    //for (i = ng_in2hd_all; i < ng_in2hd_all + ng_in2hd_bias_all; i++)
    //{
    //    int j = i - ng_in2hd_all;
    //    int idx_in_net = j % ng_in2hd_bias;
    //    int h_id = idx_in_net % 64;
    //    int o_id = idx_in_net / 64;
    //    //int h_id = (i - ng_in2hd) % 64;
    //    //int o_id = (i - ng_in2hd) / 64;
    //    *in2hd_g_ptrs[i] = *weights[offset + o_id * 64 + h_id];
    //}

    //#pragma acc parallel loop present(delta_w[0:nweights]) if (nth->compute_gpu) async(stream_id)
    //for (int i = 0; i < nweights; i++)
    //{
    //    *delta_w[i] = 0;
    //}

}

void init_v(NrnThread* nth)
{
    //printf("init_v\n");
    double* vec_v = nth->_actual_v;
    double **delta_w = nth->dw_ptrs;
    int nweights = nth->nweights;
    int i, stream_id = nth->stream_id;
    clear_event_queue();
    #pragma acc parallel loop present(nth[0:1], vec_v[0:nth->end]) if (nth->compute_gpu) async(stream_id)
    for (i = 0; i < nth->end; i++)
    {
        vec_v[i] = 0;
    }

    if (has_non_v_gap)
    {
        gap_non_v_transfer(nth);
    }

    

    nth->passed_time += nth->_t;
    t = 0.;
    for (int i = 0; i < nrn_nthread; i++)
    {
        NrnThread *nt = nrn_threads + i;
        nt->_t = t;
        #pragma acc update device(nt->_t) if (nt->compute_gpu) async(nt->stream_id)
        //#pragma acc update device(nt->passed_time) if (nt->compute_gpu) async(nt->stream_id)
        #pragma acc wait(nt->stream_id)
    }
    #pragma acc wait(nth->stream_id)
    NrnThreadMembList *tml;
    for (tml = nth->tml; tml; tml = tml->next)
    {
        mod_f_t s = memb_func[tml->index].initialize;
        if (s)
        {
            (*s)(nth, tml->ml, tml->index);
        }
    }


    init_net_events();
    nrn_deliver_events(nth);
    setup_tree_matrix_minimal(nth);
    nrn_deliver_events(nth);
}

void reset_and_update(NrnThread* nth)
{
    //printf("reset_and_update\n");
    //update weights
    update_weights(nth);
    //set stimuli and labels
    set_stim_and_labels(nth); 
    //initialize voltages and states
    init_v(nth);
    
}

void reset_stim_and_v(NrnThread* nth)
{
    set_stim_and_labels(nth);
    init_v(nth);
}

static void* nrn_fixed_step_group_thread(NrnThread* nth) {
    int i;
    nth->_stop_stepping = 0;
    int nstep_per_stim = 1;
    if (is_training || is_testing)
    {
        if (nth->ncell > 0)
        {
            nstep_per_stim = nth->sim_time_per_img / nth->_dt;
        }
        printf("dt:%f nstep_per_stim:%d\n", nth->_dt, nstep_per_stim);
    }

    for (i = step_group_begin; i < step_group_n; ++i) {
        //printf("i:%d n:%d\n", i, step_group_n);
        /*zyc just for training*/
        if (nth->ncell > 0 && i % nstep_per_stim == 0 && is_training && i > 0)
        {
            double t1 = nrn_wtime();
            reset_and_update(nth);
            #pragma acc wait(nth->stream_id)
            init_and_update_time += nrn_wtime() - t1;
            spikevec_time.clear();
            spikevec_gid.clear();
        }
        if (nth->ncell > 0 && i % nstep_per_stim == 0 && is_testing && i > 0)
        {
            reset_stim_and_v(nth);
            spikevec_time.clear();
            spikevec_gid.clear();
        }
        /*zyc just for training*/
        nrn_fixed_step_thread(nth);
        if (nth->_stop_stepping) {
            if (nth->id == 0) {
                step_group_end = i + 1;
            }
            nth->_stop_stepping = 0;
            return (void*)0;
        }
    }
    if (nth->id == 0) {
        step_group_end = step_group_n;
    }

    double total_time = 0.0;
    total_time = deliver_time + treeset_time + solve_time + second_cur_time + update_time + lastpart_time + init_and_update_time + play_continuous_time;
    printf("total time:%f\n", total_time);
    printf("play_continuous_time:%f\n", play_continuous_time);
    printf("deliver_time:%f\n", deliver_time);
    printf("treeset_time:%f\n", treeset_time);
    printf("solve_time:%f\n", solve_time);
    printf("second_cur_time:%f\n", second_cur_time);
    printf("update_time:%f\n", update_time);
    printf("lastpart_time:%f\n", lastpart_time);
    printf("init_and_update_time:%f\n", init_and_update_time);
    printf("nonv_gap_time:%f\n", nonv_gap_time);
    return (void*)0;
}

void update(NrnThread* _nt) {
    int i, i1, i2;
    i1 = 0;
    i2 = _nt->end;
#if defined(_OPENACC)
    int stream_id = _nt->stream_id;
#endif
    double* vec_v = &(VEC_V(0));
    double* vec_rhs = &(VEC_RHS(0));

    /* do not need to worry about linmod or extracellular*/
    if (secondorder) {
// clang-format off
        #pragma acc parallel loop present(          \
            vec_v[0:i2], vec_rhs[0:i2])             \
            if (_nt->compute_gpu) async(stream_id)
        // clang-format on
        for (i = i1; i < i2; ++i) {
            vec_v[i] += 2. * vec_rhs[i];
        }
    } else {
// clang-format off
        #pragma acc parallel loop present(              \
                vec_v[0:i2], vec_rhs[0:i2])             \
                if (_nt->compute_gpu) async(stream_id)
        // clang-format on
        for (i = i1; i < i2; ++i) {
            vec_v[i] += vec_rhs[i];
        }
    }

    // update_matrix_to_gpu(_nt);

    //#pragma acc wait(stream_id)
    //#pragma acc update host(vec_v[0:i2]) if (_nt->compute_gpu)
    //if (_nt->fp_rec)
    //{
    //    fwrite(vec_v, sizeof(double), i2, _nt->fp_rec);
    //}
    

    if (_nt->tml) {
        assert(_nt->tml->index == CAP);
        nrn_cur_capacitance(_nt, _nt->tml->ml, _nt->tml->index);
    }
}


void nonvint(NrnThread* _nt) {
    NrnThreadMembList* tml;
    if (nrn_have_gaps) {
        Instrumentor::phase_begin("gap-v-transfer");
        nrnthread_v_transfer(_nt);
        Instrumentor::phase_end("gap-v-transfer");
    }
    if (has_non_v_gap)
    {
        double t1 = nrn_wtime();
        gap_non_v_transfer(_nt);  
        #pragma acc wait(_nt->stream_id)
        nonv_gap_time += nrn_wtime() - t1;

    }
    errno = 0;

    Instrumentor::phase_begin("state-update");
    for (tml = _nt->tml; tml; tml = tml->next)
        if (memb_func[tml->index].state) {
            mod_f_t s = memb_func[tml->index].state;
            std::string ss("state-");
            ss += nrn_get_mechname(tml->index);
            Instrumentor::phase_begin(ss.c_str());
            (*s)(_nt, tml->ml, tml->index);
            Instrumentor::phase_end(ss.c_str());
#ifdef DEBUG
            if (errno) {
                hoc_warning("errno set during calculation of states", (char*)0);
            }
#endif
        }
    Instrumentor::phase_end("state-update");
}

void nrn_ba(NrnThread* nt, int bat) {
    NrnThreadBAList* tbl;
    for (tbl = nt->tbl[bat]; tbl; tbl = tbl->next) {
        printf("in nrn ba loop\n");
        mod_f_t f = tbl->bam->f;
        int type = tbl->bam->type;
        Memb_list* ml = tbl->ml;
        (*f)(nt, ml, type);
    }
}

static void* nrn_fixed_step_thread(NrnThread* nth) {
    /* check thresholds and deliver all (including binqueue)
       events up to t+dt/2 */
    Instrumentor::phase_begin("timestep");

    //#pragma acc wait(nth->stream_id)
    Instrumentor::phase_begin("deliver_events");
    double t1 = nrn_wtime();
    deliver_net_events(nth);
    //#pragma acc wait(nth->stream_id)
    deliver_time += nrn_wtime() - t1;
    Instrumentor::phase_end("deliver_events");

    nth->_t += .5 * nth->_dt;
    nth->_actual_t += .5 * nth->_dt;

    if (nth->ncell) {
#if defined(_OPENACC)
        int stream_id = nth->stream_id;
/*@todo: do we need to update nth->_t on GPU: Yes (Michael, but can launch kernel) */
// clang-format off
        #pragma acc update device(nth->_t, nth->_actual_t) if (nth->compute_gpu) async(stream_id)
        #pragma acc wait(stream_id)
// clang-format on
#endif
        t1 = nrn_wtime();
        fixed_play_continuous(nth);
        //#pragma acc wait(stream_id)
        play_continuous_time += nrn_wtime() - t1;

        Instrumentor::phase_begin("setup_tree_matrix");
        t1 = nrn_wtime();
        setup_tree_matrix_minimal(nth);
        //#pragma acc wait(stream_id)
        treeset_time += nrn_wtime() - t1;
        Instrumentor::phase_end("setup_tree_matrix");

        Instrumentor::phase_begin("matrix-solver");
        t1 = nrn_wtime();
        nrn_solve_minimal(nth);
        //#pragma acc wait(stream_id)
        solve_time += nrn_wtime() - t1;
        Instrumentor::phase_end("matrix-solver");

        Instrumentor::phase_begin("second_order_cur");
        t1 = nrn_wtime();
        second_order_cur(nth, secondorder);
        //#pragma acc wait(stream_id)
        second_cur_time += nrn_wtime() - t1;
        Instrumentor::phase_end("second_order_cur");

        Instrumentor::phase_begin("update");
        t1 = nrn_wtime();
        update(nth);
        //#pragma acc wait(stream_id)
        update_time += nrn_wtime() - t1;
        Instrumentor::phase_end("update");
    }
    if (!nrn_have_gaps) {
        t1 = nrn_wtime();
        nrn_fixed_step_lastpart(nth);
        //#pragma acc wait(nth->stream_id)
        lastpart_time += nrn_wtime() - t1;
    }
    Instrumentor::phase_end("timestep");

    if (need_record && nth->ncell > 0 && nth->nrec_param > 0)
    {
        int irec = nth->irec;
        int rec_len = nth->rec_len;
        int rec_stride = nth->rec_stride_len;
        //printf("irec:%d len:%d\n", irec, rec_len);
        if (nth->rec_times == NULL)
        {
            if (irec % rec_stride == 0)
            {
            //if (nth->_t >= 50 && nth->_t < 200)
            //{
                #pragma acc parallel loop present(nth[0:1]) if (nth->compute_gpu) async(nth->stream_id)
                for (int i = 0; i < nth->nrec_param; i++)
                {
                    nth->rec_vals[irec / rec_stride * nth->nrec_param + i] = (float)(*(nth->rec_ptrs[i]));
                    //nth->rec_vals[i * rec_len + irec / rec_stride] = (float)(*(nth->rec_ptrs[i]));
                    //nth->rec_vals[i * rec_len + irec] = (float)(*(nth->rec_ptrs[i]));
                    //if (irec > 400 && irec < 800)
                    //    printf("%f %f\n", _nt->rec_vals[i * rec_len + irec], _nt->_actual_v[211770]);
                }
                if (irec / rec_stride == nth->rec_len - 1)
                //if (irec == nth->rec_len - 1)
                {
                    printf("irec:%d rec_stride:%d rec_len:%d\n", irec, rec_stride, nth->rec_len);
                    int num_all_vals = nth->nrec_param * nth->rec_len;
                    printf("all_vals:%d\n", num_all_vals);
                    #pragma acc wait(nth->stream_id)
                    #pragma acc update host(nth->rec_vals[0:num_all_vals]) if (nth->compute_gpu)
                    if (nth->fp_rec)
                        fwrite(nth->rec_vals, sizeof(float), num_all_vals, nth->fp_rec);
                }
                //nth->irec = (irec + 1) % (nth->rec_len);
            //}
            }
            nth->irec = (irec + 1) % (nth->rec_len * rec_stride);
        }
        else
        {
            float actual_t = nth->_actual_t;
            //if (is_training || is_testing)
            //{
            //    actual_t += nth->passed_time; 
            //}
            if (nth->rec_type == -1)
            {
                if (nth->_t >= nth->mean_s_time && nth->_t < nth->mean_e_time)
                {
                    #pragma acc parallel loop present(nth[0:1]) if (nth->compute_gpu) async(nth->stream_id)
                    for (int i = 0; i < nth->nrec_param; i++)
                    {
                        nth->rec_vals[irec * nth->nrec_param + i] += (float)(*(nth->rec_ptrs[i]));
                    }
                }
                if (actual_t == nth->rec_times[irec])
                {
                    int mean_nstep = (nth->mean_e_time - nth->mean_s_time) / nth->_dt;
                    int irec = nth->irec;
                    int rec_len = nth->rec_len;
                    int nrec = nth->nrec_param;
                    #pragma acc parallel loop present(nth[0:1]) if (nth->compute_gpu) async(nth->stream_id)
                    for (int i = 0; i < nth->nrec_param; i++)
                    {
                        nth->rec_vals[irec * nth->nrec_param + i] /= mean_nstep;
                    }
                    nth->irec++;
                }
            }
            else if (actual_t == nth->rec_times[irec])
            {
                #pragma acc parallel loop present(nth[0:1]) if (nth->compute_gpu) async(nth->stream_id)
                for (int i = 0; i < nth->nrec_param; i++)
                {
                    nth->rec_vals[irec * nth->nrec_param + i] = (float)(*(nth->rec_ptrs[i]));
                }
                nth->irec = irec + 1;
            }
        }
        
    }
    return (void*)0;
}

void* nrn_fixed_step_lastpart(NrnThread* nth) {
    nth->_t += .5 * nth->_dt;
    nth->_actual_t += .5 * nth->_dt;

    if (nth->ncell) {
#if defined(_OPENACC)
        int stream_id = nth->stream_id;
/*@todo: do we need to update nth->_t on GPU */
// clang-format off
        #pragma acc update device(nth->_t, nth->_actual_t) if (nth->compute_gpu) async(stream_id)
        #pragma acc wait(stream_id)
// clang-format on
#endif

        fixed_play_continuous(nth);
        nonvint(nth);
        nrn_ba(nth, AFTER_SOLVE);
        nrn_ba(nth, BEFORE_STEP);
    }

    Instrumentor::phase_begin("deliver_events");
    nrn_deliver_events(nth); /* up to but not past texit */
    Instrumentor::phase_end("deliver_events");
    return (void*)0;
}
}  // namespace coreneuron

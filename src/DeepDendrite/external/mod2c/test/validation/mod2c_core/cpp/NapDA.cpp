/* Created by Language version: 6.2.0 */
/* VECTORIZED */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#undef PI
 
#include "coreneuron/utils/randoms/nrnran123.h"
#include "coreneuron/nrnoc/md1redef.h"
#include "coreneuron/nrnconf.h"
#include "coreneuron/nrnoc/membfunc.h"
#include "coreneuron/nrnoc/multicore.h"
#include "coreneuron/nrniv/nrniv_decl.h"
#include "coreneuron/nrniv/ivocvect.h"
#include "coreneuron/nrniv/nrn_acc_manager.h"
#include "coreneuron/mech/cfile/scoplib.h"

#include "coreneuron/scopmath_core/newton_struct.h"
#include "coreneuron/nrnoc/md2redef.h"
#include "coreneuron/nrnoc/register_mech.hpp"
#include "_kinderiv.h"
#if !NRNGPU
#if !defined(DISABLE_HOC_EXP)
#undef exp
#define exp hoc_Exp
#endif
#endif
 namespace coreneuron {
 
#define _thread_present_ /**/ , _thread[0:3] , _slist1[0:2], _dlist1[0:2] , _slist2[0:2] 
 
#if defined(_OPENACC) && !defined(DISABLE_OPENACC)
#include <openacc.h>
#if defined(PG_ACC_BUGS)
#define _PRAGMA_FOR_INIT_ACC_LOOP_ _Pragma("acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], nrn_ion_global_map[0:nrn_ion_global_map_size][0:3], _nt[0:1] _thread_present_) if(_nt->compute_gpu)")
#else
#define _PRAGMA_FOR_INIT_ACC_LOOP_ _Pragma("acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], nrn_ion_global_map[0:nrn_ion_global_map_size], _nt[0:1] _thread_present_) if(_nt->compute_gpu)")
#endif
#define _PRAGMA_FOR_STATE_ACC_LOOP_ _Pragma("acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], _nt[0:1], _ml[0:1] _thread_present_) if(_nt->compute_gpu) async(stream_id)")
#define _PRAGMA_FOR_CUR_ACC_LOOP_ _Pragma("acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], _vec_d[0:_nt->end], _vec_rhs[0:_nt->end], _nt[0:1] _thread_present_) if(_nt->compute_gpu) async(stream_id)")
#define _PRAGMA_FOR_CUR_SYN_ACC_LOOP_ _Pragma("acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], _vec_shadow_rhs[0:_nt->shadow_rhs_cnt], _vec_shadow_d[0:_nt->shadow_rhs_cnt], _vec_d[0:_nt->end], _vec_rhs[0:_nt->end], _nt[0:1]) if(_nt->compute_gpu) async(stream_id)")
#define _PRAGMA_FOR_NETRECV_ACC_LOOP_ _Pragma("acc parallel loop present(_pnt[0:_pnt_length], _nrb[0:1], _nt[0:1], nrn_threads[0:nrn_nthread]) if(_nt->compute_gpu) async(stream_id)")
#else
#define _PRAGMA_FOR_INIT_ACC_LOOP_ _Pragma("")
#define _PRAGMA_FOR_STATE_ACC_LOOP_ _Pragma("")
#define _PRAGMA_FOR_CUR_ACC_LOOP_ _Pragma("")
#define _PRAGMA_FOR_CUR_SYN_ACC_LOOP_ _Pragma("")
#define _PRAGMA_FOR_NETRECV_ACC_LOOP_ _Pragma("")
#endif
 
#if defined(__clang__)
#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma("clang loop vectorize(enable)")
#elif defined(__ICC) || defined(__INTEL_COMPILER)
#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma("ivdep")
#elif defined(__IBMC__) || defined(__IBMCPP__)
#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma("ibm independent_loop")
#elif defined(__PGI)
#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma("vector")
#elif defined(_CRAYC)
#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma("_CRI ivdep")
#elif defined(__GNUC__) || defined(__GNUG__)
#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma("GCC ivdep")
#else
#define _PRAGMA_FOR_VECTOR_LOOP_
#endif // _PRAGMA_FOR_VECTOR_LOOP_
 
#if !defined(LAYOUT)
/* 1 means AoS, >1 means AoSoA, <= 0 means SOA */
#define LAYOUT 1
#endif
#if LAYOUT >= 1
#define _STRIDE LAYOUT
#else
#define _STRIDE _cntml_padded + _iml
#endif
 
#define nrn_init _nrn_init__NapDA
#define nrn_cur _nrn_cur__NapDA
#define _nrn_current _nrn_current__NapDA
#define nrn_jacob _nrn_jacob__NapDA
#define nrn_state _nrn_state__NapDA
#define initmodel initmodel__NapDA
#define _net_receive _net_receive__NapDA
#define _net_init _net_init__NapDA
#define nrn_state_launcher nrn_state_NapDA_launcher
#define nrn_cur_launcher nrn_cur_NapDA_launcher
#define nrn_jacob_launcher nrn_jacob_NapDA_launcher 
#define states states_NapDA 
 
#undef _threadargscomma_
#undef _threadargsprotocomma_
#undef _threadargs_
#undef _threadargsproto_
 
#define _threadargscomma_ _iml, _cntml_padded, _p, _ppvar, _thread, _nt, v,
#define _threadargsprotocomma_ int _iml, int _cntml_padded, double* _p, Datum* _ppvar, ThreadDatum* _thread, NrnThread* _nt, double v,
#define _threadargs_ _iml, _cntml_padded, _p, _ppvar, _thread, _nt, v
#define _threadargsproto_ int _iml, int _cntml_padded, double* _p, Datum* _ppvar, ThreadDatum* _thread, NrnThread* _nt, double v
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 /* Thread safe. No static _p or _ppvar. */
 
#define t _nt->_t
#define dt _nt->_dt
#define gNapbar _p[0*_STRIDE]
#define ena _p[1*_STRIDE]
#define gna _p[2*_STRIDE]
#define m _p[3*_STRIDE]
#define h _p[4*_STRIDE]
#define ina _p[5*_STRIDE]
#define Dm _p[6*_STRIDE]
#define Dh _p[7*_STRIDE]
#define _v_unused _p[8*_STRIDE]
#define _g_unused _p[9*_STRIDE]
 
#ifndef NRN_PRCELLSTATE
#define NRN_PRCELLSTATE 0
#endif
#if NRN_PRCELLSTATE
#define _PRCELLSTATE_V _v_unused = _v;
#define _PRCELLSTATE_G _g_unused = _g;
#else
#define _PRCELLSTATE_V /**/
#define _PRCELLSTATE_G /**/
#endif
#define _ion_ina	_nt_data[_ppvar[0*_STRIDE]]
#define _ion_dinadv	_nt_data[_ppvar[1*_STRIDE]]
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 static int hoc_nrnpointerindex =  -1;
 static ThreadDatum* _extcall_thread;
 /* external NEURON variables */
 
#if 0 /*BBCORE*/
 /* declaration of user functions */
 static void _hoc_hbet(void);
 static void _hoc_half(void);
 static void _hoc_mbet(void);
 static void _hoc_malf(void);
 
#endif /*BBCORE*/
 static int _mechtype;
 
#if 0 /*BBCORE*/
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 "setdata_NapDA", _hoc_setdata,
 "hbet_NapDA", _hoc_hbet,
 "half_NapDA", _hoc_half,
 "mbet_NapDA", _hoc_mbet,
 "malf_NapDA", _hoc_malf,
 0, 0
};
 
#endif /*BBCORE*/
#define hbet hbet_NapDA
#define half half_NapDA
#define mbet mbet_NapDA
#define malf malf_NapDA
 #pragma acc routine seq
 inline double hbet( _threadargsprotocomma_ double );
 #pragma acc routine seq
 inline double half( _threadargsprotocomma_ double );
 #pragma acc routine seq
 inline double mbet( _threadargsprotocomma_ double );
 #pragma acc routine seq
 inline double malf( _threadargsprotocomma_ double );
 /* declare global and static user variables */
 
static void _acc_globals_update() {
 }

 
#if 0 /*BBCORE*/
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "gNapbar_NapDA", 0, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "gNapbar_NapDA", "mho/cm2",
 "ena_NapDA", "mV",
 "gna_NapDA", "mho/cm2",
 0,0
};
 
#endif /*BBCORE*/
 static double delta_t = 0.01;
#pragma acc declare copyin(delta_t)
 static double h0 = 0;
#pragma acc declare copyin(h0)
 static double m0 = 0;
#pragma acc declare copyin(m0)
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(double*, Datum*, int);
void nrn_init(NrnThread*, Memb_list*, int);
void nrn_state(NrnThread*, Memb_list*, int);
 void nrn_cur(NrnThread*, Memb_list*, int);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "6.2.0",
"NapDA",
 "gNapbar_NapDA",
 "ena_NapDA",
 0,
 "gna_NapDA",
 0,
 "m_NapDA",
 "h_NapDA",
 0,
 0};
 static int _na_type;
 
static void nrn_alloc(double* _p, Datum* _ppvar, int _type) {
 
#if 0 /*BBCORE*/
 	/*initialize range parameters*/
 	gNapbar = 0.0022;
 	ena = 55;
 prop_ion = need_memb(_na_sym);
 	_ppvar[0]._pval = &prop_ion->param[3]; /* ina */
 	_ppvar[1]._pval = &prop_ion->param[4]; /* _ion_dinadv */
 
#endif /* BBCORE */
 
}
 static void _initlists();
 static void _thread_mem_init(ThreadDatum*);
 static void _thread_cleanup(ThreadDatum*);
 static void _update_ion_pointer(Datum*);
 
#define _psize 10
#define _ppsize 2
 void _NapDA_reg() {
	int _vectorized = 1;
  _initlists();
 _mechtype = nrn_get_mechtype(_mechanism[1]);
 if (_mechtype == -1) return;
 _nrn_layout_reg(_mechtype, LAYOUT);
 _na_type = nrn_get_mechtype("na_ion"); 
#if 0 /*BBCORE*/
 	ion_reg("na", -10000.);
 	_na_sym = hoc_lookup("na_ion");
 
#endif /*BBCORE*/
 	register_mech(_mechanism, nrn_alloc,nrn_cur, NULL, nrn_state, nrn_init, hoc_nrnpointerindex, 4);
  _extcall_thread = (ThreadDatum*)ecalloc(3, sizeof(ThreadDatum));
  _thread_mem_init(_extcall_thread);
     _nrn_thread_reg1(_mechtype, _thread_mem_init);
     _nrn_thread_reg0(_mechtype, _thread_cleanup);
  hoc_register_prop_size(_mechtype, _psize, _ppsize);
  hoc_register_dparam_semantics(_mechtype, 0, "na_ion");
  hoc_register_dparam_semantics(_mechtype, 1, "na_ion");
 	hoc_register_var(hoc_scdoub, hoc_vdoub, NULL);
 }
static const char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
#define _deriv1_advance _thread[0]._i
#define _dith1 1
#define _newtonspace1 _thread[2]._pvoid
 
static int _ode_spec1(_threadargsproto_);
/*static int _ode_matsol1(_threadargsproto_);*/
 
/* _derivimplicit_ states _NapDA */
#ifndef INSIDE_NMODL
#define INSIDE_NMODL
#endif
 int _newton_states_NapDA(_threadargsproto_);
 
#define _slist2 _slist2_NapDA
int* _slist2;
#pragma acc declare create(_slist2)
  
#define _slist1 _slist1_NapDA
int* _slist1;
#pragma acc declare create(_slist1)

#define _dlist1 _dlist1_NapDA
int* _dlist1;
#pragma acc declare create(_dlist1)
 extern int states(_threadargsproto_);
 
/*CVODE*/
 static int _ode_spec1 (_threadargsproto_) {int _reset = 0; {
   Dm = ( 1.0 - m ) * malf ( _threadargscomma_ v ) - m * mbet ( _threadargscomma_ v ) ;
   Dh = ( ( 1.0 - h ) * half ( _threadargscomma_ v ) - h * hbet ( _threadargscomma_ v ) ) / 2.0 ;
   }
 return _reset;
}
 static int _ode_matsol1 (_threadargsproto_) {
 Dm = Dm  / (1. - dt*( (( ( - 1.0 ) ))*(malf ( _threadargscomma_ v )) - (1.0)*(mbet ( _threadargscomma_ v )) )) ;
 Dh = Dh  / (1. - dt*( ( ( (( ( - 1.0 ) ))*(half ( _threadargscomma_ v )) - (1.0)*(hbet ( _threadargscomma_ v )) ) ) / 2.0 )) ;
 return 0;
}
 /*END CVODE*/
 
int states (_threadargsproto_) {int _reset=0; int error = 0;
 { double* _savstate1 = (double*)_thread[_dith1]._pval;
 double* _dlist2 = (double*)(_thread[_dith1]._pval) + (2*_cntml_padded);
 {int _id; for(_id=0; _id < 2; _id++) { _savstate1[_id*_STRIDE] = _p[_slist1[_id]*_STRIDE];}}
 #pragma acc routine(nrn_newton_thread) seq
_reset = nrn_newton_thread((NewtonSpace*)_newtonspace1, 2,_slist2, _derivimplicit_states_NapDA, _dlist2,  _threadargs_);
 /*if(_reset) {abort_run(_reset);}*/ }
 
  return _reset;
}

int _newton_states_NapDA (_threadargsproto_) {  int _reset=0;
 { double* _savstate1 = (double*)_thread[_dith1]._pval;
 double* _dlist2 = (double*)(_thread[_dith1]._pval) + (2*_cntml_padded);
 int _counte = -1;
 {
   Dm = ( 1.0 - m ) * malf ( _threadargscomma_ v ) - m * mbet ( _threadargscomma_ v ) ;
   Dh = ( ( 1.0 - h ) * half ( _threadargscomma_ v ) - h * hbet ( _threadargscomma_ v ) ) / 2.0 ;
   {int _id; for(_id=0; _id < 2; _id++) {
if (_deriv1_advance) {
 _dlist2[(++_counte)*_STRIDE] = _p[_dlist1[_id]*_STRIDE] - (_p[_slist1[_id]*_STRIDE] - _savstate1[_id*_STRIDE])/dt;
 }else{
_dlist2[(++_counte)*_STRIDE] = _p[_slist1[_id]*_STRIDE] - _savstate1[_id*_STRIDE];}}}
 
  } }
 return _reset;}
 
double malf ( _threadargsprotocomma_ double _lv ) {
   double _lmalf;
 double _lva ;
 _lva = _lv + 12.0 ;
   if ( fabs ( _lva ) < 1e-04 ) {
     _lmalf = - 0.2816 * ( - 9.3 - _lva * 0.5 ) ;
     }
   else {
     _lmalf = - 0.2816 * _lva / ( - 1.0 + exp ( - _lva / 9.3 ) ) ;
     }
   
return _lmalf;
 }
 
#if 0 /*BBCORE*/
 
static void _hoc_malf(void) {
  double _r;
   double* _p; Datum* _ppvar; ThreadDatum* _thread; NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r =  malf ( _threadargs_, *getarg(1) ;
 hoc_retpushx(_r);
}
 
#endif /*BBCORE*/
 
double mbet ( _threadargsprotocomma_ double _lv ) {
   double _lmbet;
 double _lvb ;
 _lvb = _lv - 15.0 ;
   if ( fabs ( _lvb ) < 1e-04 ) {
     _lmbet = 0.2464 * ( 6.0 - _lvb * 0.5 ) ;
     }
   else {
     _lmbet = 0.2464 * _lvb / ( - 1.0 + exp ( _lvb / 6.0 ) ) ;
     }
   
return _lmbet;
 }
 
#if 0 /*BBCORE*/
 
static void _hoc_mbet(void) {
  double _r;
   double* _p; Datum* _ppvar; ThreadDatum* _thread; NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r =  mbet ( _threadargs_, *getarg(1) ;
 hoc_retpushx(_r);
}
 
#endif /*BBCORE*/
 
double half ( _threadargsprotocomma_ double _lv ) {
   double _lhalf;
 _lhalf = 2.8e-5 * ( exp ( - ( _lv + 42.8477 ) / 4.0248 ) ) ;
   
return _lhalf;
 }
 
#if 0 /*BBCORE*/
 
static void _hoc_half(void) {
  double _r;
   double* _p; Datum* _ppvar; ThreadDatum* _thread; NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r =  half ( _threadargs_, *getarg(1) ;
 hoc_retpushx(_r);
}
 
#endif /*BBCORE*/
 
double hbet ( _threadargsprotocomma_ double _lv ) {
   double _lhbet;
 _lhbet = 0.02 / ( 1.0 + exp ( - ( _lv - 413.9284 ) / 148.2589 ) ) ;
   
return _lhbet;
 }
 
#if 0 /*BBCORE*/
 
static void _hoc_hbet(void) {
  double _r;
   double* _p; Datum* _ppvar; ThreadDatum* _thread; NrnThread* _nt;
   if (_extcall_prop) {_p = _extcall_prop->param; _ppvar = _extcall_prop->dparam;}else{ _p = (double*)0; _ppvar = (Datum*)0; }
  _thread = _extcall_thread;
  _nt = nrn_threads;
 _r =  hbet ( _threadargs_, *getarg(1) ;
 hoc_retpushx(_r);
}
 
#endif /*BBCORE*/
 
static void _thread_mem_init(ThreadDatum* _thread) {
   _thread[_dith1]._pval = NULL; }
 
static void _thread_cleanup(ThreadDatum* _thread) {
   free( _thread[_dith1]._pval);
   nrn_destroy_newtonspace((NewtonSpace*) _newtonspace1);
 }
 static void _update_ion_pointer(Datum* _ppvar) {
 }

static inline void initmodel(_threadargsproto_) {
  int _i; double _save;{
  h = h0;
  m = m0;
 {
   m = malf ( _threadargscomma_ v ) / ( malf ( _threadargscomma_ v ) + mbet ( _threadargscomma_ v ) ) ;
   h = half ( _threadargscomma_ v ) / ( half ( _threadargscomma_ v ) + hbet ( _threadargscomma_ v ) ) ;
   }
 
}
}

void nrn_init(NrnThread* _nt, Memb_list* _ml, int _type){
double* _p; Datum* _ppvar; ThreadDatum* _thread;
double _v, v; int* _ni; int _iml, _cntml_padded, _cntml_actual;
    _ni = _ml->_nodeindices;
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
_thread = _ml->_thread;
  _deriv1_advance = 0;
  #ifdef _OPENACC
  #pragma acc update device (_deriv1_advance) if (_nt->compute_gpu)
  #endif
  if (!_newtonspace1) {
    _newtonspace1 = nrn_cons_newtonspace(2, _cntml_padded);
    _thread[_dith1]._pval = makevector(2*2*_cntml_padded*sizeof(double));
    #ifdef _OPENACC
    if (_nt->compute_gpu) {
      void* _d_ns = (void*)acc_deviceptr(_newtonspace1);
      double* _d_pd = (double*)acc_copyin(_thread[_dith1]._pval,2*2*_cntml_padded* sizeof(double));
      ThreadDatum* _d_td = (ThreadDatum*)acc_deviceptr(_thread);
      acc_memcpy_to_device(&(_d_td[2]._pvoid), &_d_ns, sizeof(void*));
      acc_memcpy_to_device(&(_d_td[_dith1]._pval), &_d_pd, sizeof(double*));
    }
    #endif
  }
_acc_globals_update();
double * _nt_data = _nt->_data;
double * _vec_v = _nt->_actual_v;
int stream_id = _nt->stream_id;
  if (_nrn_skip_initmodel == 0) {
#if LAYOUT == 1 /*AoS*/
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;
#elif LAYOUT == 0 /*SoA*/
 _p = _ml->_data; _ppvar = _ml->_pdata;
/* insert compiler dependent ivdep like pragma */
_PRAGMA_FOR_VECTOR_LOOP_
_PRAGMA_FOR_INIT_ACC_LOOP_
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
#else /* LAYOUT > 1 */ /*AoSoA*/
#error AoSoA not implemented.
for (;;) { /* help clang-format properly indent */
#endif
    int _nd_idx = _ni[_iml];
    _v = _vec_v[_nd_idx];
    _PRCELLSTATE_V
 v = _v;
 _PRCELLSTATE_V
 initmodel(_threadargs_);
 }
  }
  _deriv1_advance = 1;
  #ifdef _OPENACC
  #pragma acc update device (_deriv1_advance) if (_nt->compute_gpu)
  #endif
}

static double _nrn_current(_threadargsproto_, double _v){double _current=0.;v=_v;{ {
   gna = gNapbar * m * h ;
   ina = gna * ( v - ena ) ;
   }
 _current += ina;

} return _current;
}

#if defined(ENABLE_CUDA_INTERFACE) && defined(_OPENACC)
  void nrn_state_launcher(NrnThread*, Memb_list*, int, int);
  void nrn_jacob_launcher(NrnThread*, Memb_list*, int, int);
  void nrn_cur_launcher(NrnThread*, Memb_list*, int, int);
#endif


void nrn_cur(NrnThread* _nt, Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; ThreadDatum* _thread;
int* _ni; double _rhs, _g, _v, v; int _iml, _cntml_padded, _cntml_actual;
    _ni = _ml->_nodeindices;
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
_thread = _ml->_thread;
double * _vec_rhs = _nt->_actual_rhs;
double * _vec_d = _nt->_actual_d;

#if defined(ENABLE_CUDA_INTERFACE) && defined(_OPENACC) && !defined(DISABLE_OPENACC)
  NrnThread* d_nt = acc_deviceptr(_nt);
  Memb_list* d_ml = acc_deviceptr(_ml);
  nrn_cur_launcher(d_nt, d_ml, _type, _cntml_actual);
  return;
#endif

double * _nt_data = _nt->_data;
double * _vec_v = _nt->_actual_v;
int stream_id = _nt->stream_id;
#if LAYOUT == 1 /*AoS*/
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;
#elif LAYOUT == 0 /*SoA*/
 _p = _ml->_data; _ppvar = _ml->_pdata;
/* insert compiler dependent ivdep like pragma */
_PRAGMA_FOR_VECTOR_LOOP_
_PRAGMA_FOR_CUR_ACC_LOOP_
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
#else /* LAYOUT > 1 */ /*AoSoA*/
#error AoSoA not implemented.
for (;;) { /* help clang-format properly indent */
#endif
    int _nd_idx = _ni[_iml];
    _v = _vec_v[_nd_idx];
    _PRCELLSTATE_V
 _g = _nrn_current(_threadargs_, _v + .001);
 	{ double _dina;
  _dina = ina;
 _rhs = _nrn_current(_threadargs_, _v);
  _ion_dinadv += (_dina - ina)/.001 ;
 	}
 _g = (_g - _rhs)/.001;
  _ion_ina += ina ;
 _PRCELLSTATE_G
	_vec_rhs[_nd_idx] -= _rhs;
	_vec_d[_nd_idx] += _g;
 
}
 
}

void nrn_state(NrnThread* _nt, Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; ThreadDatum* _thread;
double v, _v = 0.0; int* _ni; int _iml, _cntml_padded, _cntml_actual;
    _ni = _ml->_nodeindices;
_cntml_actual = _ml->_nodecount;
_cntml_padded = _ml->_nodecount_padded;
_thread = _ml->_thread;

#if defined(ENABLE_CUDA_INTERFACE) && defined(_OPENACC) && !defined(DISABLE_OPENACC)
  NrnThread* d_nt = acc_deviceptr(_nt);
  Memb_list* d_ml = acc_deviceptr(_ml);
  nrn_state_launcher(d_nt, d_ml, _type, _cntml_actual);
  return;
#endif

double * _nt_data = _nt->_data;
double * _vec_v = _nt->_actual_v;
int stream_id = _nt->stream_id;
#if LAYOUT == 1 /*AoS*/
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
 _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;
#elif LAYOUT == 0 /*SoA*/
 _p = _ml->_data; _ppvar = _ml->_pdata;
/* insert compiler dependent ivdep like pragma */
_PRAGMA_FOR_VECTOR_LOOP_
_PRAGMA_FOR_STATE_ACC_LOOP_
for (_iml = 0; _iml < _cntml_actual; ++_iml) {
#else /* LAYOUT > 1 */ /*AoSoA*/
#error AoSoA not implemented.
for (;;) { /* help clang-format properly indent */
#endif
    int _nd_idx = _ni[_iml];
    _v = _vec_v[_nd_idx];
    _PRCELLSTATE_V
 v=_v;
{
 {  
  #if !defined(_derivimplicit_states_NapDA)
    #define _derivimplicit_states_NapDA 0
  #endif
  derivimplicit_thread(2, _slist1, _dlist1, _derivimplicit_states_NapDA, _threadargs_);
  } }}

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
 int _cntml_actual=1;
 int _cntml_padded=1;
 int _iml=0;
  if (!_first) return;
 
 _slist1 = (int*)malloc(sizeof(int)*2);
 _dlist1 = (int*)malloc(sizeof(int)*2);
 _slist1[0] = &(m) - _p;  _dlist1[0] = &(Dm) - _p;
 _slist1[1] = &(h) - _p;  _dlist1[1] = &(Dh) - _p;
 #pragma acc enter data copyin(_slist1[0:2])
 #pragma acc enter data copyin(_dlist1[0:2])

 _slist2 = (int*)malloc(sizeof(int)*2);
 _slist2[0] = &(h) - _p;
 _slist2[1] = &(m) - _p;
 #pragma acc enter data copyin(_slist2[0:2])

_first = 0;
}
} // namespace coreneuron_lib

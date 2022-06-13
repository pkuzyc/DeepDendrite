/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scoplib_ansi.h"
#undef PI
#define nil 0
#include "md1redef.h"
#include "section.h"
#include "nrniv_mf.h"
#include "md2redef.h"
 
#if METHOD3
extern int _method3;
#endif

#if !NRNGPU
#undef exp
#define exp hoc_Exp
extern double hoc_Exp(double);
#endif
 
#define nrn_init _nrn_init__IntFire4
#define _nrn_initial _nrn_initial__IntFire4
#define nrn_cur _nrn_cur__IntFire4
#define _nrn_current _nrn_current__IntFire4
#define nrn_jacob _nrn_jacob__IntFire4
#define nrn_state _nrn_state__IntFire4
#define _net_receive _net_receive__IntFire4 
#define factors factors__IntFire4 
#define fixprecondition fixprecondition__IntFire4 
#define newstates newstates__IntFire4 
#define update update__IntFire4 
 
#define _threadargscomma_ _p, _ppvar, _thread, _nt,
#define _threadargsprotocomma_ double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt,
#define _threadargs_ _p, _ppvar, _thread, _nt
#define _threadargsproto_ double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 /* Thread safe. No static _p or _ppvar. */
 
#define t _nt->_t
#define dt _nt->_dt
#define taue _p[0]
#define taui1 _p[1]
#define taui2 _p[2]
#define taum _p[3]
#define e _p[4]
#define i1 _p[5]
#define i2 _p[6]
#define m _p[7]
#define nself _p[8]
#define nexcite _p[9]
#define ninhibit _p[10]
#define ae _p[11]
#define ai1 _p[12]
#define ai2 _p[13]
#define enew _p[14]
#define i1new _p[15]
#define i2new _p[16]
#define mnew _p[17]
#define t0 _p[18]
#define ke _p[19]
#define ki1 _p[20]
#define ki2 _p[21]
#define km _p[22]
#define be _p[23]
#define bi1 _p[24]
#define bi2 _p[25]
#define a _p[26]
#define b _p[27]
#define tau_swap _p[28]
#define flag _p[29]
#define v _p[30]
#define _tsav _p[31]
#define _nd_area  *_ppvar[0]._pval
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 
#if defined(__cplusplus)
extern "C" {
#endif
 static int hoc_nrnpointerindex =  -1;
 static Datum* _extcall_thread;
 static Prop* _extcall_prop;
 /* external NEURON variables */
 /* declaration of user functions */
 static double _hoc_E();
 static double _hoc_I();
 static double _hoc_M();
 static double _hoc_deriv();
 static double _hoc_firetimebound();
 static double _hoc_factors();
 static double _hoc_fixprecondition();
 static double _hoc_newstates();
 static double _hoc_search();
 static double _hoc_update();
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_prop_size(int, int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
extern Memb_func* memb_func;
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static const char* nmodl_file_text;
static const char* nmodl_filename;
extern void hoc_reg_nmodl_text(int, const char*);
extern void hoc_reg_nmodl_filename(int, const char*);
#endif

 extern Prop* nrn_point_prop_;
 static int _pointtype;
 static void* _hoc_create_pnt(_ho) Object* _ho; { void* create_point_process();
 return create_point_process(_pointtype, _ho);
}
 static void _hoc_destroy_pnt();
 static double _hoc_loc_pnt(_vptr) void* _vptr; {double loc_point_process();
 return loc_point_process(_pointtype, _vptr);
}
 static double _hoc_has_loc(_vptr) void* _vptr; {double has_loc_point();
 return has_loc_point(_vptr);
}
 static double _hoc_get_loc_pnt(_vptr)void* _vptr; {
 double get_loc_point_process(); return (get_loc_point_process(_vptr));
}
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 }
 static void _hoc_setdata(void* _vptr) { Prop* _prop;
 _prop = ((Point_process*)_vptr)->_prop;
   _setdata(_prop);
 }
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 0,0
};
 static Member_func _member_func[] = {
 "loc", _hoc_loc_pnt,
 "has_loc", _hoc_has_loc,
 "get_loc", _hoc_get_loc_pnt,
 "E", _hoc_E,
 "I", _hoc_I,
 "M", _hoc_M,
 "deriv", _hoc_deriv,
 "firetimebound", _hoc_firetimebound,
 "factors", _hoc_factors,
 "fixprecondition", _hoc_fixprecondition,
 "newstates", _hoc_newstates,
 "search", _hoc_search,
 "update", _hoc_update,
 0, 0
};
#define E E_IntFire4
#define I I_IntFire4
#define M M_IntFire4
#define deriv deriv_IntFire4
#define firetimebound firetimebound_IntFire4
#define search search_IntFire4
 extern double E( _threadargsproto_ );
 extern double I( _threadargsproto_ );
 extern double M( _threadargsproto_ );
 extern double deriv( _threadargsprotocomma_ double );
 extern double firetimebound( _threadargsproto_ );
 extern double search( _threadargsproto_ );
 /* declare global and static user variables */
#define eps eps_IntFire4
 double eps = 1e-06;
#define ib ib_IntFire4
 double ib = 0;
#define taueps taueps_IntFire4
 double taueps = 0.01;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "eps_IntFire4", 1e-09, 0.01,
 "taueps_IntFire4", 1e-09, 1e+09,
 "taum", 0, 1e+09,
 "taui2", 0, 1e+09,
 "taui1", 0, 1e+09,
 "taue", 0, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "eps_IntFire4", "1e-9",
 "taueps_IntFire4", "1e-9",
 "taue", "ms",
 "taui1", "ms",
 "taui2", "ms",
 "taum", "ms",
 "ae", "1/ms",
 "ai1", "1/ms",
 "ai2", "1/ms",
 0,0
};
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "ib_IntFire4", &ib_IntFire4,
 "eps_IntFire4", &eps_IntFire4,
 "taueps_IntFire4", &taueps_IntFire4,
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(Prop*);
static void  nrn_init(_NrnThread*, _Memb_list*, int);
static void nrn_state(_NrnThread*, _Memb_list*, int);
 static void _hoc_destroy_pnt(_vptr) void* _vptr; {
   destroy_point_process(_vptr);
}
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"IntFire4",
 "taue",
 "taui1",
 "taui2",
 "taum",
 0,
 "e",
 "i1",
 "i2",
 "m",
 "nself",
 "nexcite",
 "ninhibit",
 "ae",
 "ai1",
 "ai2",
 0,
 0,
 0};
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
  if (nrn_point_prop_) {
	_prop->_alloc_seq = nrn_point_prop_->_alloc_seq;
	_p = nrn_point_prop_->param;
	_ppvar = nrn_point_prop_->dparam;
 }else{
 	_p = nrn_prop_data_alloc(_mechtype, 32, _prop);
 	/*initialize range parameters*/
 	taue = 5;
 	taui1 = 10;
 	taui2 = 20;
 	taum = 50;
  }
 	_prop->param = _p;
 	_prop->param_size = 32;
  if (!nrn_point_prop_) {
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 3, _prop);
  }
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 
}
 static void _initlists();
 
#define _tqitem &(_ppvar[2]._pvoid)
 static void _net_receive(Point_process*, double*, double);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, _NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _intfire4_reg_() {
	int _vectorized = 1;
  _initlists();
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,(void*)0, (void*)0, (void*)0, nrn_init,
	 hoc_nrnpointerindex, 1,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 32, 3);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
  hoc_register_dparam_semantics(_mechtype, 2, "netsend");
 add_nrn_artcell(_mechtype, 2);
 add_nrn_has_net_event(_mechtype);
 pnt_receive[_mechtype] = _net_receive;
 pnt_receive_size[_mechtype] = 1;
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 IntFire4 /raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/intfire4.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int factors(_threadargsproto_);
static int fixprecondition(_threadargsproto_);
static int newstates(_threadargsprotocomma_ double);
static int update(_threadargsproto_);
 
static int  newstates ( _threadargsprotocomma_ double _ld ) {
   double _lee , _lei1 , _lei2 , _lem ;
 _lee = exp ( - ke * _ld ) ;
   _lei1 = exp ( - ki1 * _ld ) ;
   _lei2 = exp ( - ki2 * _ld ) ;
   _lem = exp ( - km * _ld ) ;
   enew = e * _lee ;
   i1new = i1 * _lei1 ;
   i2new = i2 * _lei2 + bi1 * i1 * ( _lei2 - _lei1 ) ;
   mnew = m * _lem + be * e * ( _lem - _lee ) + ( bi2 * i2 + a * i1 ) * ( _lem - _lei2 ) - b * i1 * ( _lem - _lei1 ) ;
    return 0; }
 
static double _hoc_newstates(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 newstates ( _p, _ppvar, _thread, _nt, *getarg(1) );
 return(_r);
}
 
double M ( _threadargsproto_ ) {
   double _lM;
 newstates ( _threadargscomma_ t - t0 ) ;
   _lM = mnew ;
   
return _lM;
 }
 
static double _hoc_M(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  M ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
double E ( _threadargsproto_ ) {
   double _lE;
 newstates ( _threadargscomma_ t - t0 ) ;
   _lE = ae * enew ;
   
return _lE;
 }
 
static double _hoc_E(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  E ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
double I ( _threadargsproto_ ) {
   double _lI;
 newstates ( _threadargscomma_ t - t0 ) ;
   _lI = ai2 * i2new ;
   
return _lI;
 }
 
static double _hoc_I(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  I ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
static int  update ( _threadargsproto_ ) {
   e = enew ;
   i1 = i1new ;
   i2 = i2new ;
   m = mnew ;
   t0 = t ;
    return 0; }
 
static double _hoc_update(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 update ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
static int  fixprecondition ( _threadargsproto_ ) {
   if ( taui2 < 4.0 * taueps ) {
     taui2 = 4.0 * taueps ;
     }
   if ( taui1 < 3.0 * taueps ) {
     taui1 = 3.0 * taueps ;
     }
   if ( taue < 2.0 * taueps ) {
     taue = 2.0 * taueps ;
     }
   if ( taue > taui2 ) {
     tau_swap = taue ;
     taue = taui2 - taueps ;
     printf ( "Warning: Adjusted taue from %g  to %g  to ensure taue < taui2\n" , tau_swap , taue ) ;
     }
   else if ( taui2 - taue < taueps ) {
     taue = taui2 - taueps ;
     }
   if ( taui1 > taui2 ) {
     tau_swap = taui2 ;
     taui2 = taui1 ;
     taui1 = tau_swap ;
     printf ( "Warning: Swapped taui1 and taui2\n" ) ;
     }
   if ( taui2 - taui1 < taueps ) {
     taui1 = taui2 - taueps ;
     }
   if ( taum <= taui2 ) {
     if ( taui2 - taum < taueps ) {
       taum = taui2 - taueps ;
       }
     if ( fabs ( taui1 - taum ) < taueps ) {
       taum = taui1 - taueps ;
       }
     if ( fabs ( taui1 - taum ) < taueps ) {
       if ( taui1 - taum < 0.0 ) {
         taum = taui1 - taueps ;
         }
       else {
         taui1 = taum - taueps ;
         }
       }
     if ( fabs ( taue - taum ) < taueps ) {
       if ( taue - taum < 0.0 ) {
         taum = taue - taueps ;
         }
       else {
         taue = taum - taueps ;
         }
       }
     if ( fabs ( taui1 - taum ) < taueps ) {
       taum = taui1 - taueps ;
       }
     }
   else if ( taum - taui2 < taueps ) {
     taum = taui2 + taueps ;
     }
    return 0; }
 
static double _hoc_fixprecondition(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 fixprecondition ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
static int  factors ( _threadargsproto_ ) {
   double _ltp ;
 ke = 1.0 / taue ;
   ki1 = 1.0 / taui1 ;
   ki2 = 1.0 / taui2 ;
   km = 1.0 / taum ;
   _ltp = log ( km / ke ) / ( km - ke ) ;
   be = 1.0 / ( exp ( - km * _ltp ) - exp ( - ke * _ltp ) ) ;
   ae = be * ( ke - km ) ;
   _ltp = log ( ki2 / ki1 ) / ( ki2 - ki1 ) ;
   bi1 = 1.0 / ( exp ( - ki2 * _ltp ) - exp ( - ki1 * _ltp ) ) ;
   ai1 = bi1 * ( ki1 - ki2 ) ;
   e = 0.0 ;
   i1 = 1.0 ;
   i2 = 0.0 ;
   m = 0.0 ;
   bi2 = 1.0 ;
   ai2 = bi2 * ( ki2 - km ) ;
   a = bi2 * bi1 ;
   b = a * ( ki2 - km ) / ( ki1 - km ) ;
   _ltp = search ( _threadargs_ ) ;
   newstates ( _threadargscomma_ _ltp ) ;
   bi2 = 1.0 / mnew ;
   ai2 = bi2 * ( ki2 - km ) ;
   a = bi2 * bi1 ;
   b = a * ( ki2 - km ) / ( ki1 - km ) ;
   newstates ( _threadargscomma_ _ltp ) ;
   i1 = 0.0 ;
    return 0; }
 
static double _hoc_factors(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 factors ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
double deriv ( _threadargsprotocomma_ double _ld ) {
   double _lderiv;
 _lderiv = ( - km * exp ( - _ld * km ) + ki2 * exp ( - _ld * ki2 ) ) / ( ki2 - km ) - ( ( - km * exp ( - _ld * km ) + ki1 * exp ( - _ld * ki1 ) ) ) / ( ki1 - km ) ;
   
return _lderiv;
 }
 
static double _hoc_deriv(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  deriv ( _p, _ppvar, _thread, _nt, *getarg(1) );
 return(_r);
}
 
double search ( _threadargsproto_ ) {
   double _lsearch;
 double _lx , _lt1 , _lt2 ;
 _lt1 = 1.0 ;
   flag = 0.0 ;
   if ( deriv ( _threadargscomma_ _lt1 ) < 0.0 ) {
     while ( deriv ( _threadargscomma_ _lt1 ) < 0.0  && _lt1 > 1e-9 ) {
       _lt2 = _lt1 ;
       _lt1 = _lt1 / 10.0 ;
       }
     if ( deriv ( _threadargscomma_ _lt1 ) < 0.0 ) {
       printf ( "Error wrong deriv(t1): t1=%g deriv(t1)=%g\n" , _lt1 , deriv ( _threadargscomma_ _lt1 ) ) ;
       flag = 1.0 ;
       _lsearch = 1e-9 ;
       }
     }
   else {
     _lt2 = _lt1 ;
     while ( deriv ( _threadargscomma_ _lt2 ) > 0.0  && _lt2 < 1e9 ) {
       _lt1 = _lt2 ;
       _lt2 = _lt2 * 10.0 ;
       }
     if ( deriv ( _threadargscomma_ _lt2 ) > 0.0 ) {
       printf ( "Error wrong deriv(t2): t2=%g deriv(t2)=%g\n" , _lt2 , deriv ( _threadargscomma_ _lt2 ) ) ;
       flag = 1.0 ;
       _lsearch = 1e9 ;
       }
     }
   while ( _lt2 - _lt1 > 1e-6  && flag  == 0.0 ) {
     _lsearch = ( _lt1 + _lt2 ) / 2.0 ;
     _lx = deriv ( _threadargscomma_ _lsearch ) ;
     if ( _lx > 0.0 ) {
       _lt1 = _lsearch ;
       }
     else {
       _lt2 = _lsearch ;
       }
     }
   
return _lsearch;
 }
 
static double _hoc_search(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  search ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
static void _net_receive (_pnt, _args, _lflag) Point_process* _pnt; double* _args; double _lflag; 
{  double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _thread = (Datum*)0; _nt = (_NrnThread*)_pnt->_vnt;   _p = _pnt->_prop->param; _ppvar = _pnt->_prop->dparam;
  if (_tsav > t){ extern char* hoc_object_name(); hoc_execerror(hoc_object_name(_pnt->ob), ":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay");}
 _tsav = t;   if (_lflag == 1. ) {*(_tqitem) = 0;}
 {
   newstates ( _threadargscomma_ t - t0 ) ;
   update ( _threadargs_ ) ;
   if ( m > 1.0 - eps ) {
     net_event ( _pnt, t ) ;
     m = 0.0 ;
     }
   if ( _lflag  == 1.0 ) {
     nself = nself + 1.0 ;
     artcell_net_send ( _tqitem, _args, _pnt, t +  firetimebound ( _threadargs_ ) , 1.0 ) ;
     }
   else {
     if ( _args[0] > 0.0 ) {
       nexcite = nexcite + 1.0 ;
       e = e + _args[0] ;
       }
     else {
       ninhibit = ninhibit + 1.0 ;
       i1 = i1 + _args[0] ;
       }
     artcell_net_move ( _tqitem, _pnt, firetimebound ( _threadargs_ ) + t ) ;
     }
   } }
 
double firetimebound ( _threadargsproto_ ) {
   double _lfiretimebound;
 double _lslope ;
 _lslope = - km * m + ae * e + ai2 * i2 ;
   if ( _lslope <= 1e-9 ) {
     _lfiretimebound = 1e9 ;
     }
   else {
     _lfiretimebound = ( 1.0 - m ) / _lslope ;
     }
   
return _lfiretimebound;
 }
 
static double _hoc_firetimebound(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  firetimebound ( _p, _ppvar, _thread, _nt );
 return(_r);
}

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
 {
   fixprecondition ( _threadargs_ ) ;
   factors ( _threadargs_ ) ;
   e = 0.0 ;
   i1 = 0.0 ;
   i2 = 0.0 ;
   m = 0.0 ;
   t0 = t ;
   artcell_net_send ( _tqitem, (double*)0, _ppvar[1]._pvoid, t +  firetimebound ( _threadargs_ ) , 1.0 ) ;
   nself = 0.0 ;
   nexcite = 0.0 ;
   ninhibit = 0.0 ;
   }

}
}

static void nrn_init(_NrnThread* _nt, _Memb_list* _ml, int _type){
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _tsav = -1e20;
 initmodel(_p, _ppvar, _thread, _nt);
}
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _v){double _current=0.;v=_v;{
} return _current;
}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _nd = _ml->_nodelist[_iml];
 v=_v;
{
}}

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
  if (!_first) return;
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "/raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/intfire4.mod";
static const char* nmodl_file_text = 
  "COMMENT\n"
  "Authors:    Original (taum > taui2 > taui1 > taue)   :   M. Hines and N.T. Carnevale\n"
  "            Improved (taui2 > taue and taui2 > taui1):   R.A.J. van Elburg\n"
  "\n"
  "Artificial cell with fast exponential decay excitatory current and\n"
  "slower alpha function like inhibitory current and arbitrary membrane time constant.\n"
  "Fires when membrane state = 1. On firing, only the membrane state returns to 0\n"
  "\n"
  "Although the four states, e, i1, i2, m are available, plotting will be\n"
  "more pleasing if one uses the function E, I, and M\n"
  "\n"
  "Fast excitatory current, E(t), has single exponential decay with time constant, taue.\n"
  "\n"
  "Slower inhibitory current, I2(t), is alpha function like and has two time contants,\n"
  "taui1 and  taui2.\n"
  "\n"
  "The leaky integrator membrane has time constant taum.\n"
  "The total current is the input to this integrator.\n"
  "\n"
  "Cell fires when state m passes 1\n"
  "and after firing, m = 0.\n"
  "\n"
  "Require  taui2 > taue and  taui2 > taui1\n"
  "or in other words ki1 > ki2 and ke > ki2 \n"
  "\n"
  "States are normalized so that an excitatory weight of 1 produces\n"
  "a maximum e of 1 and a maximum m of 1\n"
  "Also an inhibitory weight of -1 produces a maximum i2 of -1\n"
  "and a maximum m of -1.\n"
  "\n"
  "Basic equations:\n"
  " dE(t)/dt  = -ke*E(t)\n"
  " dI1(t)/dt = -ki1*I1(t)\n"
  " dI2(t)/dt = -ki2*I2(t) + ai1*I1(t)\n"
  " dM(t)/dt  = -km*M(t) + ae*E(t) + ai2*I2(t)\n"
  "\n"
  "The initial conditions are e, i1, i2, and m\n"
  "\n"
  "The analytic solution is\n"
  "\n"
  "E(t) = e*exp(-ke*t)\n"
  "I1(t) = i1*exp(-ki1*t)\n"
  "I2(t) = i2*exp(-ki2*t) + bi1*i1*(exp(-ki2*t) - exp(-ki1*t))\n"
  "M(t) = m * exp(-km*t)\n"
  "	+ be*e*(exp(-km*t) - exp(-ke*t))\n"
  "	+ bi2*i2*(exp(-km*t) - exp(-ki2*t))\n"
  "	+ bi2*bi1*i1 * (exp(-km*t) - exp(-ki2*t))\n"
  "	- bi2*bi1*i1*(ki2 - km)/(ki1 - km)*(exp(-km*t) - exp(-ki1*t))\n"
  "\n"
  "The derivation of a lower bound on the first passage time based on Newton \n"
  "iteration is possible if we assume that taue<=taui2, i.e. that the \n"
  "inhibition outlasts excitation. The validity of the Newton iteration for setting \n"
  "a lower bound on threshold passage time follows from a physical analysis of the \n"
  "different contributing currents. The parameters taui1 are involved in controlling \n"
  "the time course of the growth of the inhibitory current. Therefore the taui1\n"
  "associated current contributes to a growth of inhibition and can be ignored when trying to find a \n"
  "lower bound for the threshold passage time because it contributes only to its delay. \n"
  "The other contributions to the membrane potential change are the \n"
  "leak current associated with the membrane time constant taum and the \n"
  "exponentially decaying (parts) of the synaptic currents. The leak current is \n"
  "maximal close to threshold and can therefore only grow when approaching \n"
  "threshold. So if we use the present value in a calculation of threshold passage \n"
  "time while we are approaching threshold we will underestimate threshold passage \n"
  "time. The exponentially decaying (parts) of the synaptic currents consist of an \n"
  "excitatory and an inhibitory part, if we assume taue<taui2 we can see \n"
  "that the inhibitory parts outlast the excitatory currents and therefore if we \n"
  "assume that m changes according to the present values of these currents we \n"
  "underestimate threshold passage time. Because the sum of these currents is m'=\n"
  "dm/dt and Newton iteration states that m(t)~m(t_0)+m'*(t-t_0) we find that \n"
  "Newton iteration underestimates threshold passsage time. If the membrane \n"
  "potential moves away from threshold toward rest, the excitatory current is \n"
  "smaller then leak current and inhibitory currents combined, because it decays \n"
  "faster then the inhibitory current it will also at future times not be able to \n"
  "overcome the leak current at the present membrane potential and hence not be \n"
  "able to rise above it and reach threshold.\n"
  "\n"
  "A formal proof based on analysis of the exact solution of this mechanism is possible and can be found in:\n"
  "    R.A.J. van Elburg and A. van Ooyen, `Generalization of the event-based Carnevale-Hines integration scheme\n"
  "for integrate-and-fire models', ..., 2008/2009.\n"
  "and is based on a formal proof for the original version which can be found in:\n"
  "    Carnevale, N.T.  and Hines, M.L., `The NEURON Book',Cambridge University Press,Cambridge, UK (2006).\n"
  "ENDCOMMENT\n"
  "\n"
  "NEURON {\n"
  "	ARTIFICIAL_CELL IntFire4\n"
  "	RANGE taue, taui1, taui2, taum, e, i1, i2, m, ae, ai2, ai1\n"
  "	RANGE nself, nexcite, ninhibit\n"
  "	GLOBAL eps, taueps\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	taue = 5 (ms)      <0,1e9>\n"
  "	taui1 = 10 (ms)    <0,1e9>\n"
  "	taui2 = 20 (ms)    <0,1e9>\n"
  "	taum = 50 (ms)     <0,1e9>\n"
  "	ib = 0\n"
  "	eps = 1e-6         <1e-9,0.01>\n"
  "	taueps = 0.01      <1e-9,1e9>\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	e i1 i2 m\n"
  "	enew i1new i2new mnew\n"
  "	t0 (ms)\n"
  "	nself nexcite ninhibit\n"
  "\n"
  "	ke (1/ms) ki1 (1/ms) ki2 (1/ms) km (1/ms)\n"
  "	ae (1/ms) ai1 (1/ms) ai2 (1/ms)\n"
  "	be bi1 bi2\n"
  "	a b\n"
  "	tau_swap (ms)\n"
  "	flag (1)\n"
  "}\n"
  "\n"
  "PROCEDURE newstates(d(ms)) {\n"
  "	LOCAL ee, ei1, ei2, em\n"
  "	\n"
  "	ee = exp(-ke*d)\n"
  "	ei1 = exp(-ki1*d)\n"
  "	ei2= exp(-ki2*d)\n"
  "	em = exp(-km*d)\n"
  "	enew = e*ee\n"
  "	i1new = i1*ei1\n"
  "	i2new = i2*ei2 + bi1*i1*(ei2 - ei1)\n"
  "	mnew = m*em\n"
  "		+ be*e*(em - ee)\n"
  "		+ (bi2*i2 + a*i1)*(em - ei2)\n"
  "		- b*i1*(em - ei1)\n"
  "}\n"
  "\n"
  "FUNCTION M() {\n"
  "	newstates(t - t0)\n"
  "	M = mnew\n"
  "}\n"
  "\n"
  "FUNCTION E() {\n"
  "	newstates(t - t0)\n"
  "	E = ae*enew\n"
  "}\n"
  "\n"
  "FUNCTION I() {\n"
  "	newstates(t - t0)\n"
  "	I = ai2*i2new\n"
  "}\n"
  "\n"
  "PROCEDURE update() {\n"
  "	e = enew\n"
  "	i1 = i1new\n"
  "	i2 = i2new\n"
  "	m = mnew\n"
  "	t0 = t\n"
  "}\n"
  "\n"
  "PROCEDURE fixprecondition() {\n"
  "    \n"
  "	: and force assertion for correctness of algorithm\n"
  "	: Preconditions:\n"
  "    : taue  < taui2 \n"
  "    : taui1 < taui2\n"
  "    : To avoid division by zero errors, the need for alpha function [x*exp(-x)] \n"
  "    : solutions  and poor convergence we also impose\n"
  "    : fabs(taux-tauy) >= taueps\n"
  "    \n"
  "    : The checks on the preconditions can lead to 3 consecutive shifts in taum \n"
  "    : and a smaller number of shifts in the other parameters to prevent this \n"
  "    : shift from bringing taum at or below zero we need to ensure \n"
  "    : that we keep sufficient distance from zero.\n"
  "    if(taui2 < 4*taueps){\n"
  "        taui2=4*taueps\n"
  "    }\n"
  "    if(taui1 < 3*taueps){\n"
  "        taui1=3*taueps\n"
  "    }\n"
  "    \n"
  "    if(taue < 2*taueps){\n"
  "        taue=2*taueps\n"
  "    }\n"
  "    \n"
  "    : taue  < taui2  \n"
  "    if (taue > taui2) { \n"
  "        tau_swap=taue\n"
  "        taue = taui2 - taueps \n"
  "        printf(\"Warning: Adjusted taue from %g  to %g  to ensure taue < taui2\\n\",tau_swap,taue)        \n"
  "    } else if (taui2-taue < taueps){\n"
  "        taue = taui2 - taueps \n"
  "    }\n"
  "\n"
  "    : taui1 < taui2, after this taui2 is fixed\n"
  "	if (taui1 > taui2) {\n"
  "        tau_swap=taui2\n"
  "        taui2=taui1\n"
  "        taui1=tau_swap\n"
  "        printf(\"Warning: Swapped taui1 and taui2\\n\")   \n"
  "    }   \n"
  "    \n"
  "    : Avoid division by zero errors and poor convergence and impose\n"
  "    : fabs(taux-tauy) >= taueps\n"
  "    if (taui2-taui1 < taueps){          :  taui1 < taui2 --> taui2-taui1 is positive\n"
  "        taui1 = taui2 - taueps \n"
  "    }\n"
  "\n"
  "    if (taum <= taui2) {\n"
  "        if (taui2 -taum < taueps){      :  (taum <= taui2) --> taui2 -taum is positive definite\n"
  "            taum=taui2-taueps\n"
  "        }\n"
  "    \n"
  "        if (fabs(taui1 -taum) < taueps){\n"
  "            taum=taui1-taueps\n"
  "        }\n"
  "    \n"
  "        if (fabs(taui1 -taum) < taueps){\n"
  "            if(taui1 -taum < 0){\n"
  "                taum=taui1-taueps\n"
  "            } else{\n"
  "                taui1=taum-taueps\n"
  "            }\n"
  "        }\n"
  "    \n"
  "        if (fabs(taue -taum) < taueps){\n"
  "            if(taue -taum < 0){\n"
  "                taum=taue-taueps\n"
  "            } else{\n"
  "                taue=taum-taueps\n"
  "            }\n"
  "        }\n"
  "    \n"
  "        if (fabs(taui1 -taum) < taueps){\n"
  "            taum=taui1-taueps\n"
  "        }\n"
  "    \n"
  "    } else if (taum-taui2 < taueps){    :  (taum > taui2) --> taum -taui2 is positive \n"
  "        taum=taui2+taueps\n"
  "    }\n"
  "}\n"
  "\n"
  "PROCEDURE factors() {\n"
  "	LOCAL tp\n"
  "    \n"
  "	ke=1/taue  ki1=1/taui1  ki2=1/taui2  km=1/taum\n"
  "\n"
  "	: normalize so the peak magnitude of m is 1 when single e of 1\n"
  "	tp = log(km/ke)/(km - ke)\n"
  "	be = 1/(exp(-km*tp) - exp(-ke*tp))\n"
  "	ae = be*(ke - km)\n"
  "        :printf(\"INITIAL be=%g tp=%g \\n\", be, tp)\n"
  "	\n"
  "    : normalize so the peak magnitude of i2 is 1 when single i of 1\n"
  "	tp = log(ki2/ki1)/(ki2 - ki1)\n"
  "	bi1 = 1/(exp(-ki2*tp) - exp(-ki1*tp))\n"
  "	ai1 = bi1*(ki1 - ki2)\n"
  "    :printf(\"INITIAL bi1=%g tp=%g \\n\", bi1, tp)\n"
  "	\n"
  "    : normalize so the peak magnitude of m is 1 when single i of 1\n"
  "	: first set up enough so we can use newstates()\n"
  "	e = 0\n"
  "	i1 = 1\n"
  "	i2 = 0\n"
  "	m = 0\n"
  "	bi2 = 1\n"
  "	ai2 = bi2*(ki2 - km)\n"
  "	a = bi2*bi1\n"
  "	b = a*(ki2 - km)/(ki1 - km)\n"
  "	\n"
  "    :find the 0 of dm/dt\n"
  "	tp = search()\n"
  "	\n"
  "    : now compute normalization factor and reset constants that depend on it\n"
  "	newstates(tp)\n"
  "	bi2 = 1/mnew\n"
  "	ai2 = bi2*(ki2 - km)\n"
  "	a = bi2*bi1\n"
  "	b = a*(ki2 - km)/(ki1 - km)\n"
  "	\n"
  "	: now newstates(tp) should set mnew=1\n"
  "	newstates(tp)\n"
  "    :printf(\"INITIAL bi2=%g tp=%g mnew=%g\\n\", bi2, tp, mnew)\n"
  "	i1 = 0\n"
  "}\n"
  "\n"
  "FUNCTION deriv(d(ms)) (/ms2) { : proportional, not exact derivative but sign true (provided ki1 > ki2)\n"
  "    deriv= ( - km *exp( - d*km ) + ki2*exp( - d*ki2))/(ki2 - km) -(( - km*exp( - d*km) + ki1*exp( - d*ki1)))/(ki1 - km )\n"
  "}\n"
  "\n"
  "FUNCTION search() (ms) {\n"
  "	LOCAL x, t1, t2\n"
  "	: should only do this when tau's change\n"
  "	\n"
  "    : exponential search for two initial t values for the chord search one factor\n"
  "    : of 10 separated, with the low value t1 before (deriv(t1) < 0) and the high\n"
  "    : value t2 after (deriv(t2) > 0) the extremal reached by m due to an isolated\n"
  "    : i1 current. For this calculation the i1 current is taken to be depolarizing\n"
  "    : and therefore the extremum is actually a maximum. During the simulation i1 is \n"
  "    : constrained to be hyperpolarizing and then the extremum will be a minimum \n"
  "    : occuring at the same location.\n"
  "	t1=1\n"
  "	flag=0\n"
  "    if(deriv(t1) < 0  ){\n"
  "        while ( deriv(t1) < 0 && t1>1e-9 ){\n"
  "            t2=t1\n"
  "            t1=t1/10\n"
  "        }\n"
  "        if (deriv(t1) < 0) { \n"
  "    		printf(\"Error wrong deriv(t1): t1=%g deriv(t1)=%g\\n\", t1, deriv(t1))\n"
  "    		flag=1\n"
  "            search = 1e-9\n"
  "    	}\n"
  "    }else{\n"
  "        t2=t1\n"
  "        while (deriv(t2) > 0 && t2<1e9 ){\n"
  "            t1=t2\n"
  "            t2=t2*10\n"
  "        }\n"
  "    	if (deriv(t2) > 0) { \n"
  "    		printf(\"Error wrong deriv(t2): t2=%g deriv(t2)=%g\\n\", t2, deriv(t2))\n"
  "    		flag=1\n"
  "    		search = 1e9\n"
  "    	}\n"
  "    }\n"
  "    : printf(\"search  t1=%g x1=%g t2=%g x2=%g\\n\", t1, deriv(t1), t2, deriv(t2))\n"
  "    \n"
  "    \n"
  "    : chord search for the  maximum in m(t) \n"
  "    while (t2 - t1 > 1e-6 && flag==0) {\n"
  "		search = (t1+t2)/2\n"
  "		x = deriv(search)\n"
  "		if (x > 0) {\n"
  "			t1 = search\n"
  "		}else{\n"
  "			t2 = search\n"
  "		}\n"
  "        :printf(\"search  t1=%g x1=%g t2=%g x2=%g\\n\", t1, deriv(t1), t2, deriv(t2))\n"
  "	}			\n"
  "}\n"
  "\n"
  "\n"
  "INITIAL {\n"
  "    \n"
  "	fixprecondition()\n"
  "	factors()\n"
  "	e = 0\n"
  "	i1 = 0\n"
  "	i2 = 0\n"
  "	m = 0\n"
  "	t0 = t\n"
  "	net_send(firetimebound(), 1)\n"
  "\n"
  "	nself=0\n"
  "	nexcite=0\n"
  "	ninhibit=0\n"
  "}\n"
  "\n"
  "NET_RECEIVE (w) {\n"
  "	newstates(t-t0)\n"
  "	update()	\n"
  "    :printf(\"event %g t=%g e=%g i1=%g i2=%g m=%g\\n\", flag, t, e, i1, i2, m)\n"
  "	if (m > 1-eps) { : time to fire\n"
  "        :printf(\"fire\\n\")\n"
  "		net_event(t)\n"
  "		m = 0\n"
  "	}\n"
  "	if (flag == 1) { :self event\n"
  "		nself = nself + 1\n"
  "		net_send(firetimebound(), 1)\n"
  "	}else{\n"
  "		if (w > 0) {\n"
  "			nexcite = nexcite + 1\n"
  "			e = e + w\n"
  "		}else{\n"
  "			ninhibit = ninhibit + 1\n"
  "			i1 = i1 + w\n"
  "		}\n"
  "        :printf(\"w=%g e=%g i1=%g\\n\", w, e, i1)\n"
  "		net_move(firetimebound() + t)\n"
  "	}\n"
  "}\n"
  "\n"
  "FUNCTION firetimebound() (ms) {\n"
  "	LOCAL slope\n"
  "	slope = -km*m + ae*e + ai2*i2\n"
  "	if (slope <= 1e-9) {\n"
  "		firetimebound = 1e9\n"
  "	}else{\n"
  "		firetimebound = (1 - m)/slope\n"
  "	}\n"
  "}\n"
  ;
#endif

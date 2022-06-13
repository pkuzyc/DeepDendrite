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
 
#define nrn_init _nrn_init__IntFire2
#define _nrn_initial _nrn_initial__IntFire2
#define nrn_cur _nrn_cur__IntFire2
#define _nrn_current _nrn_current__IntFire2
#define nrn_jacob _nrn_jacob__IntFire2
#define nrn_state _nrn_state__IntFire2
#define _net_receive _net_receive__IntFire2 
 
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
#define taus _p[0]
#define taum _p[1]
#define ib _p[2]
#define i _p[3]
#define m _p[4]
#define t0 _p[5]
#define v _p[6]
#define _tsav _p[7]
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
 static double _hoc_I();
 static double _hoc_M();
 static double _hoc_firetime();
 static double _hoc_newton1();
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
 "I", _hoc_I,
 "M", _hoc_M,
 "firetime", _hoc_firetime,
 "newton1", _hoc_newton1,
 0, 0
};
#define I I_IntFire2
#define M M_IntFire2
#define firetime firetime_IntFire2
#define newton1 newton1_IntFire2
 extern double I( _threadargsproto_ );
 extern double M( _threadargsproto_ );
 extern double firetime( _threadargsprotocomma_ double , double , double );
 extern double newton1( _threadargsprotocomma_ double , double , double , double , double );
 /* declare global and static user variables */
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "taus", "ms",
 "taum", "ms",
 0,0
};
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
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
"IntFire2",
 "taus",
 "taum",
 "ib",
 0,
 "i",
 "m",
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
 	_p = nrn_prop_data_alloc(_mechtype, 8, _prop);
 	/*initialize range parameters*/
 	taus = 20;
 	taum = 10;
 	ib = 0;
  }
 	_prop->param = _p;
 	_prop->param_size = 8;
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

 void _intfire2_reg_() {
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
  hoc_register_prop_size(_mechtype, 8, 3);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
  hoc_register_dparam_semantics(_mechtype, 2, "netsend");
 add_nrn_artcell(_mechtype, 2);
 add_nrn_has_net_event(_mechtype);
 pnt_receive[_mechtype] = _net_receive;
 pnt_receive_size[_mechtype] = 1;
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 IntFire2 /raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/intfire2.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
 
double M ( _threadargsproto_ ) {
   double _lM;
 double _lx , _lf ;
 _lf = taus / ( taus - taum ) ;
   _lx = ( i - ib ) * exp ( - ( t - t0 ) / taus ) ;
   _lM = ib + _lf * _lx + ( m - ( ib + ( i - ib ) * _lf ) ) * exp ( - ( t - t0 ) / taum ) ;
   
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
 
double I ( _threadargsproto_ ) {
   double _lI;
 double _lx , _lf ;
 _lf = taus / ( taus - taum ) ;
   _lx = ( i - ib ) * exp ( - ( t - t0 ) / taus ) ;
   _lI = _lx + ib ;
   
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
 
static void _net_receive (_pnt, _args, _lflag) Point_process* _pnt; double* _args; double _lflag; 
{  double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _thread = (Datum*)0; _nt = (_NrnThread*)_pnt->_vnt;   _p = _pnt->_prop->param; _ppvar = _pnt->_prop->dparam;
  if (_tsav > t){ extern char* hoc_object_name(); hoc_execerror(hoc_object_name(_pnt->ob), ":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay");}
 _tsav = t;   if (_lflag == 1. ) {*(_tqitem) = 0;}
 {
   double _lx , _linew , _lf , _libf ;
 _lf = taus / ( taus - taum ) ;
   _lx = ( i - ib ) * exp ( - ( t - t0 ) / taus ) ;
   _linew = ib + _lx ;
   if ( _lflag  == 1.0 ) {
     net_event ( _pnt, t ) ;
     m = 0.0 ;
     _libf = ( _linew - ib ) * _lf ;
     artcell_net_send ( _tqitem, _args, _pnt, t +  firetime ( _threadargscomma_ ib , _libf , 0.0 ) , 1.0 ) ;
     }
   else {
     m = ib + _lf * _lx + ( m - ( ib + ( i - ib ) * _lf ) ) * exp ( - ( t - t0 ) / taum ) ;
     _linew = _linew + _args[0] ;
     if ( m >= 1.0 ) {
       artcell_net_move ( _tqitem, _pnt, t ) ;
       }
     else {
       _libf = ( _linew - ib ) * _lf ;
       artcell_net_move ( _tqitem, _pnt, firetime ( _threadargscomma_ ib , _libf , m ) + t ) ;
       }
     }
   t0 = t ;
   i = _linew ;
   } }
 
double firetime ( _threadargsprotocomma_ double _la , double _lb , double _lc ) {
   double _lfiretime;
 double _lr , _lcab , _lx , _lm , _lf ;
 _lr = taum / taus ;
   if ( _la <= 1.0  && _la + _lb <= 1.0 ) {
     _lfiretime = 1e9 ;
     }
   else if ( _la > 1.0  && _lb <= 0.0 ) {
     _lcab = _lc - _la - _lb ;
     _lm = _lr * _lb + _lcab ;
     if ( _lm >= 0.0 ) {
       _lfiretime = - taus * log ( newton1 ( _threadargscomma_ _la , _lcab , _lb , 1.0 / _lr , 0.0 ) ) ;
       }
     else {
       _lx = ( 1.0 - _lc + _lm ) / _lm ;
       if ( _lx <= 0.0 ) {
         _lfiretime = - taus * log ( newton1 ( _threadargscomma_ _la , _lcab , _lb , 1.0 / _lr , 0.0 ) ) ;
         }
       else {
         _lfiretime = - taum * log ( newton1 ( _threadargscomma_ _la , _lb , _lcab , _lr , _lx ) ) ;
         }
       }
     }
   else {
     _lcab = _lc - _la - _lb ;
     _lx = pow( ( - _lcab / ( _lr * _lb ) ) , ( 1.0 / ( _lr - 1.0 ) ) ) ;
     _lf = _la + _lb * pow( _lx , _lr ) + _lcab * _lx ;
     if ( _lx >= 1.0  || _lf <= 1.0 ) {
       _lfiretime = 1e9 ;
       }
     else {
       _lm = _lr * _lb + _lcab ;
       _lx = ( 1.0 - _lc + _lm ) / _lm ;
       _lfiretime = - taum * log ( newton1 ( _threadargscomma_ _la , _lb , _lcab , _lr , _lx ) ) ;
       }
     }
   
return _lfiretime;
 }
 
static double _hoc_firetime(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  firetime ( _p, _ppvar, _thread, _nt, *getarg(1) , *getarg(2) , *getarg(3) );
 return(_r);
}
 
double newton1 ( _threadargsprotocomma_ double _la , double _lb , double _lc , double _lr , double _lx ) {
   double _lnewton1;
 double _ldx , _lf , _ldf , _liter , _lxm ;
 _ldx = 1.0 ;
   _lf = 0.0 ;
   _liter = 0.0 ;
   while ( fabs ( _ldx ) > 1e-6  || fabs ( _lf - 1.0 ) > 1e-6 ) {
     _lf = _la + _lb * pow( _lx , _lr ) + _lc * _lx ;
     _ldf = _lr * _lb * pow( _lx , ( _lr - 1.0 ) ) + _lc ;
     _ldx = ( 1.0 - _lf ) / _ldf ;
     _lx = _lx + _ldx ;
     if ( _lx <= 0.0 ) {
       _lx = 1e-4 ;
       }
     else if ( _lx > 1.0 ) {
       _lx = 1.0 ;
       }
     _liter = _liter + 1.0 ;
     if ( _liter  == 6.0  && _lr < 1.0 ) {
       _lxm = pow( ( - _lc / ( _lr * _lb ) ) , ( 1.0 / ( _lr - 1.0 ) ) ) ;
       }
     if ( _liter > 5.0 ) {
       }
     if ( _liter > 10.0 ) {
       printf ( "Intfire2 iter %g x=%g f=%g df=%g dx=%g a=%g b=%g c=%g r=%g\n" , _liter , _lx , _lf , _ldf , _ldx , _la , _lb , _lc , _lr ) ;
       _lf = 1.0 ;
       _ldx = 0.0 ;
       }
     }
   _lnewton1 = _lx ;
   
return _lnewton1;
 }
 
static double _hoc_newton1(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r =  newton1 ( _p, _ppvar, _thread, _nt, *getarg(1) , *getarg(2) , *getarg(3) , *getarg(4) , *getarg(5) );
 return(_r);
}

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
 {
   i = ib ;
   m = 0.0 ;
   if ( taus <= taum ) {
     taus = taum + .1 ;
     }
   t0 = t ;
   artcell_net_send ( _tqitem, (double*)0, _ppvar[1]._pvoid, t +  firetime ( _threadargscomma_ ib , 0.0 , 0.0 ) , 1.0 ) ;
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
static const char* nmodl_filename = "/raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/intfire2.mod";
static const char* nmodl_file_text = 
  ": total current is ib + weighted events that decay with time constant taus\n"
  ": total current is integrated and when passes 1, then fire\n"
  ": i.e. taus * di/dt + i = ib  where if event occurs at time t1 then\n"
  ":  i(t1) += w\n"
  ": and taum*dm/dt + m = i where fire if m crosses 1 in a positive direction\n"
  ": and after firing, m = 0.\n"
  ": Require, taum < taus, ie synaptic current longer than membrane time constant.\n"
  "\n"
  "NEURON {\n"
  "	ARTIFICIAL_CELL IntFire2\n"
  "	RANGE taus, taum, ib, i, m\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	taus = 20 (ms)\n"
  "	taum = 10 (ms)\n"
  "	ib = 0\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	i  m\n"
  "	t0 (ms)\n"
  "}\n"
  "\n"
  "FUNCTION M() {\n"
  "	LOCAL x, f	\n"
  "	f = taus/(taus - taum)\n"
  "	x = (i - ib)*exp(-(t - t0)/taus)\n"
  "	M = ib + f*x + (m - (ib + (i - ib)*f))*exp(-(t - t0)/taum)\n"
  "}\n"
  "\n"
  "FUNCTION I() {\n"
  "	LOCAL x, f\n"
  "	f = taus/(taus - taum)\n"
  "	x = (i - ib)*exp(-(t - t0)/taus)\n"
  "	I = x + ib\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	i = ib\n"
  "	m = 0\n"
  "	if (taus <= taum) {\n"
  "		taus = taum + .1\n"
  "	}\n"
  "	t0 = t\n"
  "	net_send(firetime(ib, 0, 0), 1)\n"
  "}\n"
  "\n"
  "NET_RECEIVE (w) {\n"
  "	LOCAL x, inew, f, ibf\n"
  "	f = taus/(taus - taum)\n"
  "	x = (i - ib)*exp(-(t - t0)/taus)\n"
  "	inew = ib + x\n"
  "	\n"
  "	if (flag == 1) { : time to fire\n"
  ":printf(\"now firing t=%g inew=%g\\n\", t, inew)\n"
  "		net_event(t)\n"
  "		m = 0\n"
  "		ibf = (inew - ib)*f\n"
  "		net_send(firetime(ib, ibf,  0), 1)\n"
  "	}else{\n"
  "		m = ib + f*x + (m - (ib + (i - ib)*f))*exp(-(t - t0)/taum)\n"
  ":printf(\"event w=%g i=%g m=%g\\n\", w, inew, m)\n"
  "		inew = inew + w\n"
  "		if (m >= 1) {\n"
  "			net_move(t) : the time to fire is now\n"
  "		}else{\n"
  "			ibf = (inew - ib)*f\n"
  "			net_move(firetime(ib, ibf, m) + t)\n"
  "		}\n"
  "	}\n"
  "	t0 = t\n"
  "	i = inew\n"
  "}\n"
  "\n"
  "FUNCTION firetime(a, b, c) (ms) {\n"
  "	: find 1 = a + b*exp(-t/taus) + (c - a - b)*exp(-t/taum)\n"
  "	: assert c < 1  taum < taus\n"
  "	LOCAL r, cab, x, m, f\n"
  "\n"
  "	r = taum/taus\n"
  "\n"
  "	: for numerical convenience when b is not too negative solve\n"
  "	: 1 = a + b*x^r + (c - a - b)*x where 0<x<1\n"
  "	: starting at x = 1\n"
  "	: if the slope at 1 points out of the domain ( b is too negative)\n"
  "	: then switch to solving 1 = a + b*x + (c - a - b)*x^(1/r)\n"
  "\n"
  "	if (a <= 1 && a + b <= 1) {\n"
  "		firetime = 1e9\n"
  "	}else if (a > 1 && b <= 0) {\n"
  "		: must be one solution in the domain since a > 1\n"
  "		: find starting point\n"
  "		cab = c - a - b\n"
  "		m = r*b + cab : slope at x=1\n"
  "		if (m >= 0) {\n"
  "			: switch to alternative\n"
  "			firetime = -taus * log(newton1(a,cab,b,1/r,0))\n"
  "		}else{\n"
  "			:intercept at f=1  note: value at x=1 is c\n"
  "			x = (1 - c + m)/m\n"
  "			if (x <= 0) {\n"
  "				:switch to alternative\n"
  "				firetime = -taus*log(newton1(a,cab,b,1/r,0))\n"
  "			}else{\n"
  "				: newton starting at x\n"
  "				firetime = -taum*log(newton1(a,b,cab,r,x))\n"
  "			}\n"
  "		}\n"
  "	}else{\n"
  "		: if a solution exists then newtons method starting from\n"
  "		: x=1 will converge to the proper one.\n"
  "		: a solution does not exist if f(tm) <= 1\n"
  "		cab = c - a - b\n"
  "		: max at\n"
  "		x = (-cab/(r*b))^(1/(r-1))\n"
  "		f = a + b*x^r + cab*x\n"
  ":printf(\"max at x=%g val=%g\\n\", x, f)\n"
  "		: believe it or not, there are cases where the max is\n"
  "		: located at x > 1. e.g. a=0, b=1.1, c=.9, r=.5\n"
  "		: so both parts of the test are essential\n"
  "		if (x >= 1 || f <= 1) {\n"
  "			firetime = 1e9\n"
  "		}else{\n"
  "			m = r*b + cab\n"
  "			x = (1 - c + m)/m\n"
  "			firetime = -taum*log(newton1(a,b,cab,r,x))\n"
  "		}\n"
  "	}\n"
  ":printf(\"firetime=%g\\n\", firetime)\n"
  "}\n"
  "\n"
  "FUNCTION newton1(a,b,c,r,x) {\n"
  "	: solve 1 = a + b*x^r + c*x starting from x\n"
  "	: derivative is r*b*x^(r-1) + c\n"
  "	: must stay in domain 0 < x <= 1\n"
  "	LOCAL dx, f, df, iter, xm\n"
  "	dx = 1\n"
  "	f = 0\n"
  "	iter = 0\n"
  ":printf(\"newton a=%g b=%g c=%g r=%g x=%g\\n\", a, b, c, r, x)\n"
  "	while( fabs(dx) > 1e-6 || fabs(f - 1) > 1e-6) {\n"
  "		f = a + b*x^r + c*x\n"
  "		df = r*b*x^(r-1) + c\n"
  "		dx = (1 - f)/df\n"
  ":printf(\"newton x=%g f=%g df=%g dx=%g\\n\", x, f, df, dx)\n"
  "		x = x + dx\n"
  "		if (x <= 0) {\n"
  "			x = 1e-4\n"
  "		}else if (x > 1) {\n"
  "			x = 1\n"
  "		}\n"
  "		iter = iter + 1\n"
  "		if (iter == 6 && r < 1) {\n"
  "			xm = (-c/(r*b))^(1/(r-1))\n"
  ":printf(\"max at x=%g val=%g\\n\", xm, (a + b*xm^r + c*xm))\n"
  "		}\n"
  "		if (iter >5) {\n"
  ":printf(\"Intfire2 iter %g x=%g f=%g df=%g dx=%g a=%g b=%g c=%g r=%g\\n\",\n"
  ":iter, x, f, df, dx, a, b, c, r)\n"
  "		}\n"
  "		if (iter > 10) {\n"
  "printf(\"Intfire2 iter %g x=%g f=%g df=%g dx=%g a=%g b=%g c=%g r=%g\\n\",\n"
  "iter, x, f, df, dx, a, b, c, r)\n"
  "			f=1 dx=0\n"
  "		}\n"
  "	}\n"
  "	newton1 = x\n"
  "}\n"
  ;
#endif

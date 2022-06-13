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
 
#define nrn_init _nrn_init__SEClamp
#define _nrn_initial _nrn_initial__SEClamp
#define nrn_cur _nrn_cur__SEClamp
#define _nrn_current _nrn_current__SEClamp
#define nrn_jacob _nrn_jacob__SEClamp
#define nrn_state _nrn_state__SEClamp
#define _net_receive _net_receive__SEClamp 
#define icur icur__SEClamp 
#define vstim vstim__SEClamp 
 
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
#define rs _p[0]
#define dur1 _p[1]
#define amp1 _p[2]
#define dur2 _p[3]
#define amp2 _p[4]
#define dur3 _p[5]
#define amp3 _p[6]
#define i _p[7]
#define vc _p[8]
#define tc2 _p[9]
#define tc3 _p[10]
#define on _p[11]
#define v _p[12]
#define _g _p[13]
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
 static double _hoc_icur();
 static double _hoc_vstim();
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
 "icur", _hoc_icur,
 "vstim", _hoc_vstim,
 0, 0
};
 /* declare global and static user variables */
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "dur3", 0, 1e+09,
 "dur2", 0, 1e+09,
 "rs", 1e-09, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "rs", "megohm",
 "dur1", "ms",
 "amp1", "mV",
 "dur2", "ms",
 "amp2", "mV",
 "dur3", "ms",
 "amp3", "mV",
 "i", "nA",
 "vc", "mV",
 0,0
};
 static double delta_t = 1;
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
 static void nrn_cur(_NrnThread*, _Memb_list*, int);
static void  nrn_jacob(_NrnThread*, _Memb_list*, int);
 static void _hoc_destroy_pnt(_vptr) void* _vptr; {
   destroy_point_process(_vptr);
}
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"SEClamp",
 "rs",
 "dur1",
 "amp1",
 "dur2",
 "amp2",
 "dur3",
 "amp3",
 0,
 "i",
 "vc",
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
 	_p = nrn_prop_data_alloc(_mechtype, 14, _prop);
 	/*initialize range parameters*/
 	rs = 1;
 	dur1 = 0;
 	amp1 = 0;
 	dur2 = 0;
 	amp2 = 0;
 	dur3 = 0;
 	amp3 = 0;
  }
 	_prop->param = _p;
 	_prop->param_size = 14;
  if (!nrn_point_prop_) {
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 2, _prop);
  }
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 
}
 static void _initlists();
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, _NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _svclmp_reg_() {
	int _vectorized = 1;
  _initlists();
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init,
	 hoc_nrnpointerindex, 1,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 14, 2);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 SEClamp /raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/svclmp.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "svclmp.mod";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int icur(_threadargsproto_);
static int vstim(_threadargsproto_);
 
static int  icur ( _threadargsproto_ ) {
   if ( on ) {
     i = ( vc - v ) / rs ;
     }
   else {
     i = 0.0 ;
     }
    return 0; }
 
static double _hoc_icur(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 icur ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
static int  vstim ( _threadargsproto_ ) {
   on = 1.0 ;
   if ( dur1 ) {
     at_time ( _nt, dur1 ) ;
     }
   if ( dur2 ) {
     at_time ( _nt, tc2 ) ;
     }
   if ( dur3 ) {
     at_time ( _nt, tc3 ) ;
     }
   if ( t < dur1 ) {
     vc = amp1 ;
     }
   else if ( t < tc2 ) {
     vc = amp2 ;
     }
   else if ( t < tc3 ) {
     vc = amp3 ;
     }
   else {
     vc = 0.0 ;
     on = 0.0 ;
     }
   icur ( _threadargs_ ) ;
    return 0; }
 
static double _hoc_vstim(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; _NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (_NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 vstim ( _p, _ppvar, _thread, _nt );
 return(_r);
}

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt) {
  int _i; double _save;{
 {
   tc2 = dur1 + dur2 ;
   tc3 = tc2 + dur3 ;
   on = 0.0 ;
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
#if EXTRACELLULAR
 _nd = _ml->_nodelist[_iml];
 if (_nd->_extnode) {
    _v = NODEV(_nd) +_nd->_extnode->_v[0];
 }else
#endif
 {
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 }
 v = _v;
 initmodel(_p, _ppvar, _thread, _nt);
}
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, _NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   vstim ( _threadargs_ ) ;
   }
 _current += i;

} return _current;
}

static void nrn_cur(_NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
#if EXTRACELLULAR
 _nd = _ml->_nodelist[_iml];
 if (_nd->_extnode) {
    _v = NODEV(_nd) +_nd->_extnode->_v[0];
 }else
#endif
 {
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 }
 _g = _nrn_current(_p, _ppvar, _thread, _nt, _v + .001);
 	{ _rhs = _nrn_current(_p, _ppvar, _thread, _nt, _v);
 	}
 _g = (_g - _rhs)/.001;
 _g *=  1.e2/(_nd_area);
 _rhs *= 1.e2/(_nd_area);
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) += _rhs;
  }else
#endif
  {
	NODERHS(_nd) += _rhs;
  }
  if (_nt->_nrn_fast_imem) { _nt->_nrn_fast_imem->_nrn_sav_rhs[_ni[_iml]] += _rhs; }
#if EXTRACELLULAR
 if (_nd->_extnode) {
   *_nd->_extnode->_rhs[0] += _rhs;
 }
#endif
 
}
 
}

static void nrn_jacob(_NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml];
 _nd = _ml->_nodelist[_iml];
#if CACHEVEC
  if (use_cachevec) {
	VEC_D(_ni[_iml]) -= _g;
  }else
#endif
  {
	NODED(_nd) -= _g;
  }
  if (_nt->_nrn_fast_imem) { _nt->_nrn_fast_imem->_nrn_sav_d[_ni[_iml]] -= _g; }
#if EXTRACELLULAR
 if (_nd->_extnode) {
   *_nd->_extnode->_d[0] += _g;
 }
#endif
 
}
 
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
#if EXTRACELLULAR
 _nd = _ml->_nodelist[_iml];
 if (_nd->_extnode) {
    _v = NODEV(_nd) +_nd->_extnode->_v[0];
 }else
#endif
 {
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 }
 v=_v;
{
 {  { icur(_p, _ppvar, _thread, _nt); }
  }}}

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
static const char* nmodl_filename = "/raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/svclmp.mod";
static const char* nmodl_file_text = 
  "TITLE svclmp.mod\n"
  "COMMENT\n"
  "Single electrode Voltage clamp with three levels.\n"
  "Clamp is on at time 0, and off at time\n"
  "dur1+dur2+dur3. When clamp is off the injected current is 0.\n"
  "The clamp levels are amp1, amp2, amp3.\n"
  "i is the injected current, vc measures the control voltage)\n"
  "Do not insert several instances of this model at the same location in order to\n"
  "make level changes. That is equivalent to independent clamps and they will\n"
  "have incompatible internal state values.\n"
  "The electrical circuit for the clamp is exceedingly simple:\n"
  "vc ---'\\/\\/`--- cell\n"
  "        rs\n"
  "\n"
  "Note that since this is an electrode current model v refers to the\n"
  "internal potential which is equivalent to the membrane potential v when\n"
  "there is no extracellular membrane mechanism present but is v+vext when\n"
  "one is present.\n"
  "Also since i is an electrode current,\n"
  "positive values of i depolarize the cell. (Normally, positive membrane currents\n"
  "are outward and thus hyperpolarize the cell)\n"
  "ENDCOMMENT\n"
  "\n"
  "INDEPENDENT {t FROM 0 TO 1 WITH 1 (ms)}\n"
  "\n"
  "DEFINE NSTEP 3\n"
  "\n"
  "NEURON {\n"
  "	POINT_PROCESS SEClamp\n"
  "	ELECTRODE_CURRENT i\n"
  "	RANGE dur1, amp1, dur2, amp2, dur3, amp3, rs, vc, i\n"
  "}\n"
  "\n"
  "UNITS {\n"
  "	(nA) = (nanoamp)\n"
  "	(mV) = (millivolt)\n"
  "	(uS) = (microsiemens)\n"
  "}\n"
  "\n"
  "\n"
  "PARAMETER {\n"
  "	rs = 1 (megohm) <1e-9, 1e9>\n"
  "	dur1 (ms) 	  amp1 (mV)\n"
  "	dur2 (ms) <0,1e9> amp2 (mV)\n"
  "	dur3 (ms) <0,1e9> amp3 (mV)\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)	: automatically v + vext when extracellular is present\n"
  "	i (nA)\n"
  "	vc (mV)\n"
  "	tc2 (ms)\n"
  "	tc3 (ms)\n"
  "	on\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	tc2 = dur1 + dur2\n"
  "	tc3 = tc2 + dur3\n"
  "	on = 0\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE icur METHOD after_cvode\n"
  "	vstim()\n"
  "}\n"
  "\n"
  "PROCEDURE icur() {\n"
  "	if (on) {\n"
  "		i = (vc - v)/rs\n"
  "	}else{\n"
  "		i = 0\n"
  "	}\n"
  "}\n"
  "\n"
  "COMMENT\n"
  "The SOLVE of icur() in the BREAKPOINT block is necessary to compute\n"
  "i=(vc - v(t))/rs instead of i=(vc - v(t-dt))/rs\n"
  "This is important for time varying vc because the actual i used in\n"
  "the implicit method is equivalent to (vc - v(t)/rs due to the\n"
  "calculation of di/dv from the BREAKPOINT block.\n"
  "The reason this works is because the SOLVE statement in the BREAKPOINT block\n"
  "is executed after the membrane potential is advanced.\n"
  "\n"
  "It is a shame that vstim has to be called twice but putting the call\n"
  "in a SOLVE block would cause playing a Vector into vc to be off by one\n"
  "time step.\n"
  "ENDCOMMENT\n"
  "\n"
  "PROCEDURE vstim() {\n"
  "	on = 1\n"
  "	if (dur1) {at_time(dur1)}\n"
  "	if (dur2) {at_time(tc2)}\n"
  "	if (dur3) {at_time(tc3)}\n"
  "	if (t < dur1) {\n"
  "		vc = amp1\n"
  "	}else if (t < tc2) {\n"
  "		vc = amp2\n"
  "	}else if (t < tc3) {\n"
  "		vc = amp3\n"
  "	}else {\n"
  "		vc = 0\n"
  "		on = 0\n"
  "	}\n"
  "	icur()\n"
  "}\n"
  "\n"
  ;
#endif

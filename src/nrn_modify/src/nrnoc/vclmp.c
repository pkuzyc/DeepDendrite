/* Created by Language version: 7.7.0 */
/* NOT VECTORIZED */
#define NRN_VECTORIZED 0
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
 
#define nrn_init _nrn_init__VClamp
#define _nrn_initial _nrn_initial__VClamp
#define nrn_cur _nrn_cur__VClamp
#define _nrn_current _nrn_current__VClamp
#define nrn_jacob _nrn_jacob__VClamp
#define nrn_state _nrn_state__VClamp
#define _net_receive _net_receive__VClamp 
#define update update__VClamp 
#define vstim vstim__VClamp 
 
#define _threadargscomma_ /**/
#define _threadargsprotocomma_ /**/
#define _threadargs_ /**/
#define _threadargsproto_ /**/
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 static double *_p; static Datum *_ppvar;
 
#define t nrn_threads->_t
#define dt nrn_threads->_dt
#define dur (_p + 0)
#define amp (_p + 3)
#define gain _p[6]
#define rstim _p[7]
#define tau1 _p[8]
#define tau2 _p[9]
#define e0 _p[10]
#define vo0 _p[11]
#define vi0 _p[12]
#define fac _p[13]
#define i _p[14]
#define e _p[15]
#define vo _p[16]
#define vi _p[17]
#define stim _p[18]
#define tc _p[19]
#define De _p[20]
#define Dvo _p[21]
#define Dvi _p[22]
#define _g _p[23]
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
 /* external NEURON variables */
 /* declaration of user functions */
 static double _hoc_icur();
 static double _hoc_update();
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
 _p = _prop->param; _ppvar = _prop->dparam;
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
 "update", _hoc_update,
 "vstim", _hoc_vstim,
 0, 0
};
#define icur icur_VClamp
 extern double icur( );
 /* declare global and static user variables */
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 "dur", 0, 1e+09,
 "fac", 1, 10,
 "gain", 0, 1e+09,
 "rstim", 1e-09, 1e+09,
 "tau2", 0, 1e+09,
 "tau1", 0, 1e+09,
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "dur", "ms",
 "amp", "mV",
 "rstim", "megohm",
 "tau1", "ms",
 "tau2", "ms",
 "e0", "mV",
 "vo0", "mV",
 "vi0", "mV",
 "fac", "1.0",
 "e", "mV",
 "vo", "mV",
 "vi", "mV",
 "i", "nA",
 0,0
};
 static double delta_t = 0.01;
 static double v = 0;
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
"VClamp",
 "dur[3]",
 "amp[3]",
 "gain",
 "rstim",
 "tau1",
 "tau2",
 "e0",
 "vo0",
 "vi0",
 "fac",
 0,
 "i",
 0,
 "e",
 "vo",
 "vi",
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
 	_p = nrn_prop_data_alloc(_mechtype, 24, _prop);
 	/*initialize range parameters*/
 	gain = 100000;
 	rstim = 1;
 	tau1 = 0.001;
 	tau2 = 0;
 	e0 = 0;
 	vo0 = 0;
 	vi0 = 0;
 	fac = 0;
  }
 	_prop->param = _p;
 	_prop->param_size = 24;
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

 void _vclmp_reg_() {
	int _vectorized = 0;
  _initlists();
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init,
	 hoc_nrnpointerindex, 0,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 24, 2);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 VClamp /raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/vclmp.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int update();
static int vstim();
 
#define _RHS1(arg) _coef1[arg][3]
 static int _slist1[3];static double **_coef1;
 static void clamp();
 
static int  vstim (  ) {
   tc = 0.0 ;
   {int  _lj ;for ( _lj = 0 ; _lj <= 3 - 1 ; _lj ++ ) {
     stim = amp [ _lj ] ;
     tc = tc + dur [ _lj ] ;
     if ( t < tc ) {
       tc = tc + 100.0 ;
       
/*VERBATIM*/
			break;
 }
     } }
    return 0; }
 
static double _hoc_vstim(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r = 1.;
 vstim (  );
 return(_r);
}
 
double icur (  ) {
   double _licur;
 double _lvout ;
 if ( t > tc ) {
     e0 = 0.0 ;
     vo0 = 0.0 ;
     _licur = 0.0 ;
     }
   else {
     error =  0; clamp();
 error = simeq(3, _coef1, _p, _slist1);
 if(error){fprintf(stderr,"at line 141 in file vclmp.mod:\n		SOLVE clamp\n"); nrn_complain(_p); abort_run(error);}
 _licur = ( vo - v ) / rstim ;
     }
   
return _licur;
 }
 
static double _hoc_icur(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r =  icur (  );
 return(_r);
}
 
static void clamp ()
 {
   zero_matrix(_coef1, 3, 4);
{
  int _counte = -1;
 double _lt1 , _lt2 ;
 _lt1 = tau1 / dt ;
   _lt2 = tau2 / dt ;
    ++_counte;
 _coef1[_counte][0] -=  1.0 ;
 _RHS1(_counte) -=  v ;
 _coef1[_counte][1] +=  fac * 1.0 ;
 _RHS1(_counte) +=  fac * v ;
   ;
  ++_counte;
 _coef1[_counte][1] -=  _lt2 * 1.0 ;
 _RHS1(_counte) -=  _lt2 * vo0 ;
 _coef1[_counte][1] -=  1.0 ;
 _coef1[_counte][2] -=  gain * 1.0 ;
   ;
  ++_counte;
 _RHS1(_counte) -=  stim ;
 _coef1[_counte][2] +=  1.0 ;
 _coef1[_counte][0] -=  1.0 ;
 _coef1[_counte][2] +=  1.0 ;
 _coef1[_counte][0] -=  _lt1 * 1.0 ;
 _coef1[_counte][2] +=  _lt1 * 1.0 ;
 _RHS1(_counte) -=  _lt1 * ( vi0 - e0 ) ;
 _RHS1(_counte) -=  0.0 ;
   ;
 
}
 }
 
static int  update (  ) {
   i = icur ( _threadargs_ ) ;
   e0 = e ;
   vo0 = vo ;
   vi0 = vi ;
   
/*VERBATIM*/
	return 0;
  return 0; }
 
static double _hoc_update(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r = 1.;
 update (  );
 return(_r);
}

static void initmodel() {
  int _i; double _save;_ninits++;
 _save = t;
 t = 0.0;
{
  e = e0;
  vi = vi0;
  vo = vo0;
 {
   e0 = 0.0 ;
   vo = v ;
   vo0 = v ;
   vi = v ;
   vi0 = v ;
   e = 0.0 ;
   {int  _lj ;for ( _lj = 0 ; _lj <= 3 - 1 ; _lj ++ ) {
     if ( dur [ _lj ] > 0.0  && amp [ _lj ]  != 0.0 ) {
       
/*VERBATIM*/
		{extern int cvode_active_;
		if (cvode_active_) {
			hoc_execerror("VClamp", " does not work with CVODE");
		}}
 }
     } }
   }
  _sav_indep = t; t = _save;

}
}

static void nrn_init(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
 initmodel();
}}

static double _nrn_current(double _v){double _current=0.;v=_v;{ {
   vstim ( _threadargs_ ) ;
   i = icur ( _threadargs_ ) ;
   }
 _current += i;

} return _current;
}

static void nrn_cur(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
 _g = _nrn_current(_v + .001);
 	{ _rhs = _nrn_current(_v);
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
 
}}

static void nrn_jacob(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
 
}}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
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
 { error =  update();
 if(error){fprintf(stderr,"at line 111 in file vclmp.mod:\n	SOLVE update METHOD after_cvode : but not really (see above)\n"); nrn_complain(_p); abort_run(error);}
 }}}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
 _slist1[0] = &(vi) - _p;
 _slist1[1] = &(vo) - _p;
 _slist1[2] = &(e) - _p;
 if (_first) _coef1 = makematrix(3, 4);
_first = 0;
}

#if NMODL_TEXT
static const char* nmodl_filename = "/raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/vclmp.mod";
static const char* nmodl_file_text = 
  "COMMENT\n"
  "Voltage clamp with three levels. Clamp is on at time 0, and off at time\n"
  "dur[0]+dur[1]+dur[2]. When clamp is off the injected current is 0.\n"
  "Do not insert several instances of this model at the same location in order to\n"
  "make level changes. That is equivalent to independent clamps and they will\n"
  "have incompatible internal state values.\n"
  "\n"
  "The control amplifier has the indicated gain and time constant.  The\n"
  "input amplifier is ideal.  \n"
  "\n"
  "		 tau2\n"
  "	         gain\n"
  "                +-|\\____rstim____>to cell\n"
  "-amp --'\\/`-------|/\n"
  "                |\n"
  "	        |----||---\n"
  "	        |___    __|-----/|___from cell\n"
  "		    `'`'        \\|\n"
  "		    tau1\n"
  "\n"
  "The clamp has a three states which are the voltage input of the gain amplifier,\n"
  "the voltage output of the gain amplfier, and the voltage output of the\n"
  "measuring amplifier.\n"
  "A good initial condition for these voltages are 0, 0, and v respectively.\n"
  "\n"
  "This model is quite stiff.  For this reason the current is updated\n"
  "within the solve block before updating the state of the clamp. This\n"
  "gives the correct value of the current on exit from fadvance(). If we\n"
  "didn't do this and\n"
  "instead used the values computed in the breakpoint block, it \n"
  "would look like the clamp current is much larger than it actually is since it\n"
  "doesn't take into account the change in voltage within the timestep, ie\n"
  "equivalent to an almost infinite capacitance.\n"
  "Also, because of stiffness, do not use this model except with secondorder=0.\n"
  "\n"
  "This model makes use of implementation details of how models are interfaced\n"
  "to neuron. At some point I will make the translation such that these kinds\n"
  "of models can be handled straightforwardly.\n"
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
  "DEFINE NSTEP 3\n"
  "\n"
  "NEURON {\n"
  "	POINT_PROCESS VClamp\n"
  "	ELECTRODE_CURRENT i\n"
  "	RANGE e0,vo0,vi0,dur,amp,gain,rstim,tau1,tau2,fac,i\n"
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
  "	dur[NSTEP] (ms)		<0, 1e9>\n"
  "	amp[NSTEP] (mV)\n"
  "	gain = 1e5		<0,1e9>\n"
  "	rstim = 1 (megohm)	<1e-9,1e9>\n"
  "	tau1 = .001 (ms)	<0,1e9>\n"
  "	tau2 = 0   (ms)		<0,1e9>\n"
  "	e0 (mV) vo0 (mV) vi0(mV)\n"
  "	fac=0			<1,10>\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	v (mV)	: automatically v + vext when extracellular is present\n"
  "	dt (ms)\n"
  "	i (nA)\n"
  "	stim (mV)\n"
  "	tc (ms)\n"
  "}\n"
  "\n"
  "\n"
  "STATE {\n"
  "	e (mV)\n"
  "	vo (mV)\n"
  "	vi (mV)\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	e0 = 0\n"
  "	vo = v\n"
  "	vo0 = v\n"
  "	vi = v\n"
  "	vi0 = v\n"
  "	e = 0\n"
  "	FROM j=0 TO NSTEP-1 { if (dur[j] > 0 && amp[j] != 0) {\n"
  "		: nrn/lib/hoc/electrod.hoc always installs a VClamp\n"
  "		: stopping cvode here if the clamp is on still allows\n"
  "		: that tool to control a IClamp under cvode\n"
  "		VERBATIM\n"
  "		{extern int cvode_active_;\n"
  "		if (cvode_active_) {\n"
  "			hoc_execerror(\"VClamp\", \" does not work with CVODE\");\n"
  "		}}\n"
  "		ENDVERBATIM\n"
  "	}}\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "	SOLVE update METHOD after_cvode : but not really (see above)\n"
  "	vstim()\n"
  "	i = icur()\n"
  "}\n"
  "\n"
  "PROCEDURE vstim() { : can't be called from update since vinput must\n"
  "			: remain constant throughout dt interval and\n"
  "			: update is called at t+dt\n"
  "	tc = 0 (ms)\n"
  "	FROM j=0 TO NSTEP-1 {\n"
  "		stim = amp[j]\n"
  "		tc = tc + dur[j]\n"
  "		if (t < tc) {\n"
  "			tc = tc + 100	: clamp is definitely not off\n"
  "			VERBATIM\n"
  "			break;\n"
  "			ENDVERBATIM\n"
  "		}\n"
  "	}\n"
  "}\n"
  "\n"
  "FUNCTION icur()(nA) {  : since this function uses range variables, it\n"
  "			: should not be called from hoc. If you want to\n"
  "			: know the current, look in i_vc\n"
  "	LOCAL vout\n"
  "	if (t > tc) {\n"
  "		e0 = 0\n"
  "		vo0 = 0\n"
  "		icur = 0\n"
  "	}else{\n"
  "		SOLVE clamp\n"
  "		icur = (vo - v)/rstim\n"
  "	}\n"
  "}\n"
  "\n"
  "LINEAR clamp {\n"
  "	LOCAL t1, t2\n"
  "	t1 = tau1/dt\n"
  "	t2 = tau2/dt\n"
  "	~ vi = v + fac*vo - fac*v\n"
  "	~ t2*vo - t2*vo0 + vo = -gain * e\n"
  "	~ -stim - e  +  vi - e  +  t1*vi - t1*e - t1*(vi0 - e0)\n"
  "	  = 0\n"
  "}\n"
  "\n"
  "PROCEDURE update() {\n"
  "	i = icur()\n"
  "	e0 = e\n"
  "	vo0 = vo\n"
  "	vi0 = vi\n"
  "	VERBATIM\n"
  "	return 0;\n"
  "	ENDVERBATIM\n"
  "}\n"
  "\n"
  "COMMENT\n"
  "This implementation is not very high level since the clamp uses a state which\n"
  "must be computed at the same time as the membrane potential and doesn't fit\n"
  "into the paradigm normally used for channel states.\n"
  "The state, vinput0, at t is integrated from its old value saved in vinput.\n"
  "The value of vinput (as well as the initial values of v and the output\n"
  "of the control amplifier) is updated when the SOLVE\n"
  "is executed.  Notice that the icur function is very stiff with respect to\n"
  "v. For this reason i is only good for the user after the SOLVE is\n"
  "executed.\n"
  "ENDCOMMENT\n"
  ;
#endif

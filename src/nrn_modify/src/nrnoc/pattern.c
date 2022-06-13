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
 
#define nrn_init _nrn_init__PatternStim
#define _nrn_initial _nrn_initial__PatternStim
#define nrn_cur _nrn_cur__PatternStim
#define _nrn_current _nrn_current__PatternStim
#define nrn_jacob _nrn_jacob__PatternStim
#define nrn_state _nrn_state__PatternStim
#define _net_receive _net_receive__PatternStim 
#define play play__PatternStim 
 
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
#define fake_output _p[0]
#define _tsav _p[1]
#define _nd_area  *_ppvar[0]._pval
#define ptr	*_ppvar[2]._pval
#define _p_ptr	_ppvar[2]._pval
 
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
 static int hoc_nrnpointerindex =  2;
 /* external NEURON variables */
 /* declaration of user functions */
 static double _hoc_initps();
 static double _hoc_play();
 static double _hoc_sendgroup();
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
 "initps", _hoc_initps,
 "play", _hoc_play,
 "sendgroup", _hoc_sendgroup,
 0, 0
};
#define initps initps_PatternStim
#define sendgroup sendgroup_PatternStim
 extern double initps( );
 extern double sendgroup( );
 /* declare global and static user variables */
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 0,0
};
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
 static void _hoc_destroy_pnt(_vptr) void* _vptr; {
   destroy_point_process(_vptr);
}
 static void _destructor(Prop*);
 static void _constructor(Prop*);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"PatternStim",
 "fake_output",
 0,
 0,
 0,
 "ptr",
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
 	_p = nrn_prop_data_alloc(_mechtype, 2, _prop);
 	/*initialize range parameters*/
 	fake_output = 0;
  }
 	_prop->param = _p;
 	_prop->param_size = 2;
  if (!nrn_point_prop_) {
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 4, _prop);
  }
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 if (!nrn_point_prop_) {_constructor(_prop);}
 
}
 static void _initlists();
 
#define _tqitem &(_ppvar[3]._pvoid)
 static void _net_receive(Point_process*, double*, double);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, _NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _pattern_reg_() {
	int _vectorized = 0;
  _initlists();
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,(void*)0, (void*)0, (void*)0, nrn_init,
	 hoc_nrnpointerindex, 0,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
 	register_destructor(_destructor);
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 2, 4);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
  hoc_register_dparam_semantics(_mechtype, 2, "pointer");
  hoc_register_dparam_semantics(_mechtype, 3, "netsend");
 add_nrn_artcell(_mechtype, 3);
 pnt_receive[_mechtype] = _net_receive;
 pnt_receive_size[_mechtype] = 1;
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 PatternStim /raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/pattern.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int play();
 
static void _net_receive (_pnt, _args, _lflag) Point_process* _pnt; double* _args; double _lflag; 
{    _p = _pnt->_prop->param; _ppvar = _pnt->_prop->dparam;
  if (_tsav > t){ extern char* hoc_object_name(); hoc_execerror(hoc_object_name(_pnt->ob), ":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay");}
 _tsav = t;   if (_lflag == 1. ) {*(_tqitem) = 0;}
 {
   double _lnst ;
 if ( _lflag  == 1.0 ) {
     _lnst = sendgroup ( _threadargs_ ) ;
     if ( _lnst >= t ) {
       artcell_net_send ( _tqitem, _args, _pnt, t +  _lnst - t , 1.0 ) ;
       }
     }
   } }
 
/*VERBATIM*/

extern int ifarg(int iarg);
extern double* vector_vec(void* vv);
extern int vector_capacity(void* vv);
extern void* vector_arg(int iarg);
extern void nrn_fake_fire(int gid, double spiketime, int fake_out);

typedef struct {
	void* tvec;
	void* gidvec;
	int index;
} Info;

#define INFOCAST Info** ip = (Info**)(&(_p_ptr))

 
double initps (  ) {
   double _linitps;
 
/*VERBATIM*/
{
	INFOCAST; Info* info = *ip;
	info->index = 0;
	if (info->tvec) {
		_linitps = 1.;
	}else{
		_linitps = 0.;
	}
}
 
return _linitps;
 }
 
static double _hoc_initps(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r =  initps (  );
 return(_r);
}
 
double sendgroup (  ) {
   double _lsendgroup;
 
/*VERBATIM*/
{
	INFOCAST; Info* info = *ip;
	int size = vector_capacity(info->tvec);
	int fake_out;
	double* tvec = vector_vec(info->tvec);
	double* gidvec = vector_vec(info->gidvec);
	int i;
	fake_out = fake_output ? 1 : 0;
	for (i=0; info->index < size; ++i) {
		/* only if the gid is NOT on this machine */
		nrn_fake_fire((int)gidvec[info->index], tvec[info->index], fake_out);
		++info->index;
		if (i > 100 && t < tvec[info->index]) { break; }
	}
	if (info->index >= size) {
		_lsendgroup = t - 1.;
	}else{
		_lsendgroup = tvec[info->index];
	}
}
 
return _lsendgroup;
 }
 
static double _hoc_sendgroup(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r =  sendgroup (  );
 return(_r);
}
 
static int  play (  ) {
   
/*VERBATIM*/
{
	INFOCAST; Info* info = *ip;
	if (ifarg(1)) {
		info->tvec = vector_arg(1);
		info->gidvec = vector_arg(2);
	}else{
		info->tvec = (void*)0;
		info->gidvec = (void*)0;
	}
}
  return 0; }
 
static double _hoc_play(void* _vptr) {
 double _r;
    _hoc_setdata(_vptr);
 _r = 1.;
 play (  );
 return(_r);
}
 
static void _constructor(Prop* _prop) {
	_p = _prop->param; _ppvar = _prop->dparam;
{
 {
   
/*VERBATIM*/
{
	INFOCAST;
	Info* info = (Info*)hoc_Emalloc(sizeof(Info)); hoc_malchk();
	*ip = info;
	info->tvec = (void*)0;
	info->gidvec = (void*)0;
	info->index = 0;
}
 }
 
}
}
 
static void _destructor(Prop* _prop) {
	_p = _prop->param; _ppvar = _prop->dparam;
{
 {
   
/*VERBATIM*/
{
	INFOCAST; Info* info = *ip;
	free(info);
}
 }
 
}
}

static void initmodel() {
  int _i; double _save;_ninits++;
{
 {
   if ( initps ( _threadargs_ ) > 0.0 ) {
     artcell_net_send ( _tqitem, (double*)0, _ppvar[1]._pvoid, t +  0.0 , 1.0 ) ;
     }
   }

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
 _tsav = -1e20;
 initmodel();
}}

static double _nrn_current(double _v){double _current=0.;v=_v;{
} return _current;
}

static void nrn_state(_NrnThread* _nt, _Memb_list* _ml, int _type){
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _nd = _ml->_nodelist[_iml];
 v=_v;
{
}}

}

static void terminal(){}

static void _initlists() {
 int _i; static int _first = 1;
  if (!_first) return;
_first = 0;
}

#if NMODL_TEXT
static const char* nmodl_filename = "/raid/zyc/code_for_dhs_paper/src/nrn_modify/src/nrnoc/pattern.mod";
static const char* nmodl_file_text = 
  ": The spikeout pairs (t, gid) resulting from a parallel network simulation\n"
  ": can become the stimulus for any single cpu subnet as long as the gid's are\n"
  ": consistent.\n"
  ": Note: hoc must retain references to the tvec and gidvec vectors\n"
  ": to prevent the Info from going out of existence\n"
  "\n"
  "NEURON {\n"
  "	ARTIFICIAL_CELL PatternStim\n"
  "	RANGE fake_output\n"
  "	POINTER ptr\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "	fake_output = 0\n"
  "}\n"
  "\n"
  "ASSIGNED {\n"
  "	ptr\n"
  "}\n"
  "\n"
  "INITIAL {\n"
  "	if (initps() > 0) { net_send(0, 1) }\n"
  "}\n"
  "\n"
  "NET_RECEIVE (w) {LOCAL nst\n"
  "	if (flag == 1) {\n"
  "		nst = sendgroup()\n"
  "		if (nst >= t) {net_send(nst - t, 1)}\n"
  "	}\n"
  "}\n"
  "\n"
  "VERBATIM\n"
  "\n"
  "extern int ifarg(int iarg);\n"
  "extern double* vector_vec(void* vv);\n"
  "extern int vector_capacity(void* vv);\n"
  "extern void* vector_arg(int iarg);\n"
  "extern void nrn_fake_fire(int gid, double spiketime, int fake_out);\n"
  "\n"
  "typedef struct {\n"
  "	void* tvec;\n"
  "	void* gidvec;\n"
  "	int index;\n"
  "} Info;\n"
  "\n"
  "#define INFOCAST Info** ip = (Info**)(&(_p_ptr))\n"
  "\n"
  "ENDVERBATIM\n"
  "\n"
  "\n"
  "CONSTRUCTOR {\n"
  "VERBATIM {\n"
  "	INFOCAST;\n"
  "	Info* info = (Info*)hoc_Emalloc(sizeof(Info)); hoc_malchk();\n"
  "	*ip = info;\n"
  "	info->tvec = (void*)0;\n"
  "	info->gidvec = (void*)0;\n"
  "	info->index = 0;\n"
  "}\n"
  "ENDVERBATIM\n"
  "}\n"
  "\n"
  "DESTRUCTOR {\n"
  "VERBATIM {\n"
  "	INFOCAST; Info* info = *ip;\n"
  "	free(info);\n"
  "}\n"
  "ENDVERBATIM\n"
  "}\n"
  "\n"
  "FUNCTION initps() {\n"
  "VERBATIM {\n"
  "	INFOCAST; Info* info = *ip;\n"
  "	info->index = 0;\n"
  "	if (info->tvec) {\n"
  "		_linitps = 1.;\n"
  "	}else{\n"
  "		_linitps = 0.;\n"
  "	}\n"
  "}\n"
  "ENDVERBATIM\n"
  "}\n"
  "\n"
  "FUNCTION sendgroup() {\n"
  "VERBATIM {\n"
  "	INFOCAST; Info* info = *ip;\n"
  "	int size = vector_capacity(info->tvec);\n"
  "	int fake_out;\n"
  "	double* tvec = vector_vec(info->tvec);\n"
  "	double* gidvec = vector_vec(info->gidvec);\n"
  "	int i;\n"
  "	fake_out = fake_output ? 1 : 0;\n"
  "	for (i=0; info->index < size; ++i) {\n"
  "		/* only if the gid is NOT on this machine */\n"
  "		nrn_fake_fire((int)gidvec[info->index], tvec[info->index], fake_out);\n"
  "		++info->index;\n"
  "		if (i > 100 && t < tvec[info->index]) { break; }\n"
  "	}\n"
  "	if (info->index >= size) {\n"
  "		_lsendgroup = t - 1.;\n"
  "	}else{\n"
  "		_lsendgroup = tvec[info->index];\n"
  "	}\n"
  "}\n"
  "ENDVERBATIM\n"
  "}\n"
  "\n"
  "PROCEDURE play() {\n"
  "VERBATIM {\n"
  "	INFOCAST; Info* info = *ip;\n"
  "	if (ifarg(1)) {\n"
  "		info->tvec = vector_arg(1);\n"
  "		info->gidvec = vector_arg(2);\n"
  "	}else{\n"
  "		info->tvec = (void*)0;\n"
  "		info->gidvec = (void*)0;\n"
  "	}\n"
  "}\n"
  "ENDVERBATIM\n"
  "}\n"
  "        \n"
  "\n"
  ;
#endif

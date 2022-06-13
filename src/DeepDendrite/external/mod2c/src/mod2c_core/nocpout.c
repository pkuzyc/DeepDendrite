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
#include "nmodlconf.h"
/* /local/src/master/nrn/src/nmodl/nocpout.c,v 4.1 1997/08/30 20:45:28 hines Exp */

/*
nrnversion is a string that is passed via the _mechanism structure
as the first arg. It will be interpreted within neuron to determine if
that version is compatible with this version.
For now try to use something of the form d.d
If this is changed then also change nrnoc/init.c
*/
char* nmodl_version_ = "6.2.0";

/* Point processes are now interfaced to nrnoc via objectvars.
Thus, p-array variables and functions accessible to hoc do not have
suffixes, and there is a constructor, destructor.
Also hoc interface functions always have a void* _vptr arg which is
always cast to (Point_process*) and the _p and _ppvar pointers set.
This makes the old setdata and create obsolete.
*/
/* The strategy is to use as much of parout and hparout method as possible.
The bulk of the variables is in a p-array. The variables that don't belong
in this p-array are indicated by a flag with sym->subtype & NRNNOTP.
All other variables have values in that single array but not all those values
are available from HOC.

Variables accessible to NEURON are variables that appear within
GLOBAL, SECTION, and RANGE statements.
SECTION variables are not currently implemented.

Variables that do not appear in the p-array are:
 1)externally declared variables such as celsius, t.
 2)parameters and assigned not declared in the NEURON{RANGE list}
   that are global with respect to sections.
 3) variables static to this model, ie. v
 4) read only variables like "diam"
States always are in the p-array.

USEION variables in the p-array have connections to other places and
depending on the context may get their value from somewhere else, or
add their value to somewhere else, or place their value somewhere else. 
The cases are:
	NONSPECIFIC and USEION WRITE  i... value added to proper ion current
		and total current.
	USEION READ  entry value assigned to local copy.
	USEION WRITE e.. ..o ..i exit value of local copy assigned to pointer..
		 It is an error for an ionic current or ionic variable
		to be a STATE. Use another variable as the state, make the
		ionic variable an ASSIGNED and just assign it at the
		proper place.
	  Alternatively, if they are STATE's then they should not be READ
	  since their value comes from the p-array itself.

POINTER variables are like USEION variables. Unfortunately, it is up to
the hoc user to make sure they point to the proper place with a connect
statement. At this time we only check for a null pointer. The pointers
are kept in the ppvar array.

each model creates a setdata_suffix(x) (or setdata_suffix(i)) function
which sets up _p and _ppvar for use by functions in the model called
directly by hoc.
*/	

/* FUNCTIONS are made external so they are callable from other models */
#define GLOBFUNCT 1

#include "modl.h"
#include "parse1.h"
#include <stdlib.h>
#include <unistd.h>
#define GETWD(buf) getcwd(buf, NRN_BUFSIZE)

#if VECTORIZE
int vectorize = 1;
/*
the idea is to put all variables into a vector of vectors so there
there is no static data. Every function has an implicit argument, underbar ix
which tells which set of data _p[ix][...] to use. There are going to have
to be limits on the kinds of scopmath functions called since many of them
need static data. We can have special versions of the most useful of these.
ie sparse.c.
Above is obsolete in detail , underbar ix is no longer used.
When vectorize = 1 then we believe the code is thread safe and most functions
pass around _p, _ppvar, _thread. When vectorize is 0 then we are definitely
not thread safe and _p and _ppvar are static.
*/

#endif
#define NRNEXTRN	01	/* t, dt, celsius, etc. */
#define NRNCURIN	02	/* input value used */
#define NRNCUROUT	04	/* added to output value */
#define NRNRANGE	010	
#define NRNPRANGEIN	020
#define NRNPRANGEOUT	040
#define NRNGLOBAL	0100	/* same for all sections, defined here */
#define NRNSTATIC	0200	/* v */
#define NRNNOTP		0400	/* doesn't belong in p array */
#define NRNIONFLAG	01000	/* temporary flag to allow READ and WRITE
				   without declaring twice */
#define NRNSECTION	02000
#define NRNPOINTER	04000
#define IONCONC		010000
#define NRNBBCOREPOINTER	020000

#define IONEREV 0	/* Parameter */
#define IONIN	1
#define IONOUT	2
#define IONCUR	3	/* assigned */
#define IONDCUR 4

extern int assert_threadsafe;
extern int brkpnt_exists;
static char* brkpnt_str_;
extern Symbol *indepsym;
extern Symbol *scop_indep;
extern List   *indeplist;
extern Symbol *stepsym;
extern char   *reprime();
extern List	*symlist[];
extern List* ldifuslist;
extern char* finname;
extern int check_tables_threads(List*);
List* acc_present_list;
List *syminorder;
List *plotlist;
List *defs_list;
List *units_def_for_acc;
int electrode_current = 0;
int thread_data_index = 0;
List *thread_cleanup_list;
List *thread_mem_init_list;
List *newtonspace_list;
List* toplocal_;
extern int protect_;
extern int protect_include_;
extern List *set_ion_variables(), *get_ion_variables();
extern char* items_as_string(Item*, Item*);

static int decode_limits();
static int decode_tolerance();

/* 1 means emit the relevant code since the model
   is a POINT_PROCESS with a NET_RECEIVE block and no net_send
   and so the NET_RECEIVE body can be executed on the GPU.
   _net_receive buffers the event for this type and thread and at the
   end of NetCvode::deliver_net_events, _net_buffer_receive is called
   so that the buffered events are
   delivered on the gpu all at once for this type and thread.
   In the c file, the define NET_RECEIVE_BUFFERING is used to conditionally
   compile the NET_RECEIVE block buffering code.
*/
int net_receive_buffering_;
static void emit_net_receive_buffering_code();
static void emit_nrn_watch_check_code();


/* NEURON block information */
List *currents;
List *useion;
List* conductance_;
List* breakpoint_local_current_;
static List *rangeparm;
static List *rangedep;
static List *rangestate;
static List *nrnpointers;
static List* uip; /* void _update_ion_pointer(Datum* _ppvar){...} text */
char suffix[50];
extern char* modprefix;
static char *rsuffix;	/* point process range and functions don't have suffix*/
static char *mechname;
static char* modbase;
int point_process; /* 1 if a point process model */
int artificial_cell; /* 1 if also explicitly declared an ARTIFICIAL_CELL */
static int diamdec = 0;	/*1 if diam is declared*/
static int areadec = 0;
static int use_bbcorepointer = 0;
static int use_celsius = 0; /* celsius is used */
static void defs_h();
static int iontype();
static void nrndeclare();
static void del_range();
static void declare_p();
static int iondef();
static void ion_promote();
static int ppvar_cnt;
static List* ppvar_semantics_;
static void ppvar_semantics(int, const char*);
static int for_netcons_; /* number of FOR_NETCONS statements */
static Item* net_init_q1_;
static Item* net_init_q2_;
static int ba_index_; /* BEFORE AFTER blocks. See bablk */
static List* ba_list_;
static List* acc_globals_update_list;

#if CVODE
List* state_discon_list_;
static int cvode_emit, cvode_not_allowed, cvode_ieq_index;
static int cond_index;
static int tqitem_index;
static int watch_index;
static int cvode_index;
static List* ion_synonym;
extern int singlechan_;
int debugging_;
int net_receive_;
int net_send_seen_;
int net_event_seen_;
int watch_seen_; /* number of WATCH statements + 1*/
List* watch_data_; /* triples of par1, par2, flag */
static Item* net_send_delivered_; /* location for if flag is 1 then clear the
				tqitem_ to allow  an error message for net_move */
static Item* net_receive_block_begin_; /* the "void _net_receive" line */
static Item* net_receive_block_open_brace_; /* the "{" line */
#endif

#define SYMITER(arg) ITERATE(q, syminorder){ \
		s = SYM(q); if (s->type == arg)

#define SYMLISTITER for (i = 'A'; i <= 'z'; i++)\
	ITERATE(q, symlist[i])
	
#define IFTYPE(arg)	if ((s->subtype & arg)\
			 && ( (s->usage & EXPLICIT_DECL) != automatic) )

/* varcount holds the index into the .var file and is saved in s->used
   parraycount holds the index into the p array and is saved in s->varnum
   pvarcount indexes pointers to variables such as ena
*/
static int varcount, parraycount;

void set_suffix() {
	for (modbase = modprefix + strlen(modprefix); modbase != modprefix;
	    modbase--) {
		if (*modbase == '\\' || *modbase == '/') {
			modbase++;
			break;
		}
	}
	if (!mechname) {
		sprintf(suffix,"_%s", modbase);
		mechname = modbase;
	} else if (strcmp(mechname, "nothing") == 0) {
		vectorize = 0;
		suffix[0] = '\0';
		mechname = modbase;
	}else{
		sprintf(suffix, "_%s", mechname);
	}
}

void nrninit() {
	extern int using_default_indep;
	currents = newlist();
	rangeparm = newlist();
	rangedep = newlist();
	rangestate = newlist();
	useion = newlist();
	nrnpointers = newlist();
	using_default_indep = 0;
	indepinstall(install("t", NAME), "0", "1", "100", (Item*)0, "ms", 0);
	using_default_indep = 1;
	debugging_ = 1;
	thread_cleanup_list = newlist();
	thread_mem_init_list = newlist();
	newtonspace_list = newlist();
}

void parout() {
	int i, j, ioncount, pointercount, gind, emit_check_table_thread;
	Item *q, *q1, *extra_pragma_loop_arg;
	Symbol *s, *sion;
	double d1, d2;

#if BBCORE
	cvode_emit = 0;
#endif

	defs_list = newlist();	/* relates hoc names to c-variables */
	if (brkpnt_exists) {
	    if (vectorize) {
		brkpnt_str_ = "nrn_cur, NULL, nrn_state";
	    }else{
		brkpnt_str_ = "nrn_cur, nrn_jacob, nrn_state";
	    }
	}else{
		brkpnt_str_ = "0, 0, 0";
#if 1 || defined(__MINGW32__)
		/* x86_64-w64-mingw32-gcc passed 0 without zeroing the high 32 bits */
		/* also cygwin64 gcc 4.8.1, so cast to void* universally */
		brkpnt_str_ = "NULL, NULL, NULL";
#endif
	}

	set_suffix();	

	if (artificial_cell && vectorize && (thread_data_index || toplocal_)) {
fprintf(stderr, "Notice: ARTIFICIAL_CELL models that would require thread specific data are not thread safe.\n");	
		vectorize = 0;
	}
	if (point_process) {
		rsuffix = "";
	}else{
		rsuffix = suffix;
	}

	if (point_process && !artificial_cell && net_receive_) {
	  	net_receive_buffering_ = 1;
		emit_net_receive_buffering_code();
	}

	Lappendstr(defs_list, "\
\n#include \"coreneuron/utils/randoms/nrnran123.h\"\
\n#include \"coreneuron/nrnoc/md1redef.h\"\
\n#include \"coreneuron/nrnconf.h\"\
\n#include \"coreneuron/nrnoc/membfunc.h\"\
\n#include \"coreneuron/nrnoc/multicore.h\"\
\n#include \"coreneuron/nrniv/nrniv_decl.h\"\
\n#include \"coreneuron/nrniv/ivocvect.h\"\
\n#include \"coreneuron/nrniv/nrn_acc_manager.h\"\
\n#include \"coreneuron/mech/cfile/scoplib.h\"\n\
\n#include \"coreneuron/scopmath_core/newton_struct.h\"\
\n#include \"coreneuron/nrnoc/md2redef.h\"\
\n#include \"coreneuron/nrnoc/register_mech.hpp\"\
\n#include \"_kinderiv.h\"\
\n#if !NRNGPU\
\n#if !defined(DISABLE_HOC_EXP)\
\n#undef exp\
\n#define exp hoc_Exp\
\n#endif\
\n#endif\n\
");
	if (protect_include_) {
		Lappendstr(defs_list, "\n#include \"nmodlmutex.h\"");
	}
	if (use_bbcorepointer) {
        Lappendstr(defs_list, "#define _threadargsproto_namespace int _iml, int _cntml_padded, double* _p, coreneuron::Datum* _ppvar, coreneuron::ThreadDatum* _thread, coreneuron::NrnThread* _nt, double v\n");

		lappendstr(defs_list, "static void bbcore_read(double *, int*, int*, int*, _threadargsproto_namespace);\n");
		lappendstr(defs_list, "static void bbcore_write(double *, int*, int*, int*, _threadargsproto_namespace);\n");
	}

	Lappendstr(defs_list, "namespace coreneuron {\n");
	if (vectorize) {
        /* macros for compiler dependent ivdep like pragma and memory layout. INIT and STATE pragma are same
         * except that sync clause absent because we saw issue only in CaDynamics_E2 */
		extra_pragma_loop_arg = lappendstr(defs_list, "\
\n#if defined(_OPENACC) && !defined(DISABLE_OPENACC)\
\n#include <openacc.h>\
\n#if defined(PG_ACC_BUGS)\
\n#define _PRAGMA_FOR_INIT_ACC_LOOP_ _Pragma(\"acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], nrn_ion_global_map[0:nrn_ion_global_map_size][0:3], _nt[0:1] _thread_present_) if(_nt->compute_gpu)\")\
\n#else\
\n#define _PRAGMA_FOR_INIT_ACC_LOOP_ _Pragma(\"acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], nrn_ion_global_map[0:nrn_ion_global_map_size], _nt[0:1] _thread_present_) if(_nt->compute_gpu)\")\
\n#endif\
\n#define _PRAGMA_FOR_STATE_ACC_LOOP_ _Pragma(\"acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], _nt[0:1], _ml[0:1] _thread_present_) if(_nt->compute_gpu) async(stream_id)\")\
\n#define _PRAGMA_FOR_CUR_ACC_LOOP_ _Pragma(\"acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], _vec_d[0:_nt->end], _vec_rhs[0:_nt->end], _nt[0:1] _thread_present_) if(_nt->compute_gpu) async(stream_id)\")\
\n#define _PRAGMA_FOR_CUR_SYN_ACC_LOOP_ _Pragma(\"acc parallel loop present(_ni[0:_cntml_actual], _nt_data[0:_nt->_ndata], _p[0:_cntml_padded*_psize], _ppvar[0:_cntml_padded*_ppsize], _vec_v[0:_nt->end], _vec_shadow_rhs[0:_nt->shadow_rhs_cnt], _vec_shadow_d[0:_nt->shadow_rhs_cnt], _vec_d[0:_nt->end], _vec_rhs[0:_nt->end], _nt[0:1]) if(_nt->compute_gpu) async(stream_id)\")\
\n#define _PRAGMA_FOR_NETRECV_ACC_LOOP_ _Pragma(\"acc parallel loop present(_pnt[0:_pnt_length], _nrb[0:1], _nt[0:1], nrn_threads[0:nrn_nthread]) if(_nt->compute_gpu) async(stream_id)\")\
\n#else\
\n#define _PRAGMA_FOR_INIT_ACC_LOOP_ _Pragma(\"\")\
\n#define _PRAGMA_FOR_STATE_ACC_LOOP_ _Pragma(\"\")\
\n#define _PRAGMA_FOR_CUR_ACC_LOOP_ _Pragma(\"\")\
\n#define _PRAGMA_FOR_CUR_SYN_ACC_LOOP_ _Pragma(\"\")\
\n#define _PRAGMA_FOR_NETRECV_ACC_LOOP_ _Pragma(\"\")\
\n#endif\
\n \
\n#if defined(__clang__)\
\n#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma(\"clang loop vectorize(enable)\")\
\n#elif defined(__ICC) || defined(__INTEL_COMPILER)\
\n#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma(\"ivdep\")\
\n#elif defined(__IBMC__) || defined(__IBMCPP__)\
\n#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma(\"ibm independent_loop\")\
\n#elif defined(__PGI)\
\n#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma(\"vector\")\
\n#elif defined(_CRAYC)\
\n#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma(\"_CRI ivdep\")\
\n#elif defined(__GNUC__) || defined(__GNUG__)\
\n#define _PRAGMA_FOR_VECTOR_LOOP_ _Pragma(\"GCC ivdep\")\
\n#else\
\n#define _PRAGMA_FOR_VECTOR_LOOP_\
\n#endif // _PRAGMA_FOR_VECTOR_LOOP_\
\n \
\n#if !defined(LAYOUT)\
\n/* 1 means AoS, >1 means AoSoA, <= 0 means SOA */\
\n#define LAYOUT 1\
\n#endif\
\n#if LAYOUT >= 1\
\n#define _STRIDE LAYOUT\
\n#else\
\n#define _STRIDE _cntml_padded + _iml\
\n#endif\
\n");
	}else{
		Lappendstr(defs_list, "\
\n#undef LAYOUT\
\n#define LAYOUT 1\
\n#define _STRIDE 1\
\n");
	}

	if (net_receive_buffering_) {
		Lappendstr(defs_list, "\
\n\
\n#if !defined(NET_RECEIVE_BUFFERING)\
\n#define NET_RECEIVE_BUFFERING 1\
\n#endif\
\n");
	}

#if 1
	/* for easier profiling, give distinct names to otherwise reused static names */
	sprintf(buf, "\n\
#define nrn_init _nrn_init_%s\n\
#define nrn_cur _nrn_cur_%s\n\
#define _nrn_current _nrn_current_%s\n\
#define nrn_jacob _nrn_jacob_%s\n\
#define nrn_state _nrn_state_%s\n\
#define initmodel initmodel_%s\n\
#define _net_receive _net_receive_%s\n\
#define _net_init _net_init_%s\n\
#define nrn_state_launcher nrn_state%s_launcher\n\
#define nrn_cur_launcher nrn_cur%s_launcher\n\
#define nrn_jacob_launcher nrn_jacob%s_launcher\
", suffix, suffix, suffix, suffix, suffix, suffix, suffix, suffix, suffix, suffix, suffix);
	Lappendstr(defs_list, buf);

	if (net_receive_buffering_) {
		sprintf(buf, "\
\n#if NET_RECEIVE_BUFFERING\
\n#define _net_buf_receive _net_buf_receive%s\
\nvoid _net_buf_receive(NrnThread*);\
\n#endif\
\n", suffix);
		Lappendstr(defs_list, buf);
	}

	if (watch_seen_) {
		sprintf(buf,
"\n#define _nrn_watch_check _nrn_watch_check_%s", suffix);
		Lappendstr(defs_list, buf);
	}

	SYMLISTITER {
		Symbol* s = SYM(q);
		/* note that with GLOBFUNCT, FUNCT will be redefined anyway */
		if (s->type == NAME && s->subtype & (PROCED | DERF | KINF)) {
			sprintf(buf, "\n#define %s %s%s", s->name, s->name, suffix);
			Lappendstr(defs_list, buf);
		}
	}
	Lappendstr(defs_list, "\n");
#endif /* distinct names for easier profiling */

	Lappendstr(defs_list, "\n\
#undef _threadargscomma_\n\
#undef _threadargsprotocomma_\n\
#undef _threadargs_\n\
#undef _threadargsproto_\n\
");
	if (vectorize) {
		Lappendstr(defs_list, "\n\
#define _threadargscomma_ _iml, _cntml_padded, _p, _ppvar, _thread, _nt, v,\n\
#define _threadargsprotocomma_ int _iml, int _cntml_padded, double* _p, Datum* _ppvar, ThreadDatum* _thread, NrnThread* _nt, double v,\n\
#define _threadargs_ _iml, _cntml_padded, _p, _ppvar, _thread, _nt, v\n\
#define _threadargsproto_ int _iml, int _cntml_padded, double* _p, Datum* _ppvar, ThreadDatum* _thread, NrnThread* _nt, double v\n\
");
}else{
		Lappendstr(defs_list, "\n\
#define _threadargscomma_ /**/\n\
#define _threadargsprotocomma_ /**/\n\
#define _threadargs_ /**/\n\
#define _threadargsproto_ /**/\n\
");
}

	Lappendstr(defs_list, "\
	/*SUPPRESS 761*/\n\
	/*SUPPRESS 762*/\n\
	/*SUPPRESS 763*/\n\
	/*SUPPRESS 765*/\n\
	");
#if VECTORIZE
    if (vectorize) {
	Sprintf(buf, "/* Thread safe. No static _p or _ppvar. */\n");
    }else
#endif
    {
	Sprintf(buf, "static double *_p; static Datum *_ppvar;\n");
    }
	Lappendstr(defs_list, buf);

	nrndeclare();
	varcount = parraycount = 0;
	declare_p();
	ioncount = iondef(&pointercount); /* first is _nd_area if point process */
	Lappendstr(defs_list, "\n#if MAC\n#if !defined(v)\n#define v _mlhv\n#endif\n#if !defined(h)\n#define h _mlhh\n#endif\n#endif\n");
	Lappendstr(defs_list, "static int hoc_nrnpointerindex = ");
	if (pointercount) {
		q = nrnpointers->next;
		Sprintf(buf, "%d;\n", SYM(q)->used);
	}else{
		Sprintf(buf, "-1;\n");
	}
	Lappendstr(defs_list, buf);
	/*above modified to also count and define pointers*/
	
	if (vectorize) {
		Lappendstr(defs_list, "static ThreadDatum* _extcall_thread;\n");
	}
#if 0
	Lappendstr(defs_list, "/* static variables special to NEURON */\n");
	SYMLISTITER {
		if (SYM(q)->nrntype & NRNSTATIC) {
			Sprintf(buf, "static double %s;\n", SYM(q)->name);
			Lappendstr(defs_list, buf);
		}
	}
#endif
	ITERATE(q, units_def_for_acc) {
		Sprintf(buf, "#define %s %s%s\n", STR(q), STR(q), suffix);
		Lappendstr(defs_list, buf);
	}

	Lappendstr(defs_list, "/* external NEURON variables */\n");
	SYMLISTITER {
		s = SYM(q);
		if (s->nrntype & NRNEXTRN) {
			if (strcmp(s->name, "dt") == 0) { continue; }
			if (strcmp(s->name, "t") == 0) { continue; }
			if (s->subtype & ARRAY) {
				Sprintf(buf, "extern double* %s;\n", s->name);
			}else{
				Sprintf(buf, "extern double %s;\n", s->name);
			}
			Lappendstr(defs_list, buf);
			if (strcmp(s->name, "celsius") == 0) {
                use_celsius = 1;
                Sprintf(buf,
                "#define _celsius_ _celsius_%s\n"
                "double _celsius_;\n"
                "#pragma acc declare copyin(_celsius_)\n", suffix);
                Lappendstr(defs_list, buf);
            }
        }
    }
	
#if BBCORE
	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
	Lappendstr(defs_list, "/* declaration of user functions */\n");
	SYMLISTITER {
		s = SYM(q);
		if (s->subtype & (FUNCT | PROCED) && s->name[0] != '_') {
			if (point_process) {
Sprintf(buf, "static double _hoc_%s();\n", s->name);
			}else{
Sprintf(buf, "static void _hoc_%s(void);\n", s->name);
			}
			Lappendstr(defs_list, buf);
		}
	}
#if BBCORE
	Lappendstr(defs_list, "\n#endif /*BBCORE*/\n");
#endif

	q = lappendstr(defs_list, "static int _mechtype;\n");
	if (net_send_seen_ && !artificial_cell) {
		Sprintf(buf,
		  "\n#define _mechtype _mechtype%s\n"
		  "int _mechtype;\n"
		  "#pragma acc declare copyin (_mechtype)\n"
		  , suffix);
		replacstr(q, buf);
	}

	/**** create special point process functions */
	if (point_process) {
		Lappendstr(defs_list, "static int _pointtype;\n");
#if BBCORE
	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
		Lappendstr(defs_list, "static void* _hoc_create_pnt(_ho) Object* _ho; { void* create_point_process();\n");
		Lappendstr(defs_list, "return create_point_process(_pointtype, _ho);\n}\n");
		Lappendstr(defs_list, "static void _hoc_destroy_pnt();\n");
		Lappendstr(defs_list, "static double _hoc_loc_pnt(_vptr) void* _vptr; {double loc_point_process();\n");
		Lappendstr(defs_list, "return loc_point_process(_pointtype, _vptr);\n}\n");
		Lappendstr(defs_list, "static double _hoc_has_loc(_vptr) void* _vptr; {double has_loc_point();\n");
		Lappendstr(defs_list, "return has_loc_point(_vptr);\n}\n");
		Lappendstr(defs_list, "static double _hoc_get_loc_pnt(_vptr)void* _vptr; {\n");
		Lappendstr(defs_list, "double get_loc_point_process(); return (get_loc_point_process(_vptr));\n}\n");
#if BBCORE
	Lappendstr(defs_list, "\n#endif /*BBCORE*/\n");
#endif
	}
	
	/* functions */
#if BBCORE
	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
	Lappendstr(defs_list, "/* connect user functions to hoc names */\n");
	Lappendstr(defs_list,"static VoidFunc hoc_intfunc[] = {\n");
   if (point_process) {
	Lappendstr(defs_list,"0,0\n};\n");
	Lappendstr(defs_list, "static Member_func _member_func[] = {\n");
	Sprintf(buf, "\"loc\", _hoc_loc_pnt,\n");
	Lappendstr(defs_list, buf);
	Sprintf(buf, "\"has_loc\", _hoc_has_loc,\n");
	Lappendstr(defs_list, buf);
	Sprintf(buf, "\"get_loc\", _hoc_get_loc_pnt,\n");
	Lappendstr(defs_list, buf);
   }else{
	Sprintf(buf, "\"setdata_%s\", _hoc_setdata,\n", mechname);
	Lappendstr(defs_list, buf);
   }
 	SYMLISTITER {
		s = SYM(q);
		if ((s->subtype & (FUNCT | PROCED)) && s->name[0] != '_') {
Sprintf(buf, "\"%s%s\", _hoc_%s,\n", s->name, rsuffix, s->name);
			Lappendstr(defs_list, buf);
		}
	}
	Lappendstr(defs_list, "0, 0\n};\n");
#if BBCORE
	Lappendstr(defs_list, "\n#endif /*BBCORE*/\n");
#endif

#if GLOBFUNCT
	/* FUNCTION's are now global so callable from other models */
	/* change name to namesuffix. This propagates everywhere except
		to hoc_name*/
	/* but don't do it if suffix is empty */
	if (suffix[0]) SYMLISTITER {
		s = SYM(q);
		if ((s->subtype & FUNCT)) {
			Sprintf(buf, "#define %s %s%s\n", s->name, s->name, suffix);
			q1 = Lappendstr(defs_list, buf);
			q1->itemtype = VERBATIM;
		}
	}
	SYMLISTITER {
		int j;
		s = SYM(q);
		if ((s->subtype & FUNCT)) {
            if(!artificial_cell) {
                Lappendstr(defs_list, "#pragma acc routine seq\n");
            }
			Sprintf(buf, "inline double %s(", s->name);
			Lappendstr(defs_list, buf);
			if (vectorize && !s->no_threadargs) {
				if (s->varnum) {
					Lappendstr(defs_list, "_threadargsprotocomma_");
				}else{
					Lappendstr(defs_list, "_threadargsproto_");
				}
			}
			for (j=0; j < s->varnum; ++j) {
				Lappendstr(defs_list, "double");
				if (j+1 < s->varnum) {
					Lappendstr(defs_list, ",");
				}
			}
			Lappendstr(defs_list, ");\n");
		}
	}
#endif

	emit_check_table_thread = 0;
	if (vectorize && check_tables_threads(defs_list)) {
		emit_check_table_thread = 1;
	}

	/* per thread top LOCAL */
	/* except those that are marked assigned_to_ == 2 stay static double */
	if (vectorize && toplocal_) {
		int cnt;
		cnt = 0;
		ITERATE(q, toplocal_) {
		    if (SYM(q)->assigned_to_ != 2) {
			if (SYM(q)->subtype & ARRAY) {
				cnt += SYM(q)->araydim;
			}else{
				++cnt;
			}
		    }
		}
		sprintf(buf, "  _thread[%d]._pval = (double*)ecalloc(%d, sizeof(double));\n", thread_data_index, cnt);
		lappendstr(thread_mem_init_list, buf);
		sprintf(buf, "  free((void*)(_thread[%d]._pval));\n", thread_data_index);
		lappendstr(thread_cleanup_list, buf);
		cnt = 0;
		ITERATE(q, toplocal_) {
		    if (SYM(q)->assigned_to_ != 2) {
			if (SYM(q)->subtype & ARRAY) {
sprintf(buf, "#define %s (_thread[%d]._pval + %d)\n", SYM(q)->name, thread_data_index, cnt);
				cnt += SYM(q)->araydim;
			}else{
sprintf(buf, "#define %s _thread[%d]._pval[%d]\n", SYM(q)->name, thread_data_index, cnt);
				++cnt;
			}
		    }else{ /* stay file static */
			if (SYM(q)->subtype & ARRAY) {
sprintf(buf, "static double %s[%d];\n", SYM(q)->name, SYM(q)->araydim);
			}else{
sprintf(buf, "static double %s;\n", SYM(q)->name);
			}
		    }
			lappendstr(defs_list, buf);
		}
		++thread_data_index;
	}
	/* per thread global data */
	gind = 0;
	if (vectorize) SYMLISTITER {
		s = SYM(q);
		if (s->nrntype & (NRNGLOBAL) && s->assigned_to_ == 1) {
			if (s->subtype & ARRAY) {
				gind += s->araydim;
			}else{
				++gind;
			}
		}
	}
	/* double scalars declared internally */
	Lappendstr(defs_list, "/* declare global and static user variables */\n");
	if (gind) {
		sprintf(buf, "static int _thread1data_inuse = 0;\nstatic double _thread1data[%d];\n#define _gth %d\n", gind, thread_data_index);
		Lappendstr(defs_list, buf);
		sprintf(buf, " if (_thread1data_inuse) {_thread[_gth]._pval = (double*)ecalloc(%d, sizeof(double));\n }else{\n _thread[_gth]._pval = _thread1data; _thread1data_inuse = 1;\n }\n", gind);
		lappendstr(thread_mem_init_list, buf);
		lappendstr(thread_cleanup_list, " if (_thread[_gth]._pval == _thread1data) {\n   _thread1data_inuse = 0;\n  }else{\n   free((void*)_thread[_gth]._pval);\n  }\n");
		++thread_data_index;
	}
	gind = 0;
	acc_globals_update_list = newlist();
 	SYMLISTITER { /* globals are now global with respect to C as well as hoc */
		s = SYM(q);
		if (s->nrntype & (NRNGLOBAL)) {
		    if (vectorize && s->assigned_to_ == 1) {
			if (s->subtype & ARRAY) {
				sprintf(buf, "#define %s%s (_thread1data + %d)\n\
#define %s (_thread[_gth]._pval + %d)\n",
s->name, suffix, gind, s->name, gind);
			}else{
				sprintf(buf, "#define %s%s _thread1data[%d]\n\
#define %s _thread[_gth]._pval[%d]\n",
s->name, suffix, gind, s->name, gind);
			}
			q1 = Lappendstr(defs_list, buf);
			q1->itemtype = VERBATIM;
			if (s->subtype & ARRAY) {
				gind += s->araydim;
			}else{
				++gind;
			}
			continue;
		    }
			if (suffix[0]) {
				Sprintf(buf, "#define %s %s%s\n", s->name, s->name, suffix);
				q1 = Lappendstr(defs_list, buf);
				q1->itemtype = VERBATIM;
			}
			decode_ustr(s, &d1, &d2, buf);
			if (s->subtype & ARRAY) {
				Sprintf(buf, "double %s[%d];\n", s->name, s->araydim);
			}else{
				Sprintf(buf, "double %s = %g;\n", s->name, d1);
			}
			Lappendstr(defs_list, buf);
#if BBCORE
			if (s->subtype & ARRAY) {
				Sprintf(buf, "#pragma acc declare copyin (%s,%d)\n", s->name, s->araydim);
			}else{
				Sprintf(buf, "#pragma acc declare copyin (%s)\n", s->name);
			}
			Lappendstr(defs_list, buf);

			if (s->subtype & ARRAY) {
				Sprintf(buf, "#pragma acc update device (%s,%d) if(nrn_threads->compute_gpu)\n", s->name, s->araydim);
			}else{
				Sprintf(buf, "#pragma acc update device (%s) if(nrn_threads->compute_gpu)\n", s->name);
			}
			Lappendstr(acc_globals_update_list, buf);
#endif
		}
	}

#if BBCORE
	if (acc_globals_update_list) {
		Lappendstr(defs_list, "\nstatic void _acc_globals_update() {\n");
		if (acc_globals_update_list->next != acc_globals_update_list) {
			movelist(acc_globals_update_list->next, acc_globals_update_list->prev, defs_list);
		}
        if (use_celsius) {
            Lappendstr(defs_list, "_celsius_ = celsius;\n");
            Lappendstr(defs_list, "#pragma acc update device(_celsius_)\n");
        }
		Lappendstr(defs_list, "}\n\n");
	}

    if (use_celsius) {
        Lappendstr(defs_list, "#define celsius _celsius_\n");
    }

	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
	Lappendstr(defs_list, "/* some parameters have upper and lower limits */\n");
	Lappendstr(defs_list, "static HocParmLimits _hoc_parm_limits[] = {\n");
	SYMLISTITER {
		s = SYM(q);
		if (s->subtype & PARM) {
			double d1=0., d2=0.;
			if (decode_limits(s, &d1, &d2)) {
				if (s->nrntype & NRNGLOBAL || !point_process) {
Sprintf(buf, "\"%s%s\", %g, %g,\n", s->name, suffix, d1, d2);
				}else{
Sprintf(buf, "\"%s\", %g, %g,\n", s->name, d1, d2);
				}
				Lappendstr(defs_list, buf);
			}
		}
	}
	Lappendstr(defs_list, "0,0,0\n};\n");
	
	units_reg();
#if BBCORE
	Lappendstr(defs_list, "\n#endif /*BBCORE*/\n");
#endif
	
 	SYMLISTITER {
		s = SYM(q);
		if (s->nrntype & (NRNSTATIC)) {
#if VECTORIZE && 0
    if (vectorize) {
diag("No statics allowed for thread safe models:", s->name);
    }
#endif
			decode_ustr(s, &d1, &d2, buf);
			if (s->subtype & ARRAY) {
				Sprintf(buf, "static double %s[%d];\n"
                             "#pragma acc declare create(%s)\n",
                             s->name, s->araydim, s->name);
			}else{
				Sprintf(buf, "static double %s = %g;\n"
                             "#pragma acc declare copyin(%s)\n",
                             s->name, d1, s->name);
			}
			Lappendstr(defs_list, buf);
		}
	}

	Lappendstr(defs_list, "/* connect global user variables to hoc */\n");
	Lappendstr(defs_list,"static DoubScal hoc_scdoub[] = {\n");
 	ITERATE(q, syminorder) {
		s = SYM(q);
		if (s->nrntype & NRNGLOBAL && !(s->subtype & ARRAY)) {
			Sprintf(buf, "\"%s%s\", &%s%s,\n", s->name, suffix, s->name, suffix);
			Lappendstr(defs_list, buf);
		}
	}
	Lappendstr(defs_list, "0,0\n};\n");
	
	/* double vectors */
	Lappendstr(defs_list,"static DoubVec hoc_vdoub[] = {\n");
 	ITERATE(q, syminorder) {
		s = SYM(q);
		if (s->nrntype & NRNGLOBAL && (s->subtype & ARRAY)) {
			Sprintf(buf, "\"%s%s\", %s%s, %d,\n", s->name, suffix, s->name, suffix, s->araydim);
			Lappendstr(defs_list, buf);
		}
	}
	Lappendstr(defs_list, "0,0,0\n};\n");

	Lappendstr(defs_list, "static double _sav_indep;\n");
	if (ba_index_ > 0) {
		Lappendstr(defs_list, "static void _ba1(NrnThread*, Memb_list*, int)");
		for (i=2; i <= ba_index_; ++i) {
			sprintf(buf, ", _ba%d(NrnThread*, Memb_list*, int)", i);
			Lappendstr(defs_list, buf);
		}
		Lappendstr(defs_list, ";\n");
	}

	/******** what normally goes into cabvars.h structures */
	
	/*declaration of the range variables names to HOC */
	Lappendstr(defs_list, "static void nrn_alloc(double*, Datum*, int);\nvoid nrn_init(NrnThread*, Memb_list*, int);\nvoid nrn_state(NrnThread*, Memb_list*, int);\n\
");
	if (brkpnt_exists) {
		Lappendstr(defs_list, "void nrn_cur(NrnThread*, Memb_list*, int);\n");
	    if (!vectorize) {
		Lappendstr(defs_list, "void nrn_jacob(NrnThread*, Memb_list*, int);\n");
	    }
	}
	if (watch_seen_) {
		Lappendstr(defs_list, "void _nrn_watch_check(NrnThread*, Memb_list*);\n");
	}
	/* count the number of pointers needed */
	ppvar_cnt = ioncount + diamdec + pointercount + areadec;
#if CVODE
	if (net_send_seen_) {
		tqitem_index = ppvar_cnt;
		ppvar_semantics(ppvar_cnt, "netsend");
		ppvar_cnt++;
	}
	if (watch_seen_) {
		watch_index = ppvar_cnt;
		for (i=0; i < watch_seen_ ; ++i) {
			ppvar_semantics(i+ppvar_cnt, "watch");
		}
		ppvar_cnt += watch_seen_;
		sprintf(buf, "\n#define _watch_array(arg) _ppvar[(arg + %d)*_STRIDE]", watch_index);
		Lappendstr(defs_list, buf);
		Lappendstr(defs_list, "\n");

		sprintf(buf, "\n"
"#define _nrn_watch_activate(_item)\\\n"
"  if (_watch_rm == 0) {\\\n"
"    int _i;\\\n"
"    for (_i = 1; _i < %d; ++_i) {\\\n"
"      _watch_array(_i) = 0;\\\n"
"    }\\\n"
"    _watch_rm = 1;\\\n"
"  }\\\n"
"  _watch_array(_item) = 2 +"
, watch_seen_);
		Lappendstr(defs_list, buf);

	}
	if (for_netcons_) {
		sprintf(buf, "\n#define _fnc_index %d\n", ppvar_cnt);
		Lappendstr(defs_list, buf);
		ppvar_semantics(ppvar_cnt, "fornetcon");
		ppvar_cnt += 1;
	}
	if (point_process) {
#if BBCORE
	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
		Lappendstr(defs_list, "static void _hoc_destroy_pnt(_vptr) void* _vptr; {\n");
		if (watch_seen_ || for_netcons_) {
			Lappendstr(defs_list, "  Prop* _prop = ((Point_process*)_vptr)->_prop;\n");
		}
#if 0
		if (watch_seen_) {
sprintf(buf, "  if (_prop) { _nrn_free_watch(_prop->dparam, %d, %d);}\n", watch_index, watch_seen_);
			Lappendstr(defs_list, buf);
		}
#endif
		if (for_netcons_) {
sprintf(buf, "  if (_prop) { _nrn_free_fornetcon(&(_prop->dparam[_fnc_index]._pvoid));}\n");
			Lappendstr(defs_list, buf);
		}
		Lappendstr(defs_list, "  destroy_point_process(_vptr);\n}\n");
#if BBCORE
	Lappendstr(defs_list, "\n#endif /*BBCORE*/\n");
#endif
	}
	if (cvode_emit) {
		cvode_ieq_index = ppvar_cnt;
		ppvar_semantics(ppvar_cnt, "cvodeieq");
		ppvar_cnt++;
	}
	cvode_emit_interface();
#endif
	if (destructorfunc->next != destructorfunc) {
		if (! point_process) {
			diag("DESTRUCTOR only permitted for POINT_PROCESS", (char*)0);
		}
		Lappendstr(defs_list, "\n"
			"#if 0 /*BBCORE*/\n"
			"static void _destructor(Prop*);\n"
			"#endif\n"
			);
	}
	
	if (constructorfunc->next != constructorfunc) {
		Lappendstr(defs_list, "\n"
			"#if 0 /*BBCORE*/\n"
			"static void _constructor(Prop*);\n"
			"#endif\n"
			);
	}
	
	Lappendstr(defs_list,
"/* connect range variables in _p that hoc is supposed to know about */\n");
	Lappendstr(defs_list, "\
static const char *_mechanism[] = {\n\
");
	Sprintf(buf, "\"%s\",\n\"%s\",\n", nmodl_version_, mechname);
	Lappendstr(defs_list, buf);
	ITERATE(q, rangeparm) {
		s = SYM(q);
		if (s->subtype & ARRAY) {
			Sprintf(buf, "\"%s%s[%d]\",\n", s->name, rsuffix, s->araydim);
		}else{
			Sprintf(buf, "\"%s%s\",\n", s->name, rsuffix);
		}
		Lappendstr(defs_list, buf);
	}
	Lappendstr(defs_list, "0,\n");
	ITERATE(q, rangedep) {
		s = SYM(q);
		if (s->subtype & ARRAY) {
			Sprintf(buf, "\"%s%s[%d]\",\n", s->name, rsuffix, s->araydim);
		}else{
			Sprintf(buf, "\"%s%s\",\n", s->name, rsuffix);
		}
		Lappendstr(defs_list, buf);
	}
	Lappendstr(defs_list, "0,\n");
	ITERATE(q, rangestate) {
		s = SYM(q);
		if (s->subtype & ARRAY) {
			Sprintf(buf, "\"%s%s[%d]\",\n", s->name, rsuffix, s->araydim);
		}else{
			Sprintf(buf, "\"%s%s\",\n", s->name, rsuffix);
		}
		Lappendstr(defs_list, buf);
	}
	Lappendstr(defs_list, "0,\n");

	/* pointer variable names */
	ITERATE(q, nrnpointers) {
		s = SYM(q);
		if (s->subtype & ARRAY) {
			Sprintf(buf, "\"%s%s[%d]\",\n", s->name, rsuffix, s->araydim);
		}else{
			Sprintf(buf, "\"%s%s\",\n", s->name, rsuffix);
		}
		Lappendstr(defs_list, buf);
	}
	
	Lappendstr(defs_list, "0};\n");

	/*********Creation of the allocation function*/

	if (diamdec) {
		Lappendstr(defs_list, "static Symbol* _morphology_sym;\n");
	}
#if !BBCORE
	if (areadec) {
		Lappendstr(defs_list, "extern Node* nrn_alloc_node_;\n");
	}
#endif
	ITERATE(q, useion) {
		sion = SYM(q);
		Sprintf(buf, "static int _%s_type;\n",sion->name);
		Lappendstr(defs_list, buf);
		if (ldifuslist) {
			sprintf(buf, "static int _type_i%s;\n", sion->name);
			lappendstr(defs_list, buf);
		}
		q=q->next->next->next;
	}
	
	Lappendstr(defs_list, "\n\
static void nrn_alloc(double* _p, Datum* _ppvar, int _type) {\n");
#if BBCORE
	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
	Lappendstr(defs_list, "	/*initialize range parameters*/\n");
	ITERATE(q, rangeparm) {
		s = SYM(q);
		if (s->subtype & ARRAY) {
			continue;
		}
		decode_ustr(s, &d1, &d2, buf);
		Sprintf(buf, "	%s = %g;\n", s->name, d1);
		Lappendstr(defs_list, buf);
	}

	if (diamdec) {
		ppvar_semantics(ioncount + pointercount, "diam");
	}
	if (areadec) {
		ppvar_semantics(ioncount + pointercount + diamdec, "area");
	}

	if (point_process) {
		ioncount = 2;
	}else{
		ioncount = 0;
	}
	ITERATE(q, useion) {
		int dcurdef = 0;
		int need_style = 0;
		sion = SYM(q);
		Sprintf(buf, "prop_ion = need_memb(_%s_sym);\n", sion->name);
		Lappendstr(defs_list, buf);
		if (ldifuslist) {
			sprintf(buf, " _type_i%s = prop_ion->_type;\n", sion->name);
			lappendstr(defs_list, buf);
		}
		ion_promote(q);
		q=q->next;
		ITERATE(q1, LST(q)) {
			SYM(q1)->nrntype |= NRNIONFLAG;
		  	Sprintf(buf,
		  	 "\t_ppvar[%d]._pval = &prop_ion->param[%d]; /* %s */\n",
		  	 ioncount++, iontype(SYM(q1)->name, sion->name),
		  	 SYM(q1)->name);
		  	Lappendstr(defs_list, buf);
		}
		q=q->next;
		ITERATE(q1, LST(q)) {
			int itype = iontype(SYM(q1)->name, sion->name);
			
			if (SYM(q1)->nrntype & NRNIONFLAG) {
				SYM(q1)->nrntype &= ~NRNIONFLAG;
			}else{
			  	Sprintf(buf,
			  	 "\t_ppvar[%d]._pval = &prop_ion->param[%d]; /* %s */\n",
			  	 ioncount++, itype, SYM(q1)->name);
			  	Lappendstr(defs_list, buf);
			}
			if (itype == IONCUR) {
				dcurdef = 1;
				Sprintf(buf,
"\t_ppvar[%d]._pval = &prop_ion->param[%d]; /* _ion_di%sdv */\n",
				 ioncount++, IONDCUR, sion->name);
			  	Lappendstr(defs_list, buf);
			}
			if (itype == IONIN || itype == IONOUT) {
				need_style = 1;
			}
		}
		if (need_style) {
				Sprintf(buf,
"\t_ppvar[%d]._pvoid = (void*)(&(prop_ion->dparam[0]._i)); /* iontype for %s */\n",
				 ioncount++, sion->name);
			  	Lappendstr(defs_list, buf);
		}
		q=q->next;
		if (!dcurdef && ldifuslist) {
				Sprintf(buf,
"\t_ppvar[%d]._pval = &prop_ion->param[%d]; /* _ion_di%sdv */\n",
				 ioncount++, IONDCUR, sion->name);
			  	Lappendstr(defs_list, buf);
		}
	}

#if BBCORE
	Lappendstr(defs_list, "\n#endif /* BBCORE */\n");
#endif

	if (constructorfunc->next != constructorfunc) {
		Lappendstr(defs_list,
			"\n#if 0 /*BBCORE*/\n"
			"if (!nrn_point_prop_) {_constructor(_prop);}\n"
			"#endif\n"
			);
		    if (vectorize) {
			Lappendstr(procfunc, "\n\
#if 0 /*BBCORE*/\n\
static _constructor(_prop)\n\
	Prop *_prop; double* _p; Datum* _ppvar; ThreadDatum* _thread;\n\
{\n\
	_thread = (Datum*)0;\n\
	_p = _prop->param; _ppvar = _prop->dparam;\n\
{\n\
");
		    }else{
			Lappendstr(procfunc, "\n\
#if 0 /*BBCORE*/\n\
static void _constructor(Prop* _prop) {\n\
	_p = _prop->param; _ppvar = _prop->dparam;\n\
{\n\
");
		    }	    	
		movelist(constructorfunc->next, constructorfunc->prev, procfunc);
		Lappendstr(procfunc, "\n}\n}\n#endif /*BBCORE*/\n");
	}
	Lappendstr(defs_list, "\n}\n");

	Lappendstr(defs_list, "static void _initlists();\n");
#if CVODE
	if (cvode_emit) {
		Lappendstr(defs_list, " /* some states have an absolute tolerance */\n");
		Lappendstr(defs_list, "static Symbol** _atollist;\n");
		Lappendstr(defs_list, "static HocStateTolerance _hoc_state_tol[] = {\n");
		ITERATE(q, rangestate) {
			double d1;
			s = SYM(q);
			if (decode_tolerance(s, &d1)) {
				if (!point_process) {
Sprintf(buf, "\"%s%s\", %g,\n", s->name, suffix, d1);
				}else{
Sprintf(buf, "\"%s\", %g,\n", s->name, d1);
				}
				Lappendstr(defs_list, buf);
			}
		}
		Lappendstr(defs_list, "0,0\n};\n");
	}
	if (singlechan_) {
		sprintf(buf, "static _singlechan_declare%d();\n", singlechan_);
		Lappendstr(defs_list, buf);
	}
#endif

#if VECTORIZE
	if (net_send_seen_) {
		if (!net_receive_) {
			diag("can't use net_send if there is no NET_RECEIVE block", (char*)0);
		}
		sprintf(buf, "\n#define _tqitem &(_nt->_vdata[_ppvar[%d*_STRIDE]])\n", tqitem_index);
		Lappendstr(defs_list, buf);
		if (net_receive_buffering_) {
			sprintf(buf, "\
\n#if NET_RECEIVE_BUFFERING\
\n#undef _tqitem\
\n#define _tqitem _ppvar[%d*_STRIDE]\
\n#endif\
\n\n", tqitem_index);
			Lappendstr(defs_list, buf);
		}
		if (net_send_delivered_) {
			insertstr(net_send_delivered_, "\
\n#if !NET_RECEIVE_BUFFERING\
\n  if (_lflag == 1. ) {*(_tqitem) = 0;}\
\n#endif\
\n");
		}
	}
	if (net_receive_) {
		Lappendstr(defs_list, "void _net_receive(Point_process*, int, double);\n");
		if (net_init_q1_) {
			Lappendstr(defs_list, "void _net_init(Point_process*, int, double);\n");
		}
	}
	if (vectorize && thread_mem_init_list->next != thread_mem_init_list) {
		Lappendstr(defs_list, "static void _thread_mem_init(ThreadDatum*);\n");
	}
	if (vectorize && thread_cleanup_list->next != thread_cleanup_list) {
		Lappendstr(defs_list, "static void _thread_cleanup(ThreadDatum*);\n");
	}
	if (uip) {
		lappendstr(defs_list, "static void _update_ion_pointer(Datum*);\n");
	}

	sprintf(buf, "\n#define _psize %d\n#define _ppsize %d\n", parraycount, ppvar_cnt);
	Lappendstr(defs_list, buf);

	Sprintf(buf, "void _%s_reg() {\n\
	int _vectorized = %d;\n", modbase, vectorize);
	Lappendstr(defs_list, buf);
	q = lappendstr(defs_list, "");
	Lappendstr(defs_list, "_initlists();\n");
#else
	Sprintf(buf, "void _%s_reg() {\n	_initlists();\n", modbase);
	Lappendstr(defs_list, buf);
#endif

    if (suffix[0]) { /* not "nothing" */
#if BBCORE
	Lappendstr(defs_list, "_mechtype = nrn_get_mechtype(_mechanism[1]);\n");
	Lappendstr(defs_list, "if (_mechtype == -1) return;\n");
	Lappendstr(defs_list, "_nrn_layout_reg(_mechtype, LAYOUT);\n");

	ITERATE(q, useion) {
		sion = SYM(q);
		Sprintf(buf, "_%s_type = nrn_get_mechtype(\"%s_ion\");",sion->name, sion->name);
		Lappendstr(defs_list, buf);
		q=q->next->next->next;
	}
	Lappendstr(defs_list, "\n#if 0 /*BBCORE*/\n");
#endif
	ITERATE(q, useion) {
		Sprintf(buf, "\tion_reg(\"%s\", %s);\n", SYM(q)->name,
			STR(q->next->next->next));
		Lappendstr(defs_list, buf);
		q = q->next->next->next;
	}
	if (diamdec) {
		Lappendstr(defs_list, "\t_morphology_sym = hoc_lookup(\"morphology\");\n");
	}
	ITERATE(q, useion) {
		Sprintf(buf, "\t_%s_sym = hoc_lookup(\"%s_ion\");\n",
			SYM(q)->name, SYM(q)->name);
		Lappendstr(defs_list, buf);
		q = q->next->next->next;
	}
#if BBCORE
	Lappendstr(defs_list, "\n#endif /*BBCORE*/\n");
#endif
#if VECTORIZE
	if (point_process) {
		sprintf(buf, "\
	_pointtype = point_register_mech(_mechanism,\n\
	 nrn_alloc,%s, nrn_init,\n\
	 hoc_nrnpointerindex,\n\
	 NULL/*_hoc_create_pnt*/, NULL/*_hoc_destroy_pnt*/, /*_member_func,*/\n\
	 %d);\n", brkpnt_str_, vectorize ? 1 + thread_data_index : 0);
	 	Lappendstr(defs_list, buf);
		if (destructorfunc->next != destructorfunc) {
			Lappendstr(defs_list, 
			"    #if 0 /*BBCORE*/\n"
			"    register_destructor(_destructor);\n"
			"    #endif\n"
			);
		}
	}else{
		sprintf(buf, "\
	register_mech(_mechanism, nrn_alloc,%s, nrn_init, hoc_nrnpointerindex, %d);\n", brkpnt_str_, vectorize ? 1 + thread_data_index : 0);
	 	Lappendstr(defs_list, buf);
	}
	if (vectorize && thread_data_index) {
		sprintf(buf, " _extcall_thread = (ThreadDatum*)ecalloc(%d, sizeof(ThreadDatum));\n", thread_data_index);
		Lappendstr(defs_list, buf);
		if (thread_mem_init_list->next != thread_mem_init_list) {
			Lappendstr(defs_list, " _thread_mem_init(_extcall_thread);\n");
			if (gind) {Lappendstr(defs_list, " _thread1data_inuse = 0;\n");}
		}
	}
	if (vectorize) {
		insertstr(extra_pragma_loop_arg, "\n#define _thread_present_ /**/");
		if (thread_data_index) {
			sprintf(buf, ", _thread[0:%d]", thread_data_index);
			insertstr(extra_pragma_loop_arg, buf);
		}
		ITERATE(q, acc_present_list) {
			insertstr(extra_pragma_loop_arg, STR(q));
		}
		insertstr(extra_pragma_loop_arg, "\n");
	}
#endif
#if !BBCORE
	Lappendstr(defs_list, "_mechtype = nrn_get_mechtype(_mechanism[1]);\n");
	lappendstr(defs_list, "    _nrn_setdata_reg(_mechtype, _setdata);\n");
#endif
	if (vectorize && thread_mem_init_list->next != thread_mem_init_list) {
		lappendstr(defs_list, "    _nrn_thread_reg1(_mechtype, _thread_mem_init);\n");
	}
	if (vectorize && thread_cleanup_list->next != thread_cleanup_list) {
		lappendstr(defs_list, "    _nrn_thread_reg0(_mechtype, _thread_cleanup);\n");
	}
	if (uip) {
#if !BBCORE
		lappendstr(defs_list, "    _nrn_thread_reg2(_mechtype, _update_ion_pointer);\n");
#endif
	}
	if (emit_check_table_thread) {
		lappendstr(defs_list, "    _nrn_thread_table_reg(_mechtype, _check_table_thread);\n");
	}
	if (use_bbcorepointer) {
		lappendstr(defs_list, "  hoc_reg_bbcore_read(_mechtype, bbcore_read);\n");
		lappendstr(defs_list, "  hoc_reg_bbcore_write(_mechtype, bbcore_write);\n");
	}
	sprintf(buf, " hoc_register_prop_size(_mechtype, _psize, _ppsize);\n");
	Lappendstr(defs_list, buf);
	if (ppvar_semantics_) ITERATE(q, ppvar_semantics_) {
		sprintf(buf, " hoc_register_dparam_semantics(_mechtype, %d, \"%s\");\n",
		  (int)q->itemtype, q->element.str);
		Lappendstr(defs_list, buf);
	}
	/* Models that write concentration need their INITIAL blocks called
	   before those that read the concentration or reversal potential. */
	i = 0;
	ITERATE(q, useion) {
		ITERATE(q1, LST(q->next->next)) {
			int type;
			type = iontype(SYM(q1)->name, SYM(q)->name);
			if (type == IONIN || type == IONOUT) {
				i += 1;
			}
		}
		q = q->next->next->next;
	}
	if (i) {
		Lappendstr(defs_list, "\tnrn_writes_conc(_mechtype, 0);\n");
	}

#if CVODE
printf("cvode_emit=%d cvode_not_allowed=%d\n", cvode_emit, cvode_not_allowed);
	if (cvode_emit) {
		Lappendstr(defs_list,"\
	hoc_register_cvode(_mechtype, _ode_count, _ode_map, _ode_spec, _ode_matsol);\n");
		Lappendstr(defs_list,"\
	hoc_register_tolerance(_mechtype, _hoc_state_tol, &_atollist);\n");
		if (ion_synonym) {
Lappendstr(defs_list, "	hoc_register_synonym(_mechtype, _ode_synonym);\n");
		}
	}else if (0 && cvode_not_allowed) {
		Lappendstr(defs_list, "\
	hoc_register_cvode(_mechtype, _ode_count, 0, 0, 0);\n");
	}
	if (singlechan_) {
		sprintf(buf, "hoc_reg_singlechan(_mechtype, _singlechan_declare%d);\n", singlechan_);
		Lappendstr(defs_list, buf);
	}
#endif
	if (artificial_cell) {
		if (brkpnt_exists || !net_receive_
		    || nrnpointers->next != nrnpointers
		    || useion->next != useion
		) {
			printf(
"Notice: ARTIFICIAL_CELL is a synonym for POINT_PROCESS which hints that it\n\
only affects and is affected by discrete events. As such it is not\n\
located in a section and is not associated with an integrator\n"
);
		}
		sprintf(buf,  "add_nrn_artcell(_mechtype, %d);\n", tqitem_index);
		Lappendstr(defs_list, buf);
	}
	if (net_event_seen_) {
		Lappendstr(defs_list, "add_nrn_has_net_event(_mechtype);\n");
	}
	if (watch_seen_) {
		Lappendstr(defs_list, "hoc_register_watch_check(_nrn_watch_check, _mechtype);\n");
	}
	if (net_receive_buffering_) {
		Lappendstr(defs_list, "\
\n#if NET_RECEIVE_BUFFERING\
\n  hoc_register_net_receive_buffering(_net_buf_receive, _mechtype);\
\n#endif\
\n");
		if (net_send_seen_ || net_event_seen_) {
			Lappendstr(defs_list, "\
\n#if NET_RECEIVE_BUFFERING\
\n  hoc_register_net_send_buffering(_mechtype);\
\n#endif\
\n");
		}
	}
	if (net_receive_) {
		Lappendstr(defs_list, "pnt_receive[_mechtype] = _net_receive;\n");
		if (net_init_q1_) {
			Lappendstr(defs_list, "pnt_receive_init[_mechtype] = _net_init;\n");
		}
		sprintf(buf, "pnt_receive_size[_mechtype] = %d;\n", net_receive_);
		Lappendstr(defs_list, buf);
	}
	if (for_netcons_) {
		sprintf(buf, "add_nrn_fornetcons(_mechtype, _fnc_index);\n");
		Lappendstr(defs_list, buf);
	}
	q = ba_list_;
	for (i = 1; i <= ba_index_; ++i) {
		List* lst;
		q = q->next;
#if 0
                if (electrode_current) {
			insertstr(ITM(q), " \
#if EXTRACELLULAR\n\
if (_nd->_extnode) {\n\
   v = NODEV(_nd) +_nd->_extnode->_v[0];\n\
}else\n\
#endif\n\
{\n\
   v = NODEV(_nd);\n\
}\n");
		}else{
			insertstr(ITM(q), " v = NODEV(_nd);\n");
		}
#endif
		lst = get_ion_variables(0);
		if (lst->next != lst->prev) {
			move(lst->next, lst->prev, ITM(q));
			freelist(lst);
		}
		q = q->next;
		lst = set_ion_variables(0);
		if (lst->next != lst->prev) {
			move(lst->next, lst->prev, ITM(q));
			freelist(lst);
		}
		q = q->next;
		sprintf(buf, "\thoc_reg_ba(_mechtype, _ba%d, %s);\n", i, STR(q));
		Lappendstr(defs_list, buf);
	}
	if (ldifuslist) {
		Lappendstr(defs_list, "\thoc_register_ldifus1(_difusfunc);\n");
		Linsertstr(defs_list, "static void _difusfunc(ldifusfunc2_t, NrnThread*);\n");
	}
    } /* end of not "nothing" */
#if BBCORE
	Lappendstr(defs_list, "\
	hoc_register_var(hoc_scdoub, hoc_vdoub, NULL);\n");
#else
	Lappendstr(defs_list, "\
	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);\n");
#endif
#if !BBCORE
	if (GETWD(buf)) {
		char buf1[NRN_BUFSIZE];
#if defined(MINGW)
{		char* cp;
		for (cp = buf; *cp; ++cp) {
			if (*cp == '\\') { *cp = '/'; }
		}
}
#endif
sprintf(buf1, "\tivoc_help(\"help ?1 %s %s/%s\\n\");\n", mechname, buf, finname);
		Lappendstr(defs_list, buf1);
	}
    if (suffix[0]) {
	Lappendstr(defs_list, "hoc_register_limits(_mechtype, _hoc_parm_limits);\n");
	Lappendstr(defs_list, "hoc_register_units(_mechtype, _hoc_parm_units);\n");
    }
#endif
	Lappendstr(defs_list, "}\n"); /* end of _reg */
	if (vectorize && thread_mem_init_list->next != thread_mem_init_list) {
		Lappendstr(procfunc, "\nstatic void _thread_mem_init(ThreadDatum* _thread) {\n");
		move(thread_mem_init_list->next, thread_mem_init_list->prev, procfunc);
		Lappendstr(procfunc, "}\n");
	}
	if (vectorize && thread_cleanup_list->next != thread_cleanup_list) {
		Lappendstr(procfunc, "\nstatic void _thread_cleanup(ThreadDatum* _thread) {\n");
		move(thread_cleanup_list->next, thread_cleanup_list->prev, procfunc);
		Lappendstr(procfunc, "}\n");
	}
	if (uip) {
		move(uip->next, uip->prev, procfunc);
	}
	if (destructorfunc->next != destructorfunc) {
	    if (vectorize) {
		Lappendstr(procfunc, "\n\
#if 0 /*BBCORE*/\n\
static _destructor(_prop)\n\
	Prop *_prop; double* _p; Datum* _ppvar; ThreadDatum* _thread;\n\
{\n\
	_thread = (Datum*)0;\n\
	_p = prop->param; _ppvar = _prop->dparam;\n\
{\n\
");
	    }else{
		Lappendstr(procfunc, "\n\
#if 0 /*BBCORE*/\n\
static void _destructor(Prop* _prop) {\n\
	_p = _prop->param; _ppvar = _prop->dparam;\n\
{\n\
");
	    }	    	
		movelist(destructorfunc->next, destructorfunc->prev, procfunc);
		Lappendstr(procfunc, "\n}\n}\n#endif /*BBCORE*/\n");
	}
	if (ldifuslist) {
		ldifusreg();
	}
	if (watch_seen_) {
		emit_nrn_watch_check_code();
	}
	SYMLISTITER {
		s = SYM(q);
		if ((s->subtype & PARM)) {
			warn_ignore(s);
		}
	}
}

void warn_ignore(s) Symbol* s; {
	int b;
	double d1, d2;
	b = 0;
	if (s->nrntype & (NRNEXTRN | NRNPRANGEIN | NRNPRANGEOUT)) b = 1;
	if (strcmp(s->name, "v") == 0) b = 1;
	
	decode_ustr(s, &d1, &d2, buf);
	if (d1 == 0.0) b = 0;
	if (b) {
		printf("Warning: Default %g of PARAMETER %s will be ignored and set by NEURON.\n", d1, s->name);
	}
}

void ldifusreg() {
	Item* q, *qdexp, *qb1, *qvexp, *qb2, *q1;
	char* cfindex, *dfdcur;
	Symbol* s, *d;
	int n;
	
	/* ldifuslist format: series of symbol qdexp qb1 svexp qb2
			indexforflux dflux/dconc */
	n = 0;
	ITERATE(q, ldifuslist) {
		s = SYM(q); q = q->next;
		qdexp = ITM(q); q = q->next;
		qb1 = ITM(q); q = q->next;
		qvexp = ITM(q); q = q->next;
		qb2 = ITM(q); q = q->next;
		cfindex = STR(q); q = q->next;
		dfdcur = STR(q);
		++n;
sprintf(buf, "static void* _difspace%d;\nextern double nrn_nernst_coef();\n\
static double _difcoef%d(int _i, double* _p, Datum* _ppvar, double* _pdvol, double* _pdfcdc, ThreadDatum* _thread, NrnThread* _nt) {\n  \
 *_pdvol = ", n, n);
		lappendstr(procfunc, buf);
		for (q1 = qvexp; q1 != qb2; q1 = q1->next) {
			lappenditem(procfunc, q1);
		}
		if (dfdcur[0]) {
			sprintf(buf, ";\n\
 if (_i == %s) {\n  *_pdfcdc = %s;\n }else{ *_pdfcdc=0.;}\n", cfindex, dfdcur);
 		}else{
 			sprintf(buf, "; *_pdfcdc=0.;\n");
 		}
		lappendstr(procfunc, buf);
		lappendstr(procfunc, "  return");
		for (q1 = qdexp; q1 != qb1; q1 = q1->next) {
			lappenditem(procfunc, q1);
		}
		lappendstr(procfunc, ";\n}\n");
	}
#if MAC
	lappendstr(procfunc, "static void _difusfunc(_f, _nt) void *_f; NrnThread* _nt; {int _i;\n");
#else
	lappendstr(procfunc, "static void _difusfunc(_f, _nt) void(*_f)(); NrnThread* _nt; {int _i;\n");
#endif
	n = 0;
	ITERATE(q, ldifuslist) {
		s = SYM(q); q = q->next;
		qdexp = ITM(q); q = q->next;
		qb1 = ITM(q); q = q->next;
		qvexp = ITM(q); q = q->next;
		qb2 = ITM(q); q = q->next;
		cfindex = STR(q); q = q->next;
		dfdcur = STR(q);
		++n;

		if (s->subtype & ARRAY) {
#if MAC
sprintf(buf, " for (_i=0; _i < %d; ++_i) mac_difusfunc(_f, _mechtype, _difcoef%d, &_difspace%d, _i, ", s->araydim, n, n);
#else
sprintf(buf, " for (_i=0; _i < %d; ++_i) (*_f)(_mechtype, _difcoef%d, &_difspace%d, _i, ", s->araydim, n, n);
#endif
		}else{
#if MAC
sprintf(buf, " mac_difusfunc(_f,_mechtype, _difcoef%d, &_difspace%d, 0, ", n, n);
#else
sprintf(buf, " (*_f)(_mechtype, _difcoef%d, &_difspace%d, 0, ", n, n);
#endif
		}
		lappendstr(procfunc, buf);

		sprintf(buf, "D%s", s->name);
		d = lookup(buf);
		assert(d);
		if (s->nrntype & IONCONC) {
			sprintf(buf, "%d, %d",
			  - (s->ioncount_ + 1), d->varnum);
		}else{
			sprintf(buf, "%d, %d", s->varnum, d->varnum);
		}
		lappendstr(procfunc, buf);
		lappendstr(procfunc, ", _nt);\n");
	}
	lappendstr(procfunc, "}\n");
}

static int decode_limits(sym, pg1, pg2)
	Symbol *sym;
	double *pg1, *pg2;
{
	int i;
	double d1;
	if (sym->subtype & PARM) {
		char* cp;
		int n;
		assert(sym->u.str);
		for (n=0, cp = sym->u.str; *cp; ++cp) {
			if (*cp == '\n') {
				++n;
				if (n == 3) {
					++cp;
					break;
				}
			}
		}
		i = sscanf(cp, "%lf %lf\n", pg1, pg2);
		if (i == 2) {
			return 1;
		}
	}
	return 0;
}

static int decode_tolerance(sym, pg1)
	Symbol *sym;
	double *pg1;
{
	int i;
	double d1;
	if (sym->subtype & STAT) {
		char* cp;
		int n;
		for (n=0, cp = sym->u.str; *cp; ++cp) {
			if (*cp == '\n') {
				++n;
				if (n == 3) {
					++cp;
					break;
				}
			}
		}
		i = sscanf(cp, "%lf\n", pg1);
		if (i == 1) {
			return 1;
		}
	}
	return 0;
}

void decode_ustr(sym, pg1, pg2, s)	/* decode sym->u.str */
	Symbol *sym;
	char *s;
	double *pg1, *pg2;
{
	int i, n;
	char *cp, *cp1;
	
	switch (sym->subtype & (INDEP | DEP | STAT | PARM)) {

	case INDEP:	/* but doesnt get all info */
	case DEP:
	case STAT:
		assert(sym && sym->u.str);
		if (sym->subtype & ARRAY) { /* see parsact.c */
			i = sscanf(sym->u.str, "[%*d]\n%lf%*c%lf", pg1, pg2);
		}else{			
			i = sscanf(sym->u.str, "%lf%*c%lf", pg1, pg2);
		}
		assert(i == 2);
		for (n=0, cp = sym->u.str; n < 2;) {
			if (*cp++ == '\n') {
				n++;
			}
		}
		for (cp1 = s; *cp != '\n';) {
			*cp1++ = *cp++;
		}
		*cp1 = '\0';
		break;

	case PARM:
		assert(sym && sym->u.str);
		if (sym->subtype & ARRAY) { /* see parsact.c */
			i = sscanf(sym->u.str, "[%*d]\n%lf\n%s", pg1, s);
		}else{			
			i = sscanf(sym->u.str, "%lf\n%s", pg1, s);
		}
		if (i == 1) {
			s[0] = '\0';
			i = 2;
		}
		assert(i == 2);
		break;
	default:
		diag(sym->name, " does not have a proper declaration");
	}
	if (s[0] == '0') {s[0] = '\0';}
}

void units_reg() {
	Symbol* s;
	Item* q;
	double d1, d2;
	char u[NRN_BUFSIZE];
	
	Lappendstr(defs_list, "static HocParmUnits _hoc_parm_units[] = {\n");
	ITERATE (q, syminorder) {
		s = SYM(q);
		if (s->nrntype & NRNGLOBAL) {
			decode_ustr(s, &d1, &d2, u);
			if (u[0]) {
				sprintf(buf, "\"%s%s\", \"%s\",\n", s->name, suffix, u);
				lappendstr(defs_list, buf);
			}
		}
	}
	ITERATE (q, rangeparm) {
		s = SYM(q);
		decode_ustr(s, &d1, &d2, u);
		if (u[0]) {
			sprintf(buf, "\"%s%s\", \"%s\",\n", s->name, rsuffix, u);
			lappendstr(defs_list, buf);
		}
	}
	ITERATE (q, rangestate) {
		s = SYM(q);
		decode_ustr(s, &d1, &d2, u);
		if (u[0]) {
			sprintf(buf, "\"%s%s\", \"%s\",\n", s->name, rsuffix, u);
			lappendstr(defs_list, buf);
		}
	}
	ITERATE (q, rangedep) {
		s = SYM(q);
		decode_ustr(s, &d1, &d2, u);
		if (u[0]) {
			sprintf(buf, "\"%s%s\", \"%s\",\n", s->name, rsuffix, u);
			lappendstr(defs_list, buf);
		}
	}
	ITERATE (q, nrnpointers) {
		s = SYM(q);
		decode_ustr(s, &d1, &d2, u);
		if (u[0]) {
			sprintf(buf, "\"%s%s\", \"%s\",\n", s->name, rsuffix, u);
			lappendstr(defs_list, buf);
		}
	}
	Lappendstr(defs_list, "0,0\n};\n");
}

static void var_count(s)
	Symbol *s;
{
	defs_h(s);
		s->used = varcount++;
		s->varnum = parraycount;
		if (s->subtype & ARRAY) {
			parraycount += s->araydim;
		}else{
			parraycount++;
		}
}

static void defs_h(s)
	Symbol *s;
{
	Item *q;
	
	if (s->subtype & ARRAY) {
		Sprintf(buf, "#define %s (_p + %d*_STRIDE)\n", s->name, parraycount);
		q = lappendstr(defs_list, buf);
	} else {
		Sprintf(buf, "#define %s _p[%d*_STRIDE]\n", s->name, parraycount);
		q = lappendstr(defs_list, buf);
	}
	q->itemtype = VERBATIM;
}


void nrn_list(q1, q2)
	Item *q1, *q2;
{
	List **plist = (List **)0;
	Item *q;
		
	switch (SYM(q1)->type) {
	case RANGE:
		plist = (List **)0;
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->nrntype |= NRNRANGE;
		}
		break;
	case SUFFIX:
		plist = (List **)0;
		mechname = SYM(q2)->name;
		if (strcmp(SYM(q1)->name, "POINT_PROCESS") == 0) {
			point_process = 1;
		}else if (strcmp(SYM(q1)->name, "ARTIFICIAL_CELL") == 0) {
			point_process = 1;
			artificial_cell = 1;
		}
		set_suffix();
		break;
	case ELECTRODE_CURRENT:
		electrode_current = 1;
	case NONSPECIFIC:
		plist = &currents;
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->nrntype |= NRNRANGE;
		}
		break;
	case SECTION:
		diag("NEURON SECTION variables not implemented", (char *)0);
		break;
	case GLOBAL:
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->nrntype |= NRNGLOBAL | NRNNOTP;
		}
		plist = (List **)0;
		break;
	case EXTERNAL:
#if VECTORIZE
threadsafe("Use of EXTERNAL is not thread safe.");
#endif
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->nrntype |= NRNEXTRN | NRNNOTP;
		}
		plist = (List **)0;
		break;
	case POINTER:
threadsafe("Use of POINTER is not thread safe.");
		plist = &nrnpointers;
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->nrntype |= NRNNOTP | NRNPOINTER;
		}
		break;
	case BBCOREPOINTER:
threadsafe("Use of BBCOREPOINTER is not thread safe.");
		plist = &nrnpointers;
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->nrntype |= NRNNOTP | NRNBBCOREPOINTER;
		}
		use_bbcorepointer = 1;
		break;
	}
	if (plist) {
		if (!*plist) {
			*plist = newlist();
		}
		assert (q1 != q2);
		movelist(q1->next, q2, *plist);
	}
}

void bablk(ba, type, q1, q2)
	int ba, type;
	Item *q1, *q2;
{
	Item* qb, *qv, *q;
	qb = insertstr(q1->prev->prev, "/*");
	insertstr(q1, "*/\n");
	if (!ba_list_) {
		ba_list_ = newlist();
	}
	sprintf(buf, "static void _ba%d(NrnThread* _nt, Memb_list* _ml, int _type) ", ++ba_index_);
	insertstr(q1, buf);
	q = q1->next;

	qv = insertstr(q, ""
"  double* _p; Datum* _ppvar; ThreadDatum* _thread;\n"
"  int* _ni; double v; int _iml, _cntml_padded, _cntml_actual;\n"
"  _cntml_actual = _ml->_nodecount;\n"
"  _cntml_padded = _ml->_nodecount_padded;\n"
"  _ni = _ml->_nodeindices;\n"
"  _thread = _ml->_thread;\n"
"  double * _nt_data = _nt->_data;\n"
"  double * _vec_v = _nt->_actual_v;\n"
"  int stream_id = _nt->stream_id;\n"
"  #if LAYOUT == 1 /*AoS*/\n"
"  for (_iml = 0; _iml < _cntml_actual; ++_iml) {\n"
"    _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;\n"
"  #elif LAYOUT == 0 /*SoA*/\n"
"    _p = _ml->_data; _ppvar = _ml->_pdata;\n"
"  /* insert compiler dependent ivdep like pragma */\n"
"  _PRAGMA_FOR_VECTOR_LOOP_\n"
"  _PRAGMA_FOR_STATE_ACC_LOOP_\n"
"  for (_iml = 0; _iml < _cntml_actual; ++_iml) {\n"
"  #else /* LAYOUT > 1 */ /*AoSoA*/\n"
"  #error AoSoA not implemented.\n"
"  for (;;) { /* help clang-format properly indent */\n"
"  #endif\n"
"    v = _vec_v[_ni[_iml]];\n"
);
	insertstr(q2, "}\n");
	movelist(qb, q2, procfunc);

	ba = (ba == BEFORE) ? 10 : 20; /* BEFORE or AFTER */
	ba += (type == BREAKPOINT) ? 1 : 0;
	ba += (type == SOLVE) ? 2 : 0;
	ba += (type == INITIAL1) ? 3 : 0;
	ba += (type == STEP) ? 4 : 0;
	lappenditem(ba_list_, qv->next);
	lappenditem(ba_list_, q2);
	sprintf(buf, "%d", ba);
	lappendstr(ba_list_, buf);
}

int ion_declared(Symbol* s) {
	Item* q;
	int used = 0;
	ITERATE(q, useion) {
		if (SYM(q) == s) {
			used = 1;
		}
		q = q->next->next->next;
	}
	return used;
}

void nrn_use(q1, q2, q3, q4)
	Item *q1, *q2, *q3, *q4;
{
	int used, i;
	Item *q, *qr, *qw;
	List *readlist, *writelist;
	Symbol *ion;
	
	ion = SYM(q1);
	/* is it already used */
	used = ion_declared(SYM(q1));
	if (used) { /* READ gets promoted to WRITE */
		diag("mergeing of neuron models not supported yet", (char *)0);
	}else{ /* create all the ionic variables */
		Lappendsym(useion, ion);
		readlist = newlist();
		writelist = newlist();
		qr = lappendsym(useion, SYM0);
		qw = lappendsym(useion, SYM0);
		if (q4) {
			lappendstr(useion, STR(q4));
		}else{
			lappendstr(useion, "-10000.");
		}
		LST(qr) = readlist;
		LST(qw) = writelist;
		if (q2) { Item *qt = q2->next;
			move(q1->next->next, q2, readlist);
			if (q3) {
				move(qt->next, q3, writelist);
			}
		}else if (q3) {
			move(q1->next->next, q3, writelist);
		}
		ITERATE(q, readlist) {
			i = iontype(SYM(q)->name, ion->name);
			if (i == IONCUR) {
				SYM(q)->nrntype |= NRNCURIN;
			}else{
				SYM(q)->nrntype |= NRNPRANGEIN;
				if (i == IONIN || i == IONOUT) {
					SYM(q)->nrntype |= IONCONC;
				}
			}
		}
		ITERATE(q, writelist) {
			i = iontype(SYM(q)->name, ion->name);
			if (i == IONCUR) {
				if (!currents) {
					currents = newlist();
				}
				Lappendsym(currents, SYM(q));
				SYM(q)->nrntype |= NRNCUROUT;
			}else{
				SYM(q)->nrntype |= NRNPRANGEOUT;
				if (i == IONIN || i == IONOUT) {
					SYM(q)->nrntype |= IONCONC;
				}
			}
		}
	}
}

static int iontype(s1, s2)	/* returns index of variable in ion mechanism */
	char *s1, *s2;
{
	Sprintf(buf, "i%s", s2);
	if (strcmp(buf, s1) == 0) {
		return IONCUR;
	}
	Sprintf(buf, "e%s", s2);
	if (strcmp(buf, s1) == 0) {
		return IONEREV;
	}
	Sprintf(buf, "%si", s2);
	if (strcmp(buf, s1) == 0) {
		return IONIN;
	}
	Sprintf(buf, "%so", s2);
	if (strcmp(buf, s1) == 0) {
		return IONOUT;
	}
	Sprintf(buf, "%s is not a valid ionic variable for %s", s1, s2);
	diag(buf, (char *)0);
	return -1;
}

static Symbol *ifnew_install(name)
	char *name;
{
	Symbol *s;
	
	if ((s = lookup(name)) == SYM0) {
		s = install(name, NAME);
		parminstall(s, "0", "", "");
	}
	return s;
}

static void nrndeclare() {
	Symbol *s;
	Item *q;
	
	s=lookup("diam"); if (s) {
		if (s->nrntype & (NRNRANGE|NRNGLOBAL)) {
diag(s->name, " cannot be a RANGE or GLOBAL variable for this mechanism");
		}
		s->nrntype |= NRNNOTP|NRNPRANGEIN; diamdec=1;
	}
	s=lookup("area"); if (s) {
		if (s->nrntype & (NRNRANGE|NRNGLOBAL)) {
diag(s->name, " cannot be a RANGE or GLOBAL variable for this mechanism");
		}
		s->nrntype |= NRNNOTP|NRNPRANGEIN; areadec=1;
	}
#if VECTORIZE
    if (vectorize) {
	s = ifnew_install("v");
	s->nrntype = NRNNOTP; /* this is a lie, it goes in at end specially */
	/* no it is not a lie. Use an optimization where voltage passed via arguments */
    }else
#endif
    {
	s = ifnew_install("v");
	s->nrntype |= NRNSTATIC | NRNNOTP;
    }
	s = ifnew_install("t");
	s->nrntype |= NRNEXTRN | NRNNOTP;
	s = ifnew_install("dt");
	s->nrntype |= NRNEXTRN | NRNNOTP;
	vectorize_substitute(lappendstr(defs_list, "\n#define t nrn_threads->_t\n#define dt nrn_threads->_dt\n"), "\n#define t _nt->_t\n#define dt _nt->_dt\n");

	s=lookup("usetable"); if (s) { s->nrntype |= NRNGLOBAL | NRNNOTP;}
	s=lookup("celsius");if(s){s->nrntype |= NRNEXTRN | NRNNOTP;}
	s=lookup("celcius"); if (s) diag("celcius should be spelled celsius",
					(char *)0);
	
 	ITERATE(q, syminorder) {
		s = SYM(q);
		if (s->type == NAME || s->type == PRIME) {
			if (s->subtype & PARM && s->nrntype & NRNRANGE) {
				Lappendsym(rangeparm, s);
			} else if (s->subtype & STAT) {
				s->nrntype |= NRNRANGE;
				Lappendsym(rangestate, s);
			} else if (s->subtype & DEP && s->nrntype & NRNRANGE) {
				Lappendsym(rangedep, s);
			}
			if (s != indepsym && !s->nrntype) {
				if (s->subtype & PARM) {
					if (s->usage & EXPLICIT_DECL) {
						s->nrntype |= NRNGLOBAL;
						s->nrntype |= NRNNOTP;
					}else{
						s->nrntype |= NRNSTATIC;
						s->nrntype |= NRNNOTP;
					}
				}
			}
		}
	}
	/* some ionic variables don't need duplicates known to hoc */
	del_range(rangeparm);
	del_range(rangestate);
	del_range(rangedep);
}

static void del_range(range)
	List *range;
{
	Item *q, *q1;
	Symbol *s;
	
	for (q = ((Item *)range)->next; q != (Item *)range; q = q1) {
		q1 = q->next;
		s = SYM(q);
		if (s->nrntype & (NRNPRANGEIN | NRNPRANGEOUT)) {
			delete(q);
		}
	}
}
	

static void declare_p() {
	Item *q;
	Symbol* s;
	int pcs = 0;
	
	ITERATE(q, syminorder) {
		SYM(q)->used = -1;
	}
	ITERATE(q, rangeparm) {
		var_count(SYM(q));
	}
	ITERATE(q, rangedep) {
		var_count(SYM(q));
	}
	ITERATE(q, rangestate) {
		var_count(SYM(q));
	}
	ITERATE(q, syminorder) {
		if (!(SYM(q)->nrntype & NRNNOTP) && SYM(q)->used < 0) {
			var_count(SYM(q));
		}
	}
#if VECTORIZE
	if (vectorize) {
		s = ifnew_install("_v_unused");
		var_count(s);
		pcs = 1;
	}
#endif
	if (brkpnt_exists) {
	    if (vectorize) {
		s = ifnew_install("_g_unused");
		var_count(s);
		pcs = 2;
	    }else{
		s = ifnew_install("_g");
		var_count(s);
	    }
	}
	if (debugging_ && net_receive_) {
		s = ifnew_install("_tsav");
		var_count(s);
	}
	if (pcs) {
		sprintf(buf, "\
\n#ifndef NRN_PRCELLSTATE\
\n#define NRN_PRCELLSTATE 0\
\n#endif\
\n#if NRN_PRCELLSTATE\
\n#define _PRCELLSTATE_V _v_unused = _v;\
\n#define _PRCELLSTATE_G %s\
\n#else\
\n#define _PRCELLSTATE_V /**/\
\n#define _PRCELLSTATE_G /**/\
\n#endif\
\n", (pcs == 2) ? "_g_unused = _g;" : "/**/");
		Lappendstr(defs_list, buf);
	}
}

List *set_ion_variables(block)
	int block;	/* 0 means equation block , 2 means initial block */
{
	/*ARGSUSED*/
	Item *q, *q1, *qconc;
	char* in;
	static List *l;
	int declared = 0;

	l = newlist();
	ITERATE(q, useion) {
		in = SYM(q)->name;
		q = q->next;
		q = q->next;
		qconc = (Item*)0;
		ITERATE(q1, LST(q)) {
			if (SYM(q1)->nrntype & NRNCUROUT) {
				if ( block == 0) {
Sprintf(buf, " _ion_%s += %s", SYM(q1)->name, breakpoint_current(SYM(q1))->name);
					Lappendstr(l, buf);
					if (point_process) {
						Sprintf(buf, "* 1.e2/ (_nd_area);\n");
					}else{
						Sprintf(buf, ";\n");
					}
				}else{
					buf[0] = '\0';
				}
			}else{
				if (iontype(SYM(q1)->name, in) != IONEREV) {
					qconc = q1;
				}
Sprintf(buf, " _ion_%s = %s;\n", SYM(q1)->name, SYM(q1)->name);
			}
			Lappendstr(l, buf);
		}
		q = q->next;
		/* when INITIAL block is called, if it modifies the concentrations
		   then the reversal potential should be recomputed in case
		   other mechanisms need the true initial value. This would be
		   rare since most initial blocks do not depend on erev. Instead
		   the right value will be present due to fcurrent or cvode f(y).
		   However, this fastidiousness cant hurt. It just makes ion_style
		   in effect always at least for initialization.
		*/
		/* sure enough, someone needed to demote the ion_style so
		   that erev is decoupled from concentrations. So we need
		   another variable pointing to the ionstyle
		*/
		if (block == 2 && qconc) {
			int ic =  iontype(SYM(qconc)->name, in);
			if (ic == IONIN) {
				ic = 1;
			}else if (ic == IONOUT) {
				ic = 2;
			}else{
				assert(0);
			}
            /* first arg is for the charge and memb_list, second and third give 
             * pointer to erev, fourth arg is the style, seventh needed for figuring 
             * out _cntml if SoA. Note 2nd argument added as pe because we see errors 
             * with pointer arithmetic in OpenACC cray compiler
             */
			Sprintf(buf, "    %s _pe = (&(_ion_%s));\n", declared ? "" : "double*", SYM(qconc)->name);
			Lappendstr(l, buf);
			Sprintf(buf, "    Memb_list* _%s_ml;\n", in);
			Lappendstr(l, buf);
			Sprintf(buf, "    _%s_ml = _nt->_ml_list[_%s_type];\n", in, in);
			Lappendstr(l, buf);
			Sprintf(buf, "    %s _tmp_cntml = _%s_ml->_nodecount_padded;\n", declared ? "" : "int", in);
			declared = 1;
			Lappendstr(l, buf);
			Sprintf(buf, "    nrn_wrote_conc(_%s_type, _pe, %d, _style_%s, nrn_ion_global_map, celsius, _tmp_cntml);\n",
				in, ic, in);
			Lappendstr(l, buf);
		}
	}
	return l;
}

List *get_ion_variables(block)
	int block;	/* 0 means equation block */
			/* 2 means ode_spec and ode_matsol blocks */
{
	/*ARGSUSED*/
	Item *q, *q1;
	static List *l;

	l = newlist();
	ITERATE(q, useion) {
		q = q->next;
		ITERATE(q1, LST(q)) {
			if (block == 2 && (SYM(q1)->nrntype & IONCONC) && (SYM(q1)->subtype & STAT)) {
				continue;
			}
Sprintf(buf, " %s = _ion_%s;\n", SYM(q1)->name, SYM(q1)->name);
			Lappendstr(l, buf);
if (point_process && (SYM(q1)->nrntype & NRNCURIN)) {
Fprintf(stderr, "WARNING: Dimensions may be wrong for READ %s with POINT_PROCESS\n", SYM(q1)->name);
}
		}
		q = q->next;
		ITERATE(q1, LST(q)) {
			if (block == 2 && (SYM(q1)->nrntype & IONCONC) && (SYM(q1)->subtype & STAT)) {
				continue;
			}
			if (SYM(q1)->nrntype & IONCONC) {
Sprintf(buf, " %s = _ion_%s;\n", SYM(q1)->name, SYM(q1)->name);
				Lappendstr(l, buf);
			}
			if (SYM(q1)->subtype & STAT) {
if (SYM(q1)->nrntype & NRNCUROUT) {
Fprintf(stderr, "WARNING: WRITE %s with it a STATE may not be translated correctly\n", SYM(q1)->name);
}
			}
		}
		q = q->next;
	}
	return l;
}

/* note: _nt_data is only defined in nrn_init, nrn_cur and nrn_state where ions are used in the current mod files */
static int iondef(p_pointercount) int *p_pointercount; {
	int ioncount, it, need_style;
	Item *q, *q1, *q2;
	Symbol *sion;
	char ionname[100];

	ioncount = 0;
	if (point_process) {
		ioncount = 2;
		q = lappendstr(defs_list, "#define _nd_area  _nt_data[_ppvar[0*_STRIDE]]\n");
		q->itemtype = VERBATIM;
		ppvar_semantics(0, "area");
		ppvar_semantics(1, "pntproc");
	}
	ITERATE(q, useion) {
		int dcurdef = 0;
		if (!uip) {
			uip = newlist();
			lappendstr(uip, "static void _update_ion_pointer(Datum* _ppvar) {\n");
		}
		need_style = 0;
		sion = SYM(q);
		sprintf(ionname, "%s_ion", sion->name);
		q=q->next;
		ITERATE(q1, LST(q)) {
			SYM(q1)->nrntype |= NRNIONFLAG;
			Sprintf(buf, "#define _ion_%s		_nt_data[_ppvar[%d*_STRIDE]]\n",
				SYM(q1)->name, ioncount);
			q2 = lappendstr(defs_list, buf);
			q2->itemtype = VERBATIM;
			sprintf(buf, "  nrn_update_ion_pointer(_%s_sym, _ppvar, %d, %d);\n",
				sion->name, ioncount, iontype(SYM(q1)->name, sion->name));
#if 0 /*BBCORE*/
			lappendstr(uip, buf);
#endif /*BBCORE*/
			SYM(q1)->ioncount_ = ioncount;
			ppvar_semantics(ioncount, ionname);
			ioncount++;
		}
		q=q->next;
		ITERATE(q1, LST(q)) {
			if (SYM(q1)->nrntype & NRNIONFLAG) {
				SYM(q1)->nrntype &= ~NRNIONFLAG;
			}else{
				Sprintf(buf, "#define _ion_%s	_nt_data[_ppvar[%d*_STRIDE]]\n",
					SYM(q1)->name, ioncount);
				q2 = lappendstr(defs_list, buf);
				q2->itemtype = VERBATIM;
				sprintf(buf, "  nrn_update_ion_pointer(_%s_sym, _ppvar, %d, %d);\n",
					sion->name, ioncount, iontype(SYM(q1)->name, sion->name));
#if 0 /*BBCORE*/
				lappendstr(uip, buf);
#endif /*BBCORE*/
				SYM(q1)->ioncount_ = ioncount;
				ppvar_semantics(ioncount, ionname);
				ioncount++;
			}
			it = iontype(SYM(q1)->name, sion->name);
			if (it == IONCUR) {
				dcurdef = 1;
Sprintf(buf, "#define _ion_di%sdv\t_nt_data[_ppvar[%d*_STRIDE]]\n", sion->name, ioncount);
				q2 = lappendstr(defs_list, buf);
				q2->itemtype = VERBATIM;
				sprintf(buf, "  nrn_update_ion_pointer(_%s_sym, _ppvar, %d, 4);\n",
					sion->name, ioncount);
#if 0 /*BBCORE*/
				lappendstr(uip, buf);
#endif /*BBCORE*/
				ppvar_semantics(ioncount, ionname);
				ioncount++;
			}
			if (it == IONIN || it == IONOUT) { /* would have wrote_ion_conc */
				need_style = 1;
			}
		}
		if (need_style) {
Sprintf(buf, "#define _style_%s\t_ppvar[%d]\n", sion->name, ioncount);
			q2 = lappendstr(defs_list, buf);
			q2->itemtype = VERBATIM;
			sprintf(buf, "#%s", ionname);
			ppvar_semantics(ioncount, buf);
			ioncount++;
		}
		q=q->next;
		if (!dcurdef && ldifuslist) {
Sprintf(buf, "#define _ion_di%sdv\t_nt_data[_ppvar[%d*_STRIDE]]\n", sion->name, ioncount);
				q2 = lappendstr(defs_list, buf);
				q2->itemtype = VERBATIM;
				sprintf(buf, "  nrn_update_ion_pointer(_%s_sym, _ppvar, %d, 4);\n",
					sion->name, ioncount);
#if 0 /*BBCORE*/
				lappendstr(uip, buf);
#endif /*BBCORE*/
				ppvar_semantics(ioncount, ionname);
				ioncount++;
		}
	}
	*p_pointercount = 0;
	ITERATE(q, nrnpointers) {
		sion = SYM(q);
	    if (sion->nrntype & NRNPOINTER) {
		Sprintf(buf, "#define %s	_nt_data[_ppvar[%d*_STRIDE]]\n",
			sion->name, ioncount + *p_pointercount);
		ppvar_semantics(ioncount + *p_pointercount, "pointer");
	    }
	    if (sion->nrntype & NRNBBCOREPOINTER) {
		Sprintf(buf, "#define _p_%s	_nt->_vdata[_ppvar[%d*_STRIDE]]\n",
			sion->name, ioncount + *p_pointercount);
		ppvar_semantics(ioncount + *p_pointercount, "bbcorepointer");
	    }
		sion->used = ioncount + *p_pointercount;
		q2 = lappendstr(defs_list, buf);
		q2->itemtype = VERBATIM;
		(*p_pointercount)++;
	}

	if (diamdec) { /* must be last */
		Sprintf(buf, "#define diam	_nt->_data[_ppvar[%d*_STRIDE]]\n", ioncount + *p_pointercount);
		q2 = lappendstr(defs_list, buf);
		q2->itemtype = VERBATIM;
	} /* notice that ioncount is not incremented */
	if (areadec) { /* must be last, if we add any more the administrative
			procedures must be redone */
		Sprintf(buf, "#define area	_nt->_data[_ppvar[%d*_STRIDE]]\n", ioncount+ *p_pointercount + diamdec);
		q2 = lappendstr(defs_list, buf);
		q2->itemtype = VERBATIM;
	} /* notice that ioncount is not incremented */
	if (uip) { lappendstr(uip, "}\n"); }
	return ioncount;
}

void ppvar_semantics(int i, const char* name) {
	Item* q;
	if (!ppvar_semantics_) { ppvar_semantics_ = newlist(); }
	q = Lappendstr(ppvar_semantics_, name);
	q->itemtype = (short)i;
}

List *begin_dion_stmt()
{
	Item *q, *q1, *qbrak;
	static List *l;
	char *strion;

	l = newlist();
	qbrak = lappendstr(l, "\t{");
	ITERATE(q, useion) {
		strion = SYM(q)->name;
		q = q->next;
		q = q->next;
		ITERATE(q1, LST(q)) {
			if (SYM(q1)->nrntype & NRNCUROUT) {
				Sprintf(buf, " _di%s = %s;\n",
				strion,  SYM(q1)->name);
				Lappendstr(l, buf);
				Sprintf(buf, "double _di%s;\n", strion);
				Insertstr(qbrak->next, buf);
			}
		}
		q = q->next;
	}
	return l;
}

List *end_dion_stmt(strdel)
	char *strdel;
{
	Item *q, *q1;
	static List *l;
	char *strion;

	l = newlist();
	ITERATE(q, useion) {
		strion = SYM(q)->name;
		q = q->next;
		q = q->next;
		ITERATE(q1, LST(q)) {
			if (SYM(q1)->nrntype & NRNCUROUT) {
Sprintf(buf, " _ion_di%sdv += (_di%s - %s)/%s",
				strion, strion, SYM(q1)->name, strdel);
				Lappendstr(l, buf);
				if (point_process) {
					Lappendstr(l, "* 1.e2/ (_nd_area);\n");
				}else{
					Lappendstr(l, ";\n");
				}
			}
		}
		q = q->next;
	}
	Lappendstr(l, "\t}\n");
	return l;
}

static void ion_promote(qion)
	Item* qion;
{
	Item* q;
	char* in;
	int conc, rev;
	int type;
	conc = 0;
	rev = 0;
	in = SYM(qion)->name;
	ITERATE(q, LST(qion->next)) { /* check READ */
		type = iontype(SYM(q)->name, in);
		if (type == IONIN || type == IONOUT) {
			conc = 1;
		}
		if (type == IONEREV) {
			rev = 1;
		}
	}
	ITERATE(q, LST(qion->next->next)) { /* promote if WRITE */
		type = iontype(SYM(q)->name, in);
		if (type == IONIN) {
			Lappendstr(defs_list, "nrn_check_conc_write(_prop, prop_ion, 1);\n");
			conc = 3;
		}
		if (type == IONOUT) {
			Lappendstr(defs_list, "nrn_check_conc_write(_prop, prop_ion, 0);\n");
			conc = 3;
		}
		if (type == IONEREV) {
			rev = 3;
		}
	}
	if (conc || rev) {
		Sprintf(buf, "nrn_promote(prop_ion, %d, %d);\n", conc, rev);
		Lappendstr(defs_list, buf);
	}
}

#define NRNFIX(arg) if (strcmp(n, arg) == 0) e=1;

void nrn_var_assigned(s) Symbol* s; {
	int e;
	char* n;
	if (s->assigned_to_ == 0) {
		s->assigned_to_ = 1;
	}
	if (protect_) {
		s->assigned_to_ = 2;
	}
	e = 0;
	n = s->name;
	NRNFIX("area");
	NRNFIX("diam");
	NRNFIX("t");
	NRNFIX("dt");
	NRNFIX("celsius");
	if (e) {
diag(s->name, " is a special NEURON variable that should not be\n assigned a value\
 in a model description file\n");
	}
}

#if CVODE

static int cvode_valid_, using_cvode;
static int cvode_num_, cvode_neq_;
static Symbol* cvode_fun_;

void slist_data(s, indx, findx) Symbol* s; int indx, findx; {
	/* format: number of pairs, followed by findx, indx pairs */
	int* pi;
	int i, n;
	if (s->slist_info_) {
		/* i'd use realloc but to avoid portability problems */
		/* this probably will never get executed anyway */
		n = s->slist_info_[0] + 1;
		pi = (int*)emalloc((1 + 2*n)*sizeof(int));
		for (i=2*(n-1); i > 0; --i) {
			pi[i] = s->slist_info_[i];
		}
		free(s->slist_info_);
		s->slist_info_ = pi;
		pi[0] = n;
		pi[2*n-1] = findx;
		pi[2*n] = indx;
	}else{
		s->slist_info_ = pi = (int*)emalloc(3*sizeof(int));
		pi[0] = 1;
		pi[1] = findx;
		pi[2] = indx;
	}
}

int slist_search(n, s) int n; Symbol* s; {
	int i, *pi;
	pi = s->slist_info_;
if (pi == (int*)0) {
	diag(s->name, " not really a STATE; Ie. No differential equation for it.\n");
}
	assert(pi);
	for (i=0; i < pi[0]; ++i) {
		if (pi[1+2*i] == n) {
			return pi[2+2*i];
		}
	}
	assert(0);
	return 0;
}

static void cvode_conc_map() {
	/* pv index is slist index, ppd index is to the concentration
	pointer to the ion concentration is eg. &(ion_cai). Unfortunately
	the slist index has nothing to do with the _p array index.
	To recover the slist index, an slist_index list was made for
	every slist which consists of an slist ordered list of state symbols
	*/
	/*
	   also must handle case where user WRITE cai but cai is not a STATE
	   since inefficiency occurs due to inability to set eca when
	   states are predicted
	*/
	Item* q, *q1, *q2, *q3;
	int sindex;
	ITERATE(q, useion) {
		q = q->next;
		q = q->next;
		ITERATE(q1, LST(q)) {
			if (SYM(q1)->nrntype & IONCONC) {
				if ((SYM(q1)->subtype & STAT)) {
					sindex = slist_search(cvode_num_, SYM(q1));
					sprintf(buf, "\t_pv[%d] = &(_ion_%s);\n",
						sindex, SYM(q1)->name);
					lappendstr(procfunc, buf);
				}else{ /* not a STATE but WRITE it*/
/*its got to have an assignment in a SOLVE block and that assignment
better not depend on intermediate variables that depend on states
because we will assign cai using only that statement prior to
calling the nernst equation code.
*/
					int b = 0;
					if (!ion_synonym) {
						ion_synonym = newlist();
					}
					ITERATE(q2, procfunc) {
	if (q2->itemtype == SYMBOL && SYM(q2) == SYM(q1)) {
		q3 = q2->next;
		if (q3->itemtype == SYMBOL && strcmp(SYM(q3)->name, "=") == 0) {
/*printf(" found reference to %s = ...\n", SYM(q2)->name);*/
			sprintf(buf, "_ion_%s = ", SYM(q2)->name);
			lappendstr(ion_synonym, buf);
			for (q3 = q3->next; q3 != procfunc->prev; q3 = q3->next) {
				lappenditem(ion_synonym, q3);
				if (q3->itemtype == SYMBOL && SYM(q3) == semi) {
#if 0
				if (q3->itemtype == STRING && strchr(STR(q3), ';')) {
					char* e, *s = stralloc(STR(q3), (char*)0);
					e = strchr(s, ';');
					*e = '\0';
					sprintf(buf, "%s;\n", s);
printf("|%s||%s||%s|\n",STR(q3), s, buf);
					lappendstr(ion_synonym, buf);
#endif
					b = 1;
					break;
				}
			}
			break;
		}
	}
					}
					if (b == 0) {
diag(SYM(q1)->name, " is WRITE but is not a STATE and has no assignment statement");
					}
				}
			}
		}
		q = q->next;
	}
}

void out_nt_ml_frag(List* p) {
		vectorize_substitute(lappendstr(p, "  ThreadDatum* _thread;\n"), "  double* _p; Datum* _ppvar; ThreadDatum* _thread;\n");
		Lappendstr(p, "  Node* _nd; double _v; int _iml, _cntml_padded, _cntml_actual;\n\
  _cntml_actual = _ml->_nodecount;\n\
  _cntml_padded = _ml->_nodecount_padded;\n\
  _thread = _ml->_thread;\n\
  for (_iml = 0; _iml < _cntml_actual; ++_iml) {\n\
    _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];\n\
    _nd = _ml->_nodelist[_iml];\n\
    v = NODEV(_nd);\n\
");
}

void cvode_emit_interface() {
	List* lst;
	Item* q, *q1;
	if (cvode_not_allowed) {
		Lappendstr(defs_list, "\n\
static int _ode_count(int);\n");
		sprintf(buf, "\n\
static int _ode_count(int _type){ hoc_execerror(\"%s\", \"cannot be used with CVODE\"); return 0;}\n",
			mechname);
		Lappendstr(procfunc, buf);
	}else if (cvode_emit) {

		Lappendstr(defs_list, "\n\
static int _ode_count(int);\n\
static void _ode_map(int, double**, double**, double*, Datum*, double*, int);\n\
static void _ode_spec(NrnThread*, Memb_list*, int);\n\
static void _ode_matsol(NrnThread*, Memb_list*, int);\n\
");
		sprintf(buf, "\n\
static int _ode_count(int _type){ return %d;}\n",
			cvode_neq_);
		Lappendstr(procfunc, buf);
		sprintf(buf, "\n#define _cvode_ieq _ppvar[%d]._i\n",cvode_ieq_index);
		Lappendstr(defs_list, buf);

   if (cvode_fun_->subtype == PROCED) {
	cvode_proced_emit();
   }else{
		Lappendstr(procfunc, "\nstatic void _ode_spec(NrnThread* _nt, Memb_list* _ml, int _type) {\n");
		out_nt_ml_frag(procfunc);
		lst = get_ion_variables(1);
		if (lst->next->itemtype) movelist(lst->next, lst->prev, procfunc);
		sprintf(buf,"    _ode_spec%d", cvode_num_);
		Lappendstr(procfunc, buf);
		vectorize_substitute(lappendstr(procfunc, "();\n"), "(_threadargs_);\n");
		lst = set_ion_variables(1);
		if (lst->next->itemtype) movelist(lst->next, lst->prev, procfunc);
		Lappendstr(procfunc, "}}\n");

		Lappendstr(procfunc, "\n\
static void _ode_map(int _ieq, double** _pv, double** _pvdot, double* _pp, Datum* _ppd, double* _atol, int _type) {");
		vectorize_substitute(lappendstr(procfunc, "\n"), "\n\
	double* _p; Datum* _ppvar;\n");
		sprintf(buf, "\
	int _i; _p = _pp; _ppvar = _ppd;\n\
	_cvode_ieq = _ieq;\n\
	for (_i=0; _i < %d; ++_i) {\n\
		_pv[_i] = _pp + _slist%d[_i];  _pvdot[_i] = _pp + _dlist%d[_i];\n\
		_cvode_abstol(_atollist, _atol, _i);\n\
	}\n",
			cvode_neq_, cvode_num_, cvode_num_);
		Lappendstr(procfunc, buf);
/* need to take care of case where a state is an ion concentration. Replace
the _pp pointer with a pointer to the actual ion model's concentration */
		cvode_conc_map();
		Lappendstr(procfunc, "}\n"); 
		if (ion_synonym) {
			Lappendstr(defs_list, "static void _ode_synonym(int, double**, Datum**);\n");
			Lappendstr(procfunc, "\
static void _ode_synonym(int _cnt, double** _pp, Datum** _ppd) {");
		vectorize_substitute(lappendstr(procfunc, "\n"), "\n\
	double* _p; Datum* _ppvar;\n");
			Lappendstr(procfunc, "\
	int _i; \n\
	for (_i=0; _i < _cnt; ++_i) {_p = _pp[_i]; _ppvar = _ppd[_i];\n");
			movelist(ion_synonym->next, ion_synonym->prev, procfunc);
			Lappendstr(procfunc, "}}\n");
		}

		Lappendstr(procfunc, "\nstatic void _ode_matsol(NrnThread* _nt, Memb_list* _ml, int _type) {\n");
		out_nt_ml_frag(procfunc);
		lst = get_ion_variables(1);
		if (lst->next->itemtype) movelist(lst->next, lst->prev, procfunc);
		if (cvode_fun_->subtype == KINF) {
			int i = cvode_num_;
sprintf(buf, "_cvode_sparse(&_cvsparseobj%d, %d, _dlist%d, _p, _ode_matsol%d, &_coef%d);\n",
				i, cvode_neq_, i, i, i);
			Lappendstr(procfunc, buf);
sprintf(buf, "_cvode_sparse_thread(&_thread[_cvspth%d]._pvoid, %d, _dlist%d, _p, _ode_matsol%d, _ppvar, _thread, _nt);\n",
				i, cvode_neq_, i, i);
			vectorize_substitute(procfunc->prev, buf);
			
		}else{
			sprintf(buf, "_ode_matsol%d", cvode_num_);
			Lappendstr(procfunc, buf);
			vectorize_substitute(lappendstr(procfunc, "();\n"), "(_threadargs_);\n");
		}
		Lappendstr(procfunc, "}}\n");
	}
	/* handle the state_discontinuities */
	if (state_discon_list_) ITERATE(q, state_discon_list_) {
		Symbol* s;
		int sindex;
		q1 = ITM(q);
		s = SYM(q1);
		if (q1->itemtype == SYMBOL && (s->subtype & STAT)) {
			sindex = slist_search(cvode_num_, s);
			sprintf(buf, "_cvode_ieq + %d, &", sindex);
			replacstr(q1->prev, buf);
		}
	}
   }
}

void cvode_proced_emit() {
		sprintf(buf, "\n\
static void _ode_spec(Node* _nd, double* _pp, Datum* _ppd) {\n\
	_p = _pp; _ppvar = _ppd; v = NODEV(_nd);\n\
	%s();\n}\n",
			cvode_fun_->name);

		Lappendstr(procfunc, buf);
		sprintf(buf, "\n\
static void _ode_map(int _ieq, double** _pv, doubl** _pvdot, double* _pp){}\n");
		Lappendstr(procfunc, buf);

		Lappendstr(procfunc, "\n\
static void _ode_matsol(Node* _nd, double* _pp, Datum* _ppd){}\n");
}

void cvode_interface(fun, num, neq) Symbol* fun; int num, neq; {
	/* if only one then allowed and emit */
	cvode_valid_ = 1;
	cvode_not_allowed = (using_cvode++) ? 1 : 0;
	cvode_emit = !cvode_not_allowed;
	cvode_num_ = num;
	cvode_neq_ = neq;
	cvode_fun_ = fun;
	if (cvode_fun_->subtype == PROCED) {
		cvode_emit = 0;
		return;
	}
	Sprintf(buf, "\n\
static int _ode_spec%d(_threadargsproto_);\n\
/*static int _ode_matsol%d(_threadargsproto_);*/\n\
", num, num);
	Linsertstr(procfunc, buf);
}

void cvode_valid() {
	static int once;
	if (!cvode_valid_ && !once++) {
		Fprintf(stderr, "Notice: This mechanism cannot be used with CVODE\n");
		cvode_not_allowed = 1;
	}
	cvode_valid_ = 0;
}

void cvode_rw_cur(b) char* b; {
	/* if a current is READ and WRITE then call the correct _ode_spec
	   since it may compute some aspect of the current */
	Item* q, *q1;
	int type;
	Symbol* sion;
	b[0] = '\0';
	ITERATE(q, useion) {
		sion = SYM(q);
		q = q->next;
		ITERATE (q1, LST(q)) {
			type = SYM(q1)->nrntype;
			if ((type & NRNCURIN) && (type & NRNCUROUT)) {
				if (!cvode_not_allowed && cvode_emit) {
					if (vectorize) {
sprintf(b, "if (_nt->_vcv) { _ode_spec%d(_threadargs_); }\n", cvode_num_);
					}else{
sprintf(b, "if (_nt->_vcv) { _ode_spec%d(); }\n", cvode_num_);
					}
					return;
				}
			}
		}
		q = q->next;
		q = q->next;
	}
}
#endif

const char* net_boilerplate(int flag) {
	char b[1000];
	b[0] = '\0';
	
	sprintf(buf, "\n\
   NrnThread* _nt;\n\
   int _tid = _pnt->_tid; \n\
   _nt = nrn_threads + _tid;\n\
");

	sprintf(b, "%s", buf);

	sprintf(buf, "%s\
   _thread = (ThreadDatum*)0; \n\
   double *_weights = _nt->_weights;\n\
   _args = _weights + _weight_index;\n\
   _ml = _nt->_ml_list[_pnt->_type];\n\
   _cntml_actual = _ml->_nodecount;\n\
   _cntml_padded = _ml->_nodecount_padded;\n\
   _iml = _pnt->_i_instance;\n\
#if LAYOUT == 1 /*AoS*/\n\
   _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;\n\
#endif\n\
#if LAYOUT == 0 /*SoA*/\n\
   _p = _ml->_data; _ppvar = _ml->_pdata;\n\
#endif\n\
#if LAYOUT > 1 /*AoSoA*/\n\
#error AoSoA not implemented.\n\
#endif\n\
", b);
	return buf;
}

void emit_net_receive_buffering_code() {
	Item* q;
	if (!net_receive_buffering_) { return; }
	q = net_receive_block_begin_;
	insertstr(q, "\n#if NET_RECEIVE_BUFFERING");

	sprintf(buf, "\
\n#undef t\
\n#define t _nrb_t\
\nstatic inline void _net_receive_kernel(double, Point_process*, int _weight_index, double _flag);\
\nvoid _net_buf_receive(NrnThread* _nt) {\
\n  if (!_nt->_ml_list) { return; }\
\n  Memb_list* _ml = _nt->_ml_list[_mechtype];\
\n  if (!_ml) { return; }\
\n  NetReceiveBuffer_t* _nrb = _ml->_net_receive_buffer;\
");
	insertstr(q, buf);

sprintf(buf, "\
\n  int _di;\
\n  int stream_id = _nt->stream_id;\
\n  Point_process* _pnt = _nt->pntprocs;\
\n  int _pnt_length = _nt->n_pntproc - _nrb->_pnt_offset;\
\n  int _displ_cnt = _nrb->_displ_cnt;\
\n  _PRAGMA_FOR_NETRECV_ACC_LOOP_ \
\n  for (_di = 0; _di < _displ_cnt; ++_di) {\
\n    int _inrb;\
\n    int _di0 = _nrb->_displ[_di];\
\n    int _di1 = _nrb->_displ[_di + 1];\
\n    for (_inrb = _di0; _inrb < _di1; ++_inrb) {\
\n      int _i = _nrb->_nrb_index[_inrb];\
\n      int _j = _nrb->_pnt_index[_i];\
\n      int _k = _nrb->_weight_index[_i];\
\n      double _nrt = _nrb->_nrb_t[_i];\
\n      double _nrflag = _nrb->_nrb_flag[_i];\
\n      _net_receive_kernel(_nrt, _pnt + _j, _k, _nrflag);\
\n    }\
\n  }\
\n  #pragma acc wait(stream_id)\
\n  _nrb->_displ_cnt = 0;\
\n  _nrb->_cnt = 0;\
\n  /*printf(\"_net_buf_receive_%s  %%d\\n\", _nt->_id);*/\
\n", suffix);
	insertstr(q, buf);

	if (net_send_seen_ || net_event_seen_) {
		sprintf(buf, "\
\n  {\
\n  NetSendBuffer_t* _nsb = _ml->_net_send_buffer;\
\n#if defined(_OPENACC) && !defined(DISABLE_OPENACC)\
\n  #pragma acc update self(_nsb->_cnt) if(_nt->compute_gpu)\
\n  update_net_send_buffer_on_host(_nt, _nsb);\
\n#endif\
\n  int _i;\
\n  for (_i=0; _i < _nsb->_cnt; ++_i) {\
\n    net_sem_from_gpu(_nsb->_sendtype[_i], _nsb->_vdata_index[_i],\
\n      _nsb->_weight_index[_i], _nt->_id, _nsb->_pnt_index[_i],\
\n      _nsb->_nsb_t[_i], _nsb->_nsb_flag[_i]);\
\n  }\
\n  _nsb->_cnt = 0;\
\n#if defined(_OPENACC) && !defined(DISABLE_OPENACC)\
\n  #pragma acc update device(_nsb->_cnt) if (_nt->compute_gpu)\
\n#endif\
\n  }\
\n");
		insertstr(q, buf);
	}

	insertstr(q, "\n}\n");

	if (net_send_seen_ || net_event_seen_) {
		sprintf(buf, "\
\nstatic void _net_send_buffering(NetSendBuffer_t* _nsb, int _sendtype, int _i_vdata, int _weight_index,\
\n int _ipnt, double _t, double _flag) {\
\n  int _i = 0;\
\n  #pragma acc atomic capture\
\n  _i = _nsb->_cnt++;\
\n  if (_i >= _nsb->_size) {\
\n  }\
\n  _nsb->_sendtype[_i] = _sendtype;\
\n  _nsb->_vdata_index[_i] = _i_vdata;\
\n  _nsb->_weight_index[_i] = _weight_index;\
\n  _nsb->_pnt_index[_i] = _ipnt;\
\n  _nsb->_nsb_t[_i] = _t;\
\n  _nsb->_nsb_flag[_i] = _flag;\
\n}\n");
		insertstr(q, buf);
	}

	sprintf(buf, "\
\nvoid _net_receive (Point_process* _pnt, int _weight_index, double _lflag) {\
\n  NrnThread* _nt = nrn_threads + _pnt->_tid;\
\n  NetReceiveBuffer_t* _nrb = _nt->_ml_list[_mechtype]->_net_receive_buffer;\
\n  if (_nrb->_cnt >= _nrb->_size){\
\n    realloc_net_receive_buffer(_nt, _nt->_ml_list[_mechtype]);\
\n  }\
\n  _nrb->_pnt_index[_nrb->_cnt] = _pnt - _nt->pntprocs;\
\n  _nrb->_weight_index[_nrb->_cnt] = _weight_index;\
\n  _nrb->_nrb_t[_nrb->_cnt] = _nt->_t;\
\n  _nrb->_nrb_flag[_nrb->_cnt] = _lflag;\
\n  ++_nrb->_cnt;\
\n}\
\n");
	insertstr(q, buf);

	sprintf(buf, "\
\nstatic void _net_receive_kernel(double _nrb_t, Point_process* _pnt, int _weight_index, double _lflag)\
\n#else\
\n");
	insertstr(q, buf);

	/* close off */
	q = net_receive_block_open_brace_;
	insertstr(q, "\n#endif\n");
}

static void emit_nrn_watch_check_code() {
	Item* q, *par1, *par2, *flag;
	int iw;
	char* par2par;
	Lappendstr(procfunc, "\n"
"void _nrn_watch_check(NrnThread* _nt, Memb_list* _ml) {\n"
"  double* _p; Datum* _ppvar; ThreadDatum* _thread;\n"
"  int* _ni; double v; int _iml, _cntml_padded, _cntml_actual;\n"
"  _cntml_actual = _ml->_nodecount;\n"
"  _cntml_padded = _ml->_nodecount_padded;\n"
"  _ni = _ml->_nodeindices;\n"
"  _thread = _ml->_thread;\n"
"  double * _nt_data = _nt->_data;\n"
"  double * _vec_v = _nt->_actual_v;\n"
"  int stream_id = _nt->stream_id;\n"
);
	/* for gpu, performance may be better factored into below ITERATE */
	Lappendstr(procfunc, "\n"
"  #if LAYOUT == 1 /*AoS*/\n"
"  for (_iml = 0; _iml < _cntml_actual; ++_iml) {\n"
"    _p = _ml->_data + _iml*_psize; _ppvar = _ml->_pdata + _iml*_ppsize;\n"
"  #elif LAYOUT == 0 /*SoA*/\n"
"    _p = _ml->_data; _ppvar = _ml->_pdata;\n"
"  /* insert compiler dependent ivdep like pragma */\n"
"  _PRAGMA_FOR_VECTOR_LOOP_\n"
"  _PRAGMA_FOR_STATE_ACC_LOOP_\n"
"  for (_iml = 0; _iml < _cntml_actual; ++_iml) {\n"
"  #else /* LAYOUT > 1 */ /*AoSoA*/\n"
"  #error AoSoA not implemented.\n"
"  for (;;) { /* help clang-format properly indent */\n"
"  #endif\n"
"    v = _vec_v[_ni[_iml]];\n"
);

	iw = 0; /* 0 unused Datum but for consistent size with NEURON... */
	ITERATE(q, watch_data_) { /* triples of par1, par2, flag items */
		par1 = ITM(q); q = q->next;
		par2 = ITM(q); q = q->next;
		flag = ITM(q);
		++iw;

		par2par = items_as_string(par1, par2);
		sprintf(buf, "\n"
"    if (_watch_array(%d)&2) {\n"
"      if %s {\n"
"        if ((_watch_array(%d)&1) == 0) {\n"
"          #if NET_RECEIVE_BUFFERING\n"
"          _net_send_buffering(_ml->_net_send_buffer, 0, _tqitem, 0, _ppvar[1*_STRIDE], t +  0.0 , %s );\n"
"          #else\n"
"          net_send ( _tqitem, -1, (Point_process*) _nt->_vdata[_ppvar[1*_STRIDE]], t +  0.0 , %s ) ;\n"
"          #endif\n"
"        }\n"
"        _watch_array(%d) = 3;\n"
"      }else{\n"
"        _watch_array(%d) = 2;\n"
"      }\n"
"    }\n"
		, iw, par2par, iw, STR(flag), STR(flag), iw, iw);
		Lappendstr(procfunc, buf);
		free(par2par);
	}

	sprintf(buf, "  }\n");
	Lappendstr(procfunc, buf);

	Lappendstr(procfunc, "\n"
"#if NET_RECEIVE_BUFFERING\n"
"  NetSendBuffer_t* _nsb = _ml->_net_send_buffer;\n"
"#if defined(_OPENACC) && !defined(DISABLE_OPENACC)\n"
"  #pragma acc wait(stream_id)\n"
"  #pragma acc update self(_nsb->_cnt) if(_nt->compute_gpu)\n"
"  update_net_send_buffer_on_host(_nt, _nsb);\n"
"#endif\n"
"  {int _i;\n"
"  for (_i=0; _i < _nsb->_cnt; ++_i) {\n"
"    net_sem_from_gpu(_nsb->_sendtype[_i], _nsb->_vdata_index[_i],\n"
"      _nsb->_weight_index[_i], _nt->_id, _nsb->_pnt_index[_i],\n"
"      _nsb->_nsb_t[_i], _nsb->_nsb_flag[_i]);\n"
"  }}\n"
"  _nsb->_cnt = 0;\n"
"#if defined(_OPENACC) && !defined(DISABLE_OPENACC)\n"
"  #pragma acc update device(_nsb->_cnt) if(_nt->compute_gpu)\n"
"#endif\n"
"#endif\n"
	);

	Lappendstr(procfunc, "}\n");
}

void net_receive(qblk, qarg, qp1, qp2, qstmt, qend)
	Item* qblk, *qarg, *qp1, *qp2, *qstmt, *qend;
{
	Item* q, *q1;
	Symbol* s;
	int i, b;
	char snew[100];
	if (net_receive_) {
		diag("Only one NET_RECEIVE block allowed", (char*)0);
	}
	if (!point_process) {
		diag("NET_RECEIVE can only exist in a POINT_PROCESS", (char*)0);
	}
	net_receive_ = 1;
	net_receive_block_begin_ = qblk;
	deltokens(qp1, qp2);
	insertstr(qstmt, "(Point_process* _pnt, int _weight_index, double _lflag)");
	i = 0;
	ITERATE(q1, qarg) if (q1->next != qarg) { /* skip last "flag" arg */
		s = SYM(q1);
		sprintf(snew, "_args[%d]", i);
		++i;
		for (q = qstmt; q != qend; q = q->next) {
			if (q->itemtype == SYMBOL && SYM(q) == s) {
				replacstr(q, snew);
			}
		}
	}
	net_send_delivered_ = qstmt;
	q = insertstr(qstmt, "\n{");
	net_receive_block_open_brace_ = q;
	vectorize_substitute(q, "\n\
{  double* _p; Datum* _ppvar; ThreadDatum* _thread; double v;\n\
   Memb_list* _ml; int _cntml_padded, _cntml_actual; int _iml; double* _args;\n\
");

	if (watch_seen_) {
		insertstr(qstmt, "  int _watch_rm = 0;\n");
	}
	q = insertstr(qstmt, net_boilerplate(1));
	if (debugging_) {
	    if (1) {
		insertstr(qstmt, " #if !defined(_OPENACC) \n assert(_tsav <= t); \n #endif \n _tsav = t;");
	    }else{
		insertstr(qstmt, " if (_tsav > t){ extern char* hoc_object_name(); hoc_execerror(hoc_object_name(_pnt->ob), \":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay\");}\n _tsav = t;");
	    }
	}
	insertstr(qend, "}");
insertstr(qend, "\
\n#if NET_RECEIVE_BUFFERING\
\n#undef t\
\n#define t _nt->_t\
\n#endif\
\n");

	if (!artificial_cell) {
		Symbol* ions[10]; int j, nion=0;
		/* v can be changed in the NET_RECEIVE block since it is
		   called between integrator steps and before a re_init
		   But no need to do so if it is not used.
		*/
		Symbol* vsym = lookup("v");
		for (q = qstmt; q != qend; q = q->next) {
			if (q->itemtype == SYMBOL && SYM(q) == vsym) {
				insertstr(qstmt, " v = VEC_V(_ml->_nodeindices[_iml]);\n");
				insertstr(qend, "\n VEC_V(_ml->_nodeindices[_iml]) = v;\n");
				break;
			}
		}
		/* if an ion concentration
		is mentioned then we need to get the relevant value
		on entry and possibly set a value on exit
		Do not allow mention of reversal potential or current
		*/
		for (q = qstmt; q != qend; q = q->next) {
			if (q->itemtype == SYMBOL && SYM(q)->type == NAME) {
				s = SYM(q);
				if ((s->nrntype & (NRNPRANGEIN | NRNPRANGEOUT)) == 0) {
					continue;
				}
				if ((s->nrntype & IONCONC) == 0) {
diag(s->name, " :only concentrations can be mentioned in a NET_RECEIVE block");
				}
				/* distinct only */
				for (j=0; j < nion; ++j) {
					if (s == ions[j]) {
						break;
					}
				}
				if (j == nion) {
					if (nion >= 10) {
diag("too many ions mentioned in NET_RECEIVE block (limit 10", (char*)0);
					}
					ions[nion] = s;
					++nion;
				}
			}
		}
		for (j = 0; j < nion; ++j) {
			sprintf(buf, "%s   %s = _ion_%s;\n", (j==0)?"\n":"", ions[j]->name, ions[j]->name);
			insertstr(qstmt, buf);
		}
		for (j = 0; j < nion; ++j) {
			if (ions[j]->subtype & STAT) {
				sprintf(buf, "%s   _ion_%s = %s;\n", (j==0)?"\n":"", ions[j]->name, ions[j]->name);
				insertstr(qend, buf);
			}
		}
	}
	if (i > 0) {
		net_receive_ = i;
	}
	if (net_init_q1_) {
		movelist(net_init_q1_, net_init_q2_, procfunc);
	}
}

void net_init(qinit, qp2)
	Item* qinit, *qp2;
{
	/* qinit=INITIAL { stmtlist qp2=} */
	replacstr(qinit, "\nvoid _net_init(Point_process* _pnt, int _weight_index, double _lflag)");
	vectorize_substitute(insertstr(qinit->next->next, ""), "\n\
   double* _p; Datum* _ppvar; ThreadDatum* _thread; \n\
   Memb_list* _ml; int _cntml_padded, _cntml_actual; int _iml; double* _args;\n\
");
	vectorize_substitute(insertstr(qinit->next->next->next, ""), net_boilerplate(0));
	if (net_init_q1_) {
		diag("NET_RECEIVE block can contain only one INITIAL block", (char*)0);
	}
	net_init_q1_ = qinit;
	net_init_q2_ = qp2;
}

void fornetcon(keyword, par1, args, par2, stmt, qend)
	Item* keyword, *par1, *args, *par2, *stmt, *qend;
{
	Item* q, *q1;
	Symbol* s;
	char snew[100];
	int i;
	/* follows net_receive pretty closely */
	++for_netcons_;
	deltokens(par1, par2);
	i = for_netcons_;
	sprintf(buf, "{int _ifn%d, _nfn%d; double* _fnargs%d, **_fnargslist%d;\n\
\t_nfn%d = _nrn_netcon_args(_ppvar[_fnc_index]._pvoid, &_fnargslist%d);\n\
\tfor (_ifn%d = 0; _ifn%d < _nfn%d; ++_ifn%d) {\n",
	i,i,i,i,i,i,i,i,i,i);
	replacstr(keyword, buf);
	sprintf(buf, "\t _fnargs%d = _fnargslist%d[_ifn%d];\n", i,i,i);
	insertstr(keyword->next, buf);
	insertstr(qend->next, "\t}}\n");
	i = 0;
	ITERATE(q1, args) {
		s = SYM(q1);
		sprintf(snew, "_fnargs%d[%d]", for_netcons_, i);
		++i;
		for (q = stmt; q != qend; q = q->next) {
			if (q->itemtype == SYMBOL && SYM(q) == s) {
				replacstr(q, snew);
			}
		}
	}
}

void chk_thread_safe() {
	Symbol* s;
	int i;
	Item* q;
 	SYMLISTITER { /* globals are now global with respect to C as well as hoc */
		s = SYM(q);
		if (s->nrntype & (NRNGLOBAL) && s->assigned_to_ == 1) {
	sprintf(buf, "Assignment to the GLOBAL variable, \"%s\", is not thread safe", s->name);
			threadsafe(buf);
		}
	}
}


void threadsafe_seen(Item* q1, Item* q2) {
	Item* q;
	assert_threadsafe = 1;
	if (q2) {
		for (q = q1->next; q != q2->next; q = q->next) {
			SYM(q)->assigned_to_ = 2;
		}
	}
}

void conductance_hint(int blocktype, Item* q1, Item* q2) {
	Item* q;
	if (blocktype != BREAKPOINT) {
		diag("CONDUCTANCE can only appear in BREAKPOINT block", (char*)0);
	}
	if (!conductance_) {
		conductance_ = newlist();
	}
	lappendsym(conductance_, SYM(q1->next));
	if (q2 != q1->next) {
		Symbol* s = SYM(q2);
		if (!ion_declared(s)) {
			diag(s->name, " not declared as USEION in NEURON block");
		}
		lappendsym(conductance_, s);
	}else{
		lappendsym(conductance_, SYM0);
	}
	deltokens(q1, q2);
}

void possible_local_current(int blocktype, List* symlist) {
	Item* q; Item* q2;
	if (blocktype != BREAKPOINT) { return; }
	ITERATE(q, currents) {
		ITERATE(q2, symlist) {
			char* n = SYM(q2)->name + 2; /* start after the _l */
			if (strcmp(SYM(q)->name, n) == 0) {
				if (!breakpoint_local_current_) {
					breakpoint_local_current_ = newlist();
				}
				lappendsym(breakpoint_local_current_, SYM(q));
				lappendsym(breakpoint_local_current_, SYM(q2));
			}
		}
	}
}

Symbol* breakpoint_current(Symbol* s) {
	if (breakpoint_local_current_) {
		Item* q;
		ITERATE(q, breakpoint_local_current_) {
			if (SYM(q) == s) {
				return SYM(q->next);
			}
		}
	}
	return s;
}

/* writes item as string to cp and returns end of string */
/* cp must be big enough */
char* item2str(Item* q, char* cp) {
  if (q->itemtype == SYMBOL) {
    sprintf(cp, " %s", SYM(q)->name);
  } else if (q->itemtype == VERBATIM) {
    sprintf(cp, "%s", STR(q));
  } else if (q->itemtype == ITEM) {
    item2str(ITM(q), cp);
  }else {
    sprintf(cp, " %s", STR(q));
  }
  return cp + strlen(cp);
}

char* items_as_string(Item* q1, Item* q2) {
  Item* q;
  char s[8192];
  char* cp = s;
  *cp = '\0';
  for (q = q1; q != q2->next; q = q->next) {
    cp = item2str(q, cp);
  }
  return strdup(s);	
}

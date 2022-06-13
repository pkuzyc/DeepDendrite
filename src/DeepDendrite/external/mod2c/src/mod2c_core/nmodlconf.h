/* nmodlconf.h.  Generated from nmodlconf.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */


#ifndef H_nrnconf_included
#define H_nrnconf_included 1


/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* define if using cygwin */
/* #undef CYGWIN */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* if mac os x */
/* #undef DARWIN */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the `bcopy' function. */
#define HAVE_BCOPY 1

/* Define to 1 if you have the `bzero' function. */
#define HAVE_BZERO 1

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <fenv.h> header file. */
#define HAVE_FENV_H 1

/* Define to 1 if you have the `fesetround' function. */
/* #undef HAVE_FESETROUND */

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `gethostname' function. */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have the `getpw' function. */
#define HAVE_GETPW 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `index' function. */
#define HAVE_INDEX 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isatty' function. */
#define HAVE_ISATTY 1

/* define if using InterViews */
#define HAVE_IV 0

/* Define to 1 if you have the `imf' library (-limf). */
/* #undef HAVE_LIBIMF */

/* Define to 1 if you have the `m' library (-lm). */
/* #undef HAVE_LIBM */

/* Define to 1 if you have the `mass' library (-lmass). */
/* #undef HAVE_LIBMASS */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the `lockf' function. */
#define HAVE_LOCKF 1

/* Define to 1 if you have the `mallinfo' function. */
#define HAVE_MALLINFO 1

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mkdir' function. */
#define HAVE_MKDIR 1

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 1

/* define if the compiler implements namespaces */
#define HAVE_NAMESPACES /**/

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the `posix_memalign' function. */
#define HAVE_POSIX_MEMALIGN 1

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `setenv' function. */
#define HAVE_SETENV 1

/* Define to 1 if you have the `setitimer' function. */
#define HAVE_SETITIMER 1

/* Define to 1 if you have the <sgtty.h> header file. */
#define HAVE_SGTTY_H 1

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* (Define if this signal exists) */
#define HAVE_SIGBUS 1

/* (Define if this signal exists) */
#define HAVE_SIGSEGV 1

/* define if the compiler has stringstream */
#define HAVE_SSTREAM /**/

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <stream.h> header file. */
/* #undef HAVE_STREAM_H */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <stropts.h> header file. */
#define HAVE_STROPTS_H 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `stty' function. */
/* #undef HAVE_STTY */

/* Define to 1 if you have the <sys/conf.h> header file. */
/* #undef HAVE_SYS_CONF_H */

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/file.h> header file. */
#define HAVE_SYS_FILE_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <termio.h> header file. */
#define HAVE_TERMIO_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <varargs.h> header file. */
/* #undef HAVE_VARARGS_H */

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the <_G_config.h> header file. */
#define HAVE__G_CONFIG_H 1

/* define if can declare inline float abs(float) */
#define INLINE_FLOAT_ABS 1

/* define if can declare inline long abs(long) */
#define INLINE_LONG_ABS 1

/* undefined or ::fabs or std::fabs */
#define IVOS_FABS ::fabs

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* define if using mingw */
/* #undef MINGW */

/* define if using Mike Neubig <neubig@salk.edu> contributions */
/* #undef MikeNeubig */

/* where the lib hoc is */
#define NEURON_DATA_DIR "/usr/local/nrn/share/nrn"

/* host triplet */
#define NRNHOST "x86_64-unknown-linux-gnu"

/* cpu type consistent with nrnivmodl */
#define NRNHOSTCPU "x86_64"

/* if 1 then dlopen nrnmech instead of special */
#define NRNMECH_DLL_STYLE 1

/* if nrnoc can use X11 */
#define NRNOC_X11 1

/* Name of package */
#define PACKAGE "nrn"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "nrn"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "nrn 7.4"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "nrn"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "7.4"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* define if RETSIGTYPE(*)(int) is not the prototype for a signal handler */
/* #undef SIGNAL_CAST */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define SUNDIALS data type 'realtype' as 'long double' */
#define SUNDIALS_DOUBLE_PRECISION 1

/* Use generic math functions */
#define SUNDIALS_USE_GENERIC_MATH 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Version number of package */
#define VERSION "7.4"

/* define if no terminal capabilities */
/* #undef WITHOUT_MEMACS */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* define if using Carbon libraries */
/* #undef carbon */

/* Define to the type of a signed integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int64_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */


#if defined(__cplusplus)
#include <ivstream.h>
#endif

#if defined(CYGWIN) || defined(MINGW)
#define WIN32 1
#endif

#if defined(MINGW)
#define CYGWIN
#endif

#if defined(carbon) && defined(__cplusplus)
#define MAC 1
#include <Carbon/Carbon.h>
#endif

#endif /* H_nrnconf_included */


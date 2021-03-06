/* src/oc/nrnmpiuse.h.  Generated from nrnmpiuse.h.in by configure.  */
#ifndef usenrnmpi_h
#define usenrnmpi_h

/* define to 1 if you want MPI specific features activated */
#define NRNMPI 1

/* define to 1 if you want parallel distributed cells (and gap junctions) */
#define PARANEURON 1

/* define to 1 if you want mpi dynamically loaded instead of linked normally */
/* #undef NRNMPI_DYNAMICLOAD */

/* define to 1 if you want the MUSIC - MUlti SImulation Coordinator */
/* #undef NRN_MUSIC */

/* define to the dll path if you want to load automatically */
#define DLL_DEFAULT_FNAME "x86_64/.libs/libnrnmech.so"

/* define if needed */
/* #undef ALWAYS_CALL_MPI_INIT */

/* Number of times to retry a failed open */
/* #undef FILE_OPEN_RETRY */

/* define if IBM BlueGene L, P or Q (activates BGLCheckPoint functionality) */
/* #undef BLUEGENE */

/* define if IBM BlueGene/P */
/* #undef BLUEGENEP */

/* define if IBM BlueGene/Q */
/* #undef BLUEGENEQ */

/* define BlueGene with checkpointing */
/* #undef BLUEGENE_CHECKPOINT */

/* Define bits for BGPDMA & 1 (ISend) & 2 (DMA spike transfer) & 4 (DMA Record Replay */
/* #undef BGPDMA */

/* Define to 1 for possibility of rank 0 xopen/ropen a file and broadcast everywhere */
/* #undef USE_NRNFILEWRAP */

#endif

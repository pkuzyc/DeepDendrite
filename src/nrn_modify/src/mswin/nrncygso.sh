#!/bin/sh

set -x

#mingw='yes'
mingw='no'
nrnpy='yes'
#nrnpy='yes'
#nrnpy='no'
#nrnjvm='yes'
nrnjvm='no'
#nrnmpi='yes'

NPYVER=

#NPYONLY='yes'
NPYONLY='no'

if test mingw = 'yes' ; then
  ltlibs=
else
  ltlibs=
fi

if test "$NPYONLY" = "no" ; then

#all the .o files
find .. -name \*.o -print | sed '
/\/modlunit\//d
/\/nmodl\//d
/\/e_editor\//d
/\/ivoc\/classreg\.o/d
/\/ivoc\/datapath\.o/d
/\/ivoc\/nrnmain\.o/d
/\/ivoc\/ocjump\.o/d
/\/ivoc\/symdir\.o/d
/\/ivoc*\/ivocman1\.o/d
#/\/nrnpython\/.*\.o/d
##/\/nrnmpi\/.*\.o/d
/\/nrnoc\/cprop\.o/d
/\/oc*\/code\.o/d
/\/oc*\/hoc_init\.o/d
/\/oc*\/hoc_oop\.o/d
/\/oc*\/hocusr\.o/d
/\/oc*\/plt\.o/d
/\/oc*\/settext\.o/d
/\/oc*\/spinit\.o/d
/\/oc*\/spinit1\.o/d
/\/oc*\/spinit2\.o/d
/\/memacs*\/.*termio\.o/d
/\/memacs\/main\.o/d
/\/nvkludge\.o/d
/\/nocable\.o/d
/\/nrnnoiv\.o/d
/\/ockludge\.o/d
/\/ocnoiv\.o/d
/\/ocmain\.o/d
/\/inithoc\.o/d
' > temp

mpicc=mpicc

if test "$mpicc" = "mpicc" ; then
  mpich=`which $mpicc | sed "s,/bin/.*,,"`
  echo "mpich=$mpich"

  #mpich made from mpich-3.0.2 and dlls created by
  if false ; then
    ./configure  --prefix=$HOME/mpich2 --with-pm=gforker:mpd \      
      --disable-static --enable-shared --disable-f77 --disable-fc

    make
    make install 

    cd $HOME/mpich2/lib

    mkdir objs
    cd objs
    ar -x ../libpmpich.a
    ar -x ../libmpl.a  
    ar -x ../libmpich.a

    rm lib_libpmpich_la-comm_split_type.o

    cd ..
    gcc -shared -Wl,--out-implib,libmpich.dll.a -o libmpich.dll objs/*.o

  fi
fi

obj=`cat temp`

fi # NPYONLY no

CXX=mpic++

echo IVLIBDIR=\"${IVLIBDIR}\"
echo CFLAGS=\"${CFLAGS}\"
echo LDFLAGS=\"${LDFLAGS}\"

if test "$CFLAGS" != "-mno-cygwin" ; then

  if test "$mingw" = "no" ; then

    mpilib=mpich/lib/libmpich.dll.a
    mpilib=""

    echo 'make nrniv.dll under cygwin'
    $CXX -shared $obj \
      -L${IVLIBDIR} -lIVhines \
      $mpilib \
      -lreadline \
      $LIBS \
      -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
      -lgdi32 -lcomdlg32 \
      -lncurses -lm \
      -o nrniv.dll \
      -Wl,--enable-auto-image-base \
      ${LDFLAGS} \
      -Xlinker --out-implib -Xlinker libnrniv.dll.a

    if test $nrnpy = 'yes' ; then
  
      echo 'make nrnpython27.dll'
      $CXX -shared ../nrnpython/${ltlibs}/*.o -L. -lnrniv -o nrnpython27.dll \
        -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
        -L${IVLIBDIR} -lIVhines \
        -lreadline \
        -Wl,--enable-auto-image-base \
        ${LDFLAGS} \
       -Xlinker --out-implib -Xlinker libnrnpython27.dll.a

      echo 'make hocmodule.dll'
      $CXX -shared \
        ../nrnpython/${ltlibs}/inithoc.o \
        -L. -lnrniv \
        -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
        -o hocmodule.dll \
        -Wl,--enable-auto-image-base \
        ${LDFLAGS} \
       -Xlinker --out-implib -Xlinker libhocmodule.dll.a

      LHOCMODULE='-lhocmodule'
    else  # if test $nrnpy = 'yes'
      LHOCMODULE=''
    fi  # if test $nrnpy = 'yes'

    echo 'make nrniv.exe'
    $CXX -g -O2 -mwindows -o nrniv.exe \
      ../ivoc/nrnmain.o ../oc/modlreg.o \
      -L. -lnrniv \
      $LHOCMODULE \
      -lncurses \
      -L${IVLIBDIR} -lIVhines \
      -lstdc++ -lgdi32 -lcomdlg32 \
      ${LDFLAGS} \
      -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat

  fi # mingw = no

  if test "$mingw" = "yes" ; then

if test "$NPYONLY" = "no" ; then
    echo 'make nrniv.dll for mingw under mingw'
    $CXX -shared $obj \
      -L${IVLIBDIR} -lIVhines \
      $LIBS \
      -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
      -lreadline -ltermcap -lpthread \
      -lgdi32 \
      -o nrniv.dll \
      -Wl,--enable-auto-image-base \
      ${LDFLAGS} \
      -Xlinker --out-implib -Xlinker libnrniv.dll.a

    if test $nrnmpi = 'yes' ; then

      echo 'make libnrnmpi.dll'
      $CXX -shared ../nrnmpi/${ltlibs}/*.o -L. -lnrniv -o libnrnmpi.dll \
	${LIBS} \
        -Wl,--enable-auto-image-base \
        ${LDFLAGS} \
        -Xlinker --out-implib -Xlinker libnrnmpi.dll.a

    fi # if test $nrnmpi = 'yes'

fi # NPYONLY no

    if test $nrnpy = 'yes' ; then

      echo 'make libnrnpython${NPYVER}.dll'
      z=../nrnpython
      lnpyfiles="$z/grids.o $z/nrnpy_hoc.o $z/nrnpy_nrn.o $z/nrnpy_p2h.o $z/nrnpython.o $z/rxd.o $z/rxd_vol.o $z/rxd_extracellular.o"
      $CXX -shared $lnpyfiles -L. -lnrniv -o libnrnpython${NPYVER}.dll \
        -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
        -L${IVLIBDIR} -lIVhines \
        -lreadline -lpthread \
        -Wl,--enable-auto-image-base \
        ${LDFLAGS} \
        -Xlinker --out-implib -Xlinker libnrnpython${NPYVER}.dll.a

      echo 'make librxdmath.dll'
      $CXX -shared ../nrnpython/${ltlibs}/rxdmath.o -o librxdmath.dll \
        -Wl,--enable-auto-image-base \
        ${LDFLAGS} \
        -Xlinker --out-implib -Xlinker librxdmath.dll.a

      echo "make hocmodule${NPYVER}.dll"
      $CXX -shared \
        ../nrnpython/${ltlibs}/inithoc.o \
        -L. -lnrniv -lnrnpython${NPYVER}\
        -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
        -o hocmodule${NPYVER}.dll \
        -Wl,--enable-auto-image-base \
        ${LDFLAGS} \
        -Xlinker --out-implib -Xlinker libhocmodule${NPYVER}.dll.a

      LHOCMODULE="-lhocmodule${NPYVER}"
    else # if test $nrnpy = 'yes'
      LHOCMODULE=''
    fi # if test $nrnpy = 'yes'

if test "$NPYONLY" = "no" ; then
    echo 'make nrniv.exe'

    $CXX -g -O2 -o nrniv.exe \
      ../ivoc/nrnmain.o \
      -L. -lnrniv \
      ${LDFLAGS}

    #without msvcr90.dll and a manifest, cannot 'import numpy' from enthought.
    # also 2.7 problem with matplotlib.pyplot
    #To allow this,  create an alternative executable.
    manifest=Microsoft.VC90.CRT.manifest
    if test -f /e/Python27/$manifest ; then
      cp /e/Python27/$manifest .
      echo '#include "winuser.h"
1 RT_MANIFEST '$manifest'
' > msvcr.rc
      windres --input msvcr.rc --output msvcr.o

      $CXX -g -O2 -o nrniv_crt90.exe \
        ../ivoc/nrnmain.o msvcr.o\
        -L. -lnrniv \
        ${LDFLAGS}

    fi # enthought manifest

fi # NPYONLY no

  fi # mingw = yes

else # if test "$CFLAGS" != "-mno-cygwin"

  $CXX -shared -mno-cygwin $obj \
    $LIBS -lstdc++ \
    -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
    -o nrniv.dll \
    -Wl,--enable-auto-image-base \
    ${LDFLAGS} \
    -Xlinker --out-implib -Xlinker libnrniv.dll.a

  $CXX -shared -mno-cygwin \
    ../nrnpython/${ltlibs}/inithoc.o \
    -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat \
    -L. -lnrniv -lstdc++ \
    -o hocmodule.dll \
    -Wl,--enable-auto-image-base \
    ${LDFLAGS} \
    -Xlinker --out-implib -Xlinker libhocmodule${NPYVER}.dll.a

  $CXX -g -O2 -mno-cygwin -e _mainCRTStartup -o nrniv.exe \
    ../ivoc/nrnmain.o ../oc/modlreg.o \
    -L. -lnrniv -lstdc++ \
    ${LDFLAGS} \
    -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lexpat                     -L/usr/lib -lz  -lexpat

fi # if test "$CFLAGS" != "-mno-cygwin"

#mv nrniv.exe c:/nrn61/bin
#cd ..
#mv hocmodule.dll c:/nrn61/bin

#!/bin/sh

prefix=/raid/zyc/code_for_dhs_paper/install
exec_prefix=/raid/zyc/code_for_dhs_paper/install/x86_64
NRNBIN=${exec_prefix}/bin
ARCH=x86_64
MODSUBDIR=x86_64
NEURONHOME=/raid/zyc/code_for_dhs_paper/install/share/nrn

cd $1

if [ -x ${MODSUBDIR}/special ] ; then
	program="./${MODSUBDIR}/special"
else
	program="${NRNBIN}/nrniv"
fi

hostname
pwd
shift
shift
echo "time $program $*"
time $program $*


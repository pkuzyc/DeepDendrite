# run with
# python setup.py build_ext --inplace

import sys
nrn_srcdir = "."
#mingw = 1
mingw = 0
instdir = "/raid/zyc/code_for_dhs_paper/install"
if nrn_srcdir[0] != '/' :
    # TODO: is this right?
    nrn_srcdir = '../../../../../../' + nrn_srcdir

from distutils.core import setup
from distutils.extension import Extension

def have_vc():
    if not mingw:
        return False
    try:
        from distutils import spawn
        x = spawn.find_executable("cl")
        x = True if x is not None and "Microsoft" in x else False
    except:
        x = False
    return x

try:
    import numpy
except:
    setup()
else:
    use_vc = have_vc()
    if use_vc:
        mpicc_bin = 'cl'
        mpicxx_bin = 'cl'
    else:
        mpicc_bin = "mpicc"
        mpicxx_bin = "mpic++"
    print (mpicxx_bin)
    import os
    os.environ["CC"]=mpicc_bin
    os.environ["CXX"]=mpicxx_bin

    define_macros=[]
    if mingw and sys.version_info[0] == 2:
        define_macros=[('MS_WIN64', None)]

    include_dirs = [nrn_srcdir + '/share/lib/python/neuron/rxd/geometry3d', '.', numpy.get_include()]

    srcdir = nrn_srcdir + '/share/lib/python/neuron/rxd/geometry3d/'

    #    name='neuron/rxd/geometry3d',
    setup(
        ext_modules = [
                       Extension("graphicsPrimitives",
                                 sources=["graphicsPrimitives.cpp"],
                                 define_macros=define_macros,
                                 include_dirs=include_dirs),
                       Extension("ctng",
                                 sources=["ctng.cpp"],
                                 define_macros=define_macros,
                                 include_dirs=include_dirs),
                       Extension("surfaces",
                                 sources=["surfaces.cpp", "marching_cubes2.c", "llgramarea.c"],
                                 define_macros=define_macros,
                                 include_dirs=include_dirs)],
    )
    #    package_dir = {'': instdir + '/share/lib/python/neuron/rxd/geometry3d'}


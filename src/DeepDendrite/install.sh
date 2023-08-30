rm -rf build
mkdir build
cd build
# module purge
module load /path/to/your/nvhpc/modulefile/modulefiles
# for older versions of pgi, load the moduleflie as following
## module purge
# module load /path/to/your/pgi/modulefiles/pgi/19.10
# module load /path/to/your/pgi/modulefiles/openmpi/3.1.3/2019
export CC=mpicc
export CXX=mpicxx
cmake .. -DCMAKE_C_FLAGS:STRING="-lrt -g -O0 -mp -mno-abm" -DCMAKE_CXX_FLAGS:STRING="-lrt -std=c++11 -g -O0 -mp -mno-abm" -DCOMPILE_LIBRARY_TYPE=STATIC -DCMAKE_INSTALL_PREFIX="../../../install" -DADDITIONAL_MECHPATH="../../all_mechanisms" -DCUDA_HOST_COMPILER=`which gcc` -DCUDA_PROPAGATE_HOST_FLAGS=OFF -DENABLE_SELECTIVE_GPU_PROFILING=ON -DENABLE_OPENACC=ON -DAUTO_TEST_WITH_SLURM=OFF -DAUTO_TEST_WITH_MPIEXEC=OFF -DFUNCTIONAL_TESTS=OFF -DUNIT_TESTS=OFF
make -j24
make install

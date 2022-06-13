# MOD2C
> NMODL to C converter for CoreNEURON

[![Build Status](https://travis-ci.org/BlueBrain/mod2c.svg?branch=master)](https://travis-ci.org/BlueBrain/mod2c)

MOD2C is NMODL to C adapted for [CoreNEURON simulator](https://github.com/BlueBrain/CoreNEURON).
More information about NMODL can be found [here](https://www.neuron.yale.edu/neuron/static/docs/refman/nocmodl.html).

This version supports GPU code generation using OpenACC. Use latest
version of CoreNeuron.

# Requirements
* [CMake 2.8.9+](https://cmake.org/)
* [Flex](http://flex.sourceforge.net)
* [Bison](https://www.gnu.org/software/bison/)

# Installation

Once you clone the repository, you can build mod2c using CMake as follows:

```bash
cd mod2c
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install/directory
make
make install
```

Note that mod2c is used only during the compilation phase of the CoreNEURON simulator
and hence should be built only for the front-end/login nodes in cross-compile environments
like BG-Q, Cray, Intel MIC etc.

## License
* See LICENSE.txt
* See [NEURON](https://www.neuron.yale.edu/neuron/)
* The mechanisms and test datasets appearing in the CoreNeuron repository are subject to separate licenses.
  More information is available on the NMC portal website [NMC portal](https://bbp.epfl.ch/nmc-portal/copyright),
  the specific licenses are described in the ME-type model packages downloadable from that website.

## Contributors
To facilitate the future distributions of the software the Blue Brain Project wishes to remain the sole
owner of the copyright. Therefore we will ask contributors to not modify the existing copyright.
Contributors will however be gratefully acknowledged in the corresponding CREDIT.txt file.

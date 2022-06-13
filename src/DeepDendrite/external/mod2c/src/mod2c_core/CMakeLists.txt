# Copyright (c) 2016, Blue Brain Project
# All rights reserved.

# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.


set(NOCMODL_CORE_SOURCES
"consist.c"
"io.c"       "list.c"
"nocpout.c"  "partial.c"
"solve.c"    "version.c"
"discrete.c"
"kinetic.c"  "modl.c"
"parsact.c"  "sens.c"
"symbol.c"   "deriv.c"
"init.c"
"noccout.c"
"simultan.c" "units.c"
)

bison_target(diffeq diffeq.y ${CMAKE_CURRENT_BINARY_DIR}/diffeq.c)
bison_target(parse1 parse1.y ${CMAKE_CURRENT_BINARY_DIR}/parse1.c)

flex_target(lex lex.l ${CMAKE_CURRENT_BINARY_DIR}/lex.c)
add_flex_bison_dependency(lex parse1)

add_definitions(-DNMODL=1 -DBBCORE=1 -DNOCMODL=1 -DCVODE=1 -DVECTORIZE=1)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(mod2c_core
    ${NOCMODL_CORE_SOURCES}
    ${BISON_diffeq_OUTPUTS} ${BISON_parse1_OUTPUTS}
    ${FLEX_lex_OUTPUTS})

install(TARGETS mod2c_core DESTINATION bin)


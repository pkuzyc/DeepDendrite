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


include(TestScriptUtils)

set(TEST_LABEL validation)

add_test_class(validation)

set(MOD2CFILTER "perl ${CMAKE_CURRENT_SOURCE_DIR}/filter_mod2c_output.pl")
configure_file("test_mod2c.in" ${CMAKE_CURRENT_BINARY_DIR}/test_mod2c @ONLY)

set(MOD2CFILTER "perl ${CMAKE_CURRENT_SOURCE_DIR}/filter_mod2c_output.pl -DNRNBBCORE=1")
configure_file("test_mod2c.in" ${CMAKE_CURRENT_BINARY_DIR}/test_mod2c_core @ONLY)

set(TEST_ENVIRONMENT "MODLUNIT=${CMAKE_SOURCE_DIR}/share/nrnunits.lib")

foreach(mod2c "mod2c_core")
    set(test_data "${CMAKE_CURRENT_SOURCE_DIR}/${mod2c}")
    file(GLOB test_mods RELATIVE "${test_data}/mod" "${test_data}/mod/*.mod")

    foreach(modfile ${test_mods})
	set(mod ${modfile})
	string(REGEX REPLACE "\\.mod$" "" mod "${modfile}")

	set(TEST_ARGS ${CMAKE_BINARY_DIR}/src/${mod2c}/${mod2c} ${test_data}/mod/${mod}.mod ${test_data}/cpp/${mod}.cpp)
	add_test_script("${mod2c}-${mod}" "${CMAKE_CURRENT_BINARY_DIR}/test_${mod2c}" "/bin/bash")
    endforeach()
endforeach()


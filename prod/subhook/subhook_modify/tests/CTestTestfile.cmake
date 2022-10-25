# CMake generated Testfile for 
# Source directory: /home/qwerty/github/subhook/tests
# Build directory: /home/qwerty/github/subhook/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(subhook_test_exe_test "/home/qwerty/github/subhook/tests/test")
set_tests_properties(subhook_test_exe_test PROPERTIES  PASS_REGULAR_EXPRESSION "Testing initial install
foo_hooked\\(\\) called
foo\\(\\) called
Testing re-install
foo_hooked\\(\\) called
foo\\(\\) called
Testing trampoline
foo_hooked_tr\\(\\) called
foo\\(\\) called
")
add_test(subhook_cxx_test_exe_test "/home/qwerty/github/subhook/tests/test++")
set_tests_properties(subhook_cxx_test_exe_test PROPERTIES  PASS_REGULAR_EXPRESSION "Testing initial install
foo_hooked\\(\\) called
foo\\(\\) called
Testing re-install
foo_hooked\\(\\) called
foo\\(\\) called
Testing trampoline
foo_hooked_tr\\(\\) called
foo\\(\\) called
")

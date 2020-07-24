#
# Copyright 2014 by Philip N. Garner
#
# See the file COPYING for the licence associated with this software.
#
# Author(s):
#   Phil Garner, December 2014
#

include(LubeTest)
exe_diff_test(
  CMD ./test-lapack
  REF ${TEST_DIR}/test-lapack-ref.txt
  OUT test-lapack-out.txt
  )

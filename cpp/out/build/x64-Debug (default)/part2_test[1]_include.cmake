if(EXISTS "C:/Users/berkd/source/repos/google-code-sample/cpp/out/build/x64-Debug (default)/part2_test[1]_tests.cmake")
  include("C:/Users/berkd/source/repos/google-code-sample/cpp/out/build/x64-Debug (default)/part2_test[1]_tests.cmake")
else()
  add_test(part2_test_NOT_BUILT part2_test_NOT_BUILT)
endif()

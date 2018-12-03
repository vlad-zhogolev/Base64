include_directories(../libs/googletest/include ../../src)

file(MAKE_DIRECTORY ../../cmake-build-debug/tests/base64_tests/encoded)
file(MAKE_DIRECTORY ../../cmake-build-debug/tests/base64_tests/decoded)

add_executable(runBase64Tests Base64Tests.cpp ../../src/Base64.cpp)

target_link_libraries(runBase64Tests gtest gtest_main)
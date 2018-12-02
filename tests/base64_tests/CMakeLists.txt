include_directories(../libs/googletest/include ../../src)

add_executable(runBase64Tests Base64Tests.cpp ../../src/Base64.cpp)

target_link_libraries(runBase64Tests gtest gtest_main)
# may need rename base64 (same as in main CMakeLists.txt)
project(base64)
set(SOURCES main.cpp Base64.h Base64.cpp)
add_executable(base64 ${SOURCES})
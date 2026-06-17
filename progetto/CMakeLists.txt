cmake_minimum_required(VERSION 3.20)
project(progetto)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

add_executable(test_grafi_progetto test_grafi_progetto.cpp)
add_test(NAME test_grafi COMMAND test_grafi_progetto)

add_executable(test_grad_coniugato gradienteconiugatotest.cpp)
add_test(NAME test_grad_coniugato COMMAND test_grad_coniugato)


add_executable(mainprogetto mainprogetto.cpp)

enable_testing()
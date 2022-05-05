cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(dual-readout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(rootIO)
add_subdirectory(CBDsim)
add_subdirectory(Reco)
add_subdirectory(analysis)

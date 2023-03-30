# Require a recent version of cmake
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# HPX install dir, change as per install location
set(HPX_INCLUDE_DIR /usr/local/include/)
set(HPX_LIBRARY_DIR /home/arnav/)

# This project is C++ based.
project(mat_mul
        VERSION 1
        DESCRIPTION "Parallelized matrix multiplication using HPX."
        LANGUAGES CXX)


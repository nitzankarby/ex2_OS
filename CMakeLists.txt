cmake_minimum_required(VERSION 3.21)
project(ex2_yoav_dan)

set(CMAKE_CXX_STANDARD 11)

add_library(ex2_yoav_dan
        uthreads.cpp
        uthreads.h
        demo_itimer.c
        demo_jmp.c
        demo_singInt_handler.c
        )

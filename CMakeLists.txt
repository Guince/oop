cmake_minimum_required(VERSION 3.9)
project(oop)

set(CMAKE_CXX_STANDARD 14)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic" )
add_executable(polina_oop main.cpp Base.h Shapes.h Shape_factory.h Container.h)
cmake_minimum_required(VERSION 3.8)
project(FastNoiseLite)

add_library(FastNoiseLite INTERFACE include/FastNoiseLite/FastNoiseLite.h)
target_include_directories(FastNoiseLite INTERFACE include)

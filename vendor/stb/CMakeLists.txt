cmake_minimum_required(VERSION 3.8)
project(stb)

add_library(stb STATIC include/stb/stb_image.h src/stb_image.cc)
target_include_directories(stb PUBLIC include)

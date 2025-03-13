cmake_minimum_required(VERSION 3.8)
project(stb)

add_library(stb INTERFACE include/stb/stb_image.h src/stb_image.cc)
target_include_directories(glad PUBLIC include)

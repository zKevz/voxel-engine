cmake_minimum_required(VERSION 3.8)
project(imgui)

add_library(imgui STATIC 
    include/imgui/imconfig.h
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_internal.h
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui.cpp
    include/imgui/imgui.h
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
    
    include/imgui/misc/cpp/imgui_stdlib.cpp
    include/imgui/misc/cpp/imgui_stdlib.h
    include/imgui/misc/fonts/binary_to_compressed_c.cpp

    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_glfw.h
    include/imgui/backends/imgui_impl_opengl3_loader.h
    include/imgui/backends/imgui_impl_opengl3.cpp
    include/imgui/backends/imgui_impl_opengl3.h
)
target_include_directories(imgui PUBLIC include include/imgui)
target_link_libraries(imgui PRIVATE glfw)

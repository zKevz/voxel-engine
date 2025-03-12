cmake_minimum_required(VERSION 3.8)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME}
    main.cc
)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} glfw GLEW::GLEW)
target_include_directories(${PROJECT_NAME} PUBLIC .)

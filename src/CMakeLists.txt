cmake_minimum_required(VERSION 3.8)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_executable(${PROJECT_NAME}
    main.cc

    Application.cc

    OpenGL/IndexBuffer.cc
    OpenGL/Shader.cc
    OpenGL/VertexArrayObject.cc
    OpenGL/VertexBufferObject.cc

    Utils/Utils.cc
    Utils/Logger.cc
)
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARY}
    glfw
    glad
    glm::glm
    stb
    spdlog::spdlog
    fmt::fmt
)
target_include_directories(${PROJECT_NAME} PUBLIC .)

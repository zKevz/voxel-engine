cmake_minimum_required(VERSION 3.8)

add_executable(${PROJECT_NAME}
    main.cc

    Application.cc
    Block.cc
    BlockSpecification.cc
    Chunk.cc
    Light.cc
    Renderer.cc
    ThreadPool.cc
    World.cc

    OpenGL/Camera3D.cc
    OpenGL/IndexBuffer.cc
    OpenGL/Shader.cc
    OpenGL/Texture.cc
    OpenGL/TextureCubeMap.cc
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
    FastNoiseLite
    imgui
)
target_include_directories(${PROJECT_NAME} PUBLIC .)

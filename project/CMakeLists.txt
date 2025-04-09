cmake_minimum_required(VERSION 3.10)

project(OpenGLViewer)

add_executable(OpenGLViewer
    main.cpp
    Cupe.cpp
    Camera.cpp
    Model.cpp
)

target_include_directories(OpenGLViewer PRIVATE
    ${CMAKE_SOURCE_DIR}/imported/tinygltf
)

target_link_libraries(OpenGLViewer PRIVATE
    glad
    glfw
    glm::glm
    imgui
    tinygltf
)

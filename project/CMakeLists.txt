# CMakeList.txt : CMake project for computergraphics, include source and define
# project specific logic here.
#

# Add source to this project's executable.
project(project)
add_executable(project)

target_sources(project
    PRIVATE

    main.cpp
 "Cupe.cpp" "Cube.h" "Camera.cpp" "Camera.h" "Model.h" "Model.cpp")
target_link_libraries(project
    PRIVATE
    glad
    glfw
    glm::glm
    imgui
)
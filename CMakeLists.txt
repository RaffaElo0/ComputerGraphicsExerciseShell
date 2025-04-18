cmake_minimum_required(VERSION 3.6...3.14 FATAL_ERROR)
cmake_policy(VERSION 3.6...3.14)

project(computergraphics)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
  add_compile_options("/MP" "/Zi")
else()
  add_compile_options("-lglfw3" "-lGL" "-lX11" "-lpthread" "-lXrandr" "-lXi" "-ldl" "-g")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY "." PROPERTY VS_STARTUP_PROJECT project)

# ——— GLM — entweder per FetchContent … ——
include(FetchContent)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)
FetchContent_MakeAvailable(glm)

# ——— Include für Header‑Only Libraries ——
include_directories(
  ${CMAKE_SOURCE_DIR}/imported/tinygltf/include
  ${CMAKE_SOURCE_DIR}/imported/nlohmannjson/include
  ${CMAKE_SOURCE_DIR}/imported/stb            # stb_image.h
)

# ——— Fremdbibliotheken als Subprojekte ——
add_subdirectory(imported/glad)
add_subdirectory(imported/glfw)
add_subdirectory(imported/imgui)

# Assimp‑Build‑Optionen
set(ASSIMP_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT     ON  CACHE BOOL "" FORCE)
add_subdirectory(imported/assimp)

# eigenes Projekt
add_subdirectory(project)

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.6...3.14 FATAL_ERROR)
cmake_policy(VERSION 3.6...3.14)

project ("computergraphics")

if(MSVC)
  add_compile_options("/MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZI")
else()
  add_compile_options("-lglfw3")
  add_compile_options("-lGL")
  add_compile_options("-lX11")
  add_compile_options("-lpthread")
  add_compile_options("-lXrandr")
  add_compile_options("-lXi")
  add_compile_options("-ldl")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(DIRECTORY "." PROPERTY VS_STARTUP_PROJECT project)

include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# Include sub-projects.
include_directories(${CMAKE_SOURCE_DIR}/imported/tinygltf/include)
include_directories(${CMAKE_SOURCE_DIR}/imported/nlohmannjson/include)
include_directories(${CMAKE_SOURCE_DIR}/imported/stbi/include)

add_subdirectory(imported)
add_subdirectory(project)

cmake_minimum_required(VERSION 3.10)
project(OpenGLViewer)

# Quellen
add_executable(OpenGLViewer
    main.cpp
    Camera.cpp      
    Shader.cpp      
    Model.cpp
    Mesh.cpp
)

# Kopiere den gesamten assets-Ordner in das Ausgabeverzeichnis
add_custom_command(TARGET OpenGLViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:OpenGLViewer>/assets")

# Include‑Verzeichnisse
target_include_directories(OpenGLViewer PRIVATE
    ${CMAKE_SOURCE_DIR}/imported/glad/include
    ${CMAKE_SOURCE_DIR}/imported/glfw/include
    ${CMAKE_SOURCE_DIR}/imported/imgui        
    ${CMAKE_SOURCE_DIR}/imported/stb          
    ${CMAKE_SOURCE_DIR}/imported/assimp/include
    # glm kommt als FetchContent und liefert den Target-Namen glm::glm:
)

# Linker‑Bibliotheken
target_link_libraries(OpenGLViewer PRIVATE
    glad
    glfw
    imgui
    assimp              
    glm::glm            

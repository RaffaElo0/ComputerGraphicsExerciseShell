#version 330 core

// === Vertex Shader ===
// Transforms vertex positions to clip space and passes along normals and texture coordinates

// Per-vertex attributes (from VBO)
layout (location = 0) in vec3 aPos;      // Vertex position (object space)
layout (location = 1) in vec3 aNormal;   // Vertex normal (object space)
layout (location = 2) in vec2 aTexCoords;// Vertex texture coordinates

// Uniform matrices set per-frame
uniform mat4 model;      // Model matrix: object -> world space
uniform mat4 view;       // View matrix: world -> camera space
uniform mat4 projection; // Projection matrix: camera -> clip space

// Outputs to fragment shader
out vec2 TexCoords;      // Pass texture coordinates
out vec3 FragPos;        // Position in world space
out vec3 Normal;         // Normal in world space

void main() {
    // Compute world-space position of the vertex
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normal with inverse-transpose of model matrix to handle non-uniform scaling correctly
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass through texture coordinates
    TexCoords = aTexCoords;

    // Final vertex position in clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
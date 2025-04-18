#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

/// Model: loads a 3D model using Assimp and stores its meshes and textures for rendering.
class Model {
public:
    /// Constructs the model by loading it from the given file path
    Model(const std::string& path);
    
    /**
     * Draws the model by binding necessary textures and VAOs
     * @param shaderProgram OpenGL shader program ID
     * @param modelMatrix  Model transformation matrix
     */
    void draw(unsigned int shaderProgram, const glm::mat4& modelMatrix);

private:
    /// Represents a single vertex with position, normal, and texture coordinates
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    /// Holds OpenGL texture ID, its type (e.g. "texture_diffuse"), and file path
    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    /// Encapsulates an OpenGL mesh: VAO/VBO/EBO, index count, and associated textures
    struct Mesh {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int indexCount;
        std::vector<Texture> textures;
    };

    std::vector<Mesh> meshes;                ///< All meshes in this model
    std::vector<Texture> texturesLoaded;     ///< Loaded textures to avoid duplication
    std::string directory;                   ///< Directory of the model file, for relative texture paths

    /// Loads the model file and initializes meshes
    void loadModel(const std::string& path);
    /// Recursively processes each node in the Assimp scene graph
    void processNode(aiNode* node, const aiScene* scene);
    /// Converts an Assimp mesh into our Mesh format
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    /// Retrieves textures of a specific type from a material
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
    /// Utility to load a texture from file using stb_image
    unsigned int TextureFromFile(const char* path, const std::string& directory);
};

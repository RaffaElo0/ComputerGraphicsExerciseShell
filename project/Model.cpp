#include "Model.h"
#include <assimp/Importer.hpp>   // C++ importer interface
#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags
#include "stb_image.h"           // Image loading library
#include <iostream>

// Constructor: loads model data from file
Model::Model(const std::string& path) {
    loadModel(path);
}

// Draws the model: sets uniforms, binds textures, and renders each mesh
void Model::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    // Activate the shader program
    glUseProgram(shaderProgram);

    // Upload the transformation matrix to the 'model' uniform
    GLint locModel = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, &modelMatrix[0][0]);

    // Counters for naming multiple diffuse/specular textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    // Iterate over all meshes in this model
    for (const auto& mesh : meshes) {
        // Bind each texture for the current mesh
        for (unsigned int i = 0; i < mesh.textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);  // Activate proper texture unit
            const std::string& name = mesh.textures[i].type;
            std::string number;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);

            // Set the sampler to the correct texture unit
            glUniform1i(
                glGetUniformLocation(shaderProgram, (name + number).c_str()),
                i
            );
            // Bind the texture by its OpenGL ID
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
        }

        // Render the mesh
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Reset active texture to default unit
        glActiveTexture(GL_TEXTURE0);
    }
}

// Loads a model using Assimp and stores the directory for texture loading
void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    // Read file with post-processing steps:
    // - Triangulate meshes, flip UVs, and calculate tangent space
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs    |
        aiProcess_CalcTangentSpace
    );

    // Check for loading errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    // Extract directory path for texture loading
    directory = path.substr(0, path.find_last_of("/\\"));

    // Recursively process the root node and its children
    processNode(scene->mRootNode, scene);
}

// Processes an Assimp node: loads all its meshes and processes children
void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process each mesh referenced by this node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(ai_mesh, scene));
    }
    // Recursively process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

// Converts an Assimp mesh to our Mesh format, including vertex data and textures
Model::Mesh Model::processMesh(aiMesh* meshA, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Iterate over each vertex
    for (unsigned int i = 0; i < meshA->mNumVertices; ++i) {
        Vertex vertex;
        // Position data
        vertex.Position = {
            meshA->mVertices[i].x,
            meshA->mVertices[i].y,
            meshA->mVertices[i].z
        };
        // Normal data
        vertex.Normal = {
            meshA->mNormals[i].x,
            meshA->mNormals[i].y,
            meshA->mNormals[i].z
        };
        // Texture coordinates (if present)
        if (meshA->mTextureCoords[0]) {
            vertex.TexCoords = {
                meshA->mTextureCoords[0][i].x,
                meshA->mTextureCoords[0][i].y
            };
        } else {
            vertex.TexCoords = {0.0f, 0.0f};
        }
        vertices.push_back(vertex);
    }

    // Build index list from faces
    for (unsigned int i = 0; i < meshA->mNumFaces; ++i) {
        aiFace face = meshA->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // Load material textures (diffuse and specular)
    if (meshA->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[meshA->mMaterialIndex];
        auto diffuseMaps  = loadMaterialTextures(material, aiTextureType_DIFFUSE,  "texture_diffuse");
        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), diffuseMaps.begin(),  diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Generate OpenGL buffers/arrays for this mesh
    Mesh myMesh;
    myMesh.indexCount = static_cast<unsigned int>(indices.size());
    myMesh.textures   = textures;

    glGenVertexArrays(1, &myMesh.VAO);
    glGenBuffers(1, &myMesh.VBO);
    glGenBuffers(1, &myMesh.EBO);

    glBindVertexArray(myMesh.VAO);

    // Upload vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, myMesh.VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // Upload index data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );

    // Setup vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
    return myMesh;
}

// Loads textures from material and returns Texture structures
std::vector<Model::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (const auto& loaded : texturesLoaded) {
            if (loaded.path == str.C_Str()) {
                textures.push_back(loaded);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id   = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

// Utility function to load a texture image from disk using stb_image
unsigned int Model::TextureFromFile(const char* path, const std::string& directory) {
    std::string filename = directory + "/" + std::string(path);
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = (nrComponents == 1 ? GL_RED : (nrComponents == 3 ? GL_RGB : GL_RGBA));
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

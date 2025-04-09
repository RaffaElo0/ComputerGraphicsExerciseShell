// Model.cpp (GLTF-Version)
#include "Model.h"
#include <tiny_gltf.h>
#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltfModel;
    std::string err, warn;

    bool ret = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, path);

    if (!warn.empty()) std::cout << "[TinyGLTF warning] " << warn << std::endl;
    if (!err.empty()) std::cerr << "[TinyGLTF error] " << err << std::endl;
    if (!ret) {
        std::cerr << "[TinyGLTF] Failed to load: " << path << std::endl;
        return;
    }

    for (const auto& mesh : gltfModel.meshes) {
        for (const auto& primitive : mesh.primitives) {
            if (primitive.attributes.find("POSITION") == primitive.attributes.end()) continue;

            const auto& posAccessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
            const auto& posBufferView = gltfModel.bufferViews[posAccessor.bufferView];
            const auto& posBuffer = gltfModel.buffers[posBufferView.buffer];

            const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset]);
            size_t posCount = posAccessor.count;

            const float* normals = nullptr;
            size_t normalCount = 0;
            if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                const auto& normAccessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
                const auto& normBufferView = gltfModel.bufferViews[normAccessor.bufferView];
                const auto& normBuffer = gltfModel.buffers[normBufferView.buffer];
                normals = reinterpret_cast<const float*>(&normBuffer.data[normBufferView.byteOffset + normAccessor.byteOffset]);
                normalCount = normAccessor.count;
            }

            std::vector<float> vertexData;
            for (size_t i = 0; i < posCount; ++i) {
                vertexData.push_back(positions[3 * i + 0]);
                vertexData.push_back(positions[3 * i + 1]);
                vertexData.push_back(positions[3 * i + 2]);

                if (normals && i < normalCount) {
                    vertexData.push_back(normals[3 * i + 0]);
                    vertexData.push_back(normals[3 * i + 1]);
                    vertexData.push_back(normals[3 * i + 2]);
                }
                else {
                    vertexData.insert(vertexData.end(), { 0.0f, 0.0f, 1.0f });
                }
            }

            std::vector<unsigned int> indices;
            if (primitive.indices >= 0) {
                const auto& idxAccessor = gltfModel.accessors[primitive.indices];
                const auto& idxBufferView = gltfModel.bufferViews[idxAccessor.bufferView];
                const auto& idxBuffer = gltfModel.buffers[idxBufferView.buffer];

                const unsigned short* idxData = reinterpret_cast<const unsigned short*>(&idxBuffer.data[idxBufferView.byteOffset + idxAccessor.byteOffset]);
                for (size_t i = 0; i < idxAccessor.count; ++i) {
                    indices.push_back(static_cast<unsigned int>(idxData[i]));
                }
            }
            else {
                for (unsigned int i = 0; i < posCount; ++i) {
                    indices.push_back(i);
                }
            }

            Mesh mesh = {};
            mesh.indexCount = static_cast<unsigned int>(indices.size());

            glGenVertexArrays(1, &mesh.VAO);
            glGenBuffers(1, &mesh.VBO);
            glGenBuffers(1, &mesh.EBO);

            glBindVertexArray(mesh.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
            glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

            meshes.push_back(mesh);
        }
    }
}

void Model::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 1.0f, 1.0f);

    for (const auto& mesh : meshes) {
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
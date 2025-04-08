// Model.h
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

class Model {
public:
    Model(const std::string& path);
    void draw(unsigned int shaderProgram, const glm::mat4& modelMatrix);

private:
    struct Mesh {
        unsigned int VAO, VBO, EBO;
        unsigned int indexCount;
    };

    std::vector<Mesh> meshes;
    void loadModel(const std::string& path);
};

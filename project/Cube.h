#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    void init();
    void render(unsigned int shaderProgram);
    void setPosition(const glm::vec3& pos);
    void setColor(const glm::vec3& col);

    const char* vertexShaderSource;
    const char* fragmentShaderSource;

private:
    unsigned int VAO, VBO, EBO;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);
};

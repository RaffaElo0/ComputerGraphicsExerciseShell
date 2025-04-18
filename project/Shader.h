#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;

    // Konstruktor kompiliert Vertex- & Fragment‑Shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // Shader aktivieren
    void use() const;
    // Uniform‑Setter
    void setBool(const std::string &name, bool value) const;
    void setInt (const std::string &name, int  value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4 (const std::string &name, const glm::mat4 &mat) const;
    void setVec3 (const std::string &name, const glm::vec3 &vec) const;
private:
    void checkCompileErrors(GLuint shader, const std::string& type);
};

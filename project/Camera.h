// Camera.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(GLFWwindow* window);

    void update();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

    void reset();

private:
    GLFWwindow* window;

    float distance = 5.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 400.0f, lastY = 300.0f;
    bool firstMouse = true;
    bool leftMousePressed = false;

    glm::vec3 position;
    glm::vec3 target = glm::vec3(0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    void processMouseInput();
    void processScrollInput();
};

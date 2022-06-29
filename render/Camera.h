//
// Created by bendi on 2022. 06. 28..
//

#ifndef MC_CLONE_CAMERA_H
#define MC_CLONE_CAMERA_H


#include "glm/ext/matrix_float4x4.hpp"
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../config.h"

class Camera {
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;

    glm::vec3 m_Position;
    glm::vec3 m_Forward;
    glm::vec3 m_Right;
    glm::vec3 m_Up;

    float m_VerticalAngle;
    float m_HorizontalAngle;

    float m_Mouse_sensitivity;
    float m_Movement_speed;

    GLFWwindow* m_Window;

public:
    Camera() = default;

    Camera(float fovy, float aspect, float near, float far, float mouse_sensitivity, float movement_speed, GLFWwindow* window);

    void Update(float delta);

    glm::mat4 GetMatrix();
};


#endif //MC_CLONE_CAMERA_H

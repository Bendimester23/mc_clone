//
// Created by bendi on 2022. 06. 28..
//

#include "Camera.h"
#include "spdlog/spdlog.h"

Camera::Camera(float fovy, float aspect, float near, float far, float mouse_sensitivity, float movement_speed, GLFWwindow* window) {

    this->m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
    this->m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
    this->m_Up = glm::cross(this->m_Forward, this->m_Right);

    this->m_VerticalAngle = 0.0f;
    this->m_HorizontalAngle = 180.0f;

    this->m_Mouse_sensitivity = mouse_sensitivity;
    this->m_Movement_speed = movement_speed;

    this->m_Window = window;

    this->ToggleCursorState();

    this->m_ProjectionMatrix = glm::perspective(
            glm::radians(fovy),
            aspect,
            near,
            far
            );

    this->m_ViewMatrix = glm::lookAt(
            glm::vec3(m_Position),
            glm::vec3(m_Position + m_Forward),
            glm::vec3(m_Up)
            );
}

glm::mat4 Camera::GetMatrix() {
    return this->m_ProjectionMatrix * this->m_ViewMatrix;
}

void Camera::Update(float delta) {
    if (this->m_CursorGrabbed) {
        double xpos, ypos;
        glfwGetCursorPos(this->m_Window, &xpos, &ypos);

        this->m_HorizontalAngle -= ((float) xpos - WINDOW_WIDTH / 2.0f) * delta * this->m_Mouse_sensitivity;
        if (this->m_HorizontalAngle > 360) this->m_HorizontalAngle -= 360;
        if (this->m_HorizontalAngle < 0) this->m_HorizontalAngle += 360;
        this->m_VerticalAngle -= ((float) ypos - WINDOW_HEIGHT / 2.0f) * delta * this->m_Mouse_sensitivity;
        this->m_VerticalAngle = glm::clamp(this->m_VerticalAngle, -90.0f, 90.0f);

        glfwSetCursorPos(this->m_Window, 640, 360);

        float angleH = glm::radians(this->m_VerticalAngle);
        float angleV = glm::radians(this->m_HorizontalAngle);

        this->m_Forward = glm::vec3(
                std::cos(angleH) * std::sin(angleV),
                std::sin(angleH),
                std::cos(angleH) * std::cos(angleV)
        );

        this->m_Right = glm::vec3(
                std::sin(angleV - 3.14f / 2.0f),
                0,
                std::cos(angleV - 3.14f / 2.0f)
        );

        this->m_Up = glm::cross(this->m_Right, this->m_Forward);
    }

    if (glfwGetKey(this->m_Window, GLFW_KEY_W ) == GLFW_PRESS){
        this->m_Position += this->m_Forward * delta * this->m_Movement_speed;
    }

    if (glfwGetKey(this->m_Window, GLFW_KEY_S ) == GLFW_PRESS){
        this->m_Position -= this->m_Forward * delta * this->m_Movement_speed;
    }

    if (glfwGetKey(this->m_Window, GLFW_KEY_D ) == GLFW_PRESS){
        this->m_Position += this->m_Right * delta * this->m_Movement_speed;
    }

    if (glfwGetKey(this->m_Window, GLFW_KEY_A ) == GLFW_PRESS){
        this->m_Position -= this->m_Right * delta * this->m_Movement_speed;
    }

    this->m_ViewMatrix = glm::lookAt(
            this->m_Position,
            this->m_Position+this->m_Forward,
            this->m_Up
    );

    //spdlog::info("Camera Pos: X:{} Y:{} Z:{}", this->m_Position.x, this->m_Position.y, this->m_Position.z);
}

Camera::Camera() {
    this->m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
    this->m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
    this->m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
    this->m_Up = glm::cross(this->m_Forward, this->m_Right);

    this->m_VerticalAngle = 0.0f;
    this->m_HorizontalAngle = 180.0f;

    this->m_Mouse_sensitivity = 10;
    this->m_Movement_speed = 20;

    this->m_ProjectionMatrix = glm::perspective(
            glm::radians(60.0f),
            1280.0f/720.0f,
            0.01f,
            1000.0f
    );

    this->m_ViewMatrix = glm::lookAt(
            glm::vec3(m_Position),
            glm::vec3(m_Position + m_Forward),
            glm::vec3(m_Up)
    );
}

void Camera::SetPosition(glm::vec3 pos) {
    this->m_Position = pos;
    //Don't accept input
    this->Update(0.0f);
}

void Camera::ToggleCursorState() {
    spdlog::info("Cursor grab toggled.");
    this->m_CursorGrabbed = !this->m_CursorGrabbed;

    if (this->m_CursorGrabbed) {
        glfwSetInputMode(this->m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    } else {
        glfwSetInputMode(this->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

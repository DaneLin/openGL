#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler angles
    float Yaw;
    float Pitch;
    // camera options;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                                                                                                 MovementSpeed(SPEED),
                                                                                                                                                 MouseSensitivity(SENSITIVITY),
                                                                                                                                                 Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the lookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
        //return calculate_lookAt_matrix(Position, Position + Front, Up);
    }

    // glm::mat4 myLookAt(glm::vec3 camPos, glm::vec3 lookDir, glm::vec3 upDir)
    // {
    //     glm::vec3 camRight = glm::normalize(glm::cross(lookDir, upDir));
    //     glm::vec3 camUp = glm::normalize(glm::cross(camRight, lookDir));
    //     glm::mat4 view;
    //     view[0][0] =
    //     glm::mat4 trans;
    //     trans[3][0] = -camPos.x;
    //     trans[3][1] = -camPos.y;
    //     trans[3][2] = -camPos.z;
    //     return view * trans;
    // }

    // Custom implementation of the LookAt function
    glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
    {
        // 计算方向法线，即z轴
        glm::vec3 zaxis = glm::normalize(position - target);
        // 计算x轴
        glm::vec3 xaxis = glm::normalize(glm::cross(worldUp, zaxis));
        // 计算y轴，通过叉乘得到
        glm::vec3 yaxis = glm::cross(zaxis, xaxis);

        glm::mat4 trans;
        glm::mat4 view;
        //in glm we access elements as mat[col][row] due to column-major layout
        //glm中是列向量
        for (int i = 0; i < 3; i++)
        {
            trans[3][i] = -position[i];
            view[i][0] = xaxis[i];
            view[i][1] = yaxis[i];
            view[i][2] = zaxis[i];
        }

        return view * trans;
    }

    // processes input received from any keyboard-like input system
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += glm::vec3(Front.x, 0, Front.z) * velocity;
        if (direction == BACKWARD)
            Position -= glm::vec3(Front.x, 0, Front.z) * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
            {
                Pitch = 89.0f;
            }
            if (Pitch < -89.0f)
            {
                Pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.f)
        {
            Zoom = 1.f;
        }
        if (Zoom > 45.0f)
        {
            Zoom = 45.0f;
        }
    }

private:
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = cos(glm::radians(PITCH)) * sin(glm::radians(Yaw));
        Front = glm::normalize(front);
        // re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
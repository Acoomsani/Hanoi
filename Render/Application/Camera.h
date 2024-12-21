// @brief: 相机封装类（基于欧拉角实现）

#include "General.h"

#pragma once

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera();

    Camera(Camera &&) = default;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    glm::mat4 getViewMatrix()const;

    void refresh();

    void keyInput(Camera_Movement move, float time);

    void mouseCall(float x, float y, bool constrain);

private:
    static constexpr float yaw_{-89.0f};
    static constexpr float pitch_{0.0f};
    static constexpr glm::vec3 right_{1.0f, 0.0f, 0.0f};
    static constexpr glm::vec3 front_{0.0f, 0.0f, 0.0f};
    static constexpr glm::vec3 position_{0.0f, 2.5f, 8.0f};

    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{right_};
    glm::vec3 front{front_};
    glm::vec3 position{position_};

    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    float yaw{yaw_};
    float zoom{45.0f};
    float speed{7.5f};
    float pitch{pitch_};
    float sensitivity{0.08f};

    void updateCameraVectors();
};


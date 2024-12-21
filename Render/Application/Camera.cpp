#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : position(position), up(up), yaw(yaw),
                                                                           pitch(pitch) {
      updateCameraVectors();
}

void Camera::updateCameraVectors() {
      // calculate the new Front vector
      glm::vec3 front_(1.0f);
      front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      front_.y = sin(glm::radians(pitch));
      front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      front = glm::normalize(front_);
      // also re-calculate the Right and Up vector
      up = glm::normalize(glm::cross(right, front));
      right = glm::normalize(glm::cross(front, worldUp));
}

glm::mat4 Camera::getViewMatrix() const {
      return glm::lookAt(position, position + front, up);
}

void Camera::keyInput(Camera_Movement move, float time) {
      float cameraSpeed = speed * time; // adjust accordingly
      if (move == FORWARD) {
            float y = position.y;
            position += cameraSpeed * front;
            position.y = y;
      }
      if (move == BACKWARD) {
            float y = position.y;
            position -= cameraSpeed * front;
            position.y = y;
      }
      if (move == LEFT)
            position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
      if (move == RIGHT)
            position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
      if (move == UP)
            position += glm::vec3(0, 1, 0) * cameraSpeed;
      if (move == DOWN)
            position -= glm::vec3(0, 1, 0) * cameraSpeed;
}

void Camera::mouseCall(float x, float y, bool constrain) {
      yaw += x * sensitivity;
      pitch += y * sensitivity;
      // make sure that when pitch is out of bounds, screen doesn't get flipped
      if (constrain) {
            if (pitch > 89.0f)
                  pitch = 89.0f;
            if (pitch < -89.0f)
                  pitch = -89.0f;
      }
      updateCameraVectors();
}

Camera::Camera() {
      updateCameraVectors();
}

void Camera::refresh() {
      yaw = yaw_;
      pitch = pitch_;
      right = right_;
      front = front_;
      position = position_;
      updateCameraVectors();
}


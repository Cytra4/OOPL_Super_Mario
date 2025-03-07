#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
private:
    glm::vec3 Position;
    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;

public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
    ~Camera();
    
    glm::mat4 GetViewMatrix();
};

#endif
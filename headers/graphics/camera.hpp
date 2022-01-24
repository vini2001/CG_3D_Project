#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "shaderClass.hpp"

typedef glm::vec3 v3;
typedef glm::vec4 v4;
typedef glm::mat4 m4;

class Camera {
    public:
    v3 position;
    v3 orientation = v3(0.0f, 0.0f, 1.0f);
    v3 up = v3(0.0f, 1.0f, 0.0f);
    m4 cameraMatrix = m4(1.0f);


    int width;
    int height;

    float speed = 0.1f;
    float sensitivity = 100.0f;

    bool firstClick = true;

    Camera(int width, int height, v3 position);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void matrix(Shader *shader, const char* uniform);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane, Shader* shader, const char* uniform);
    void inputs(GLFWwindow* window);

    void lookAt(v3 target);
};

#endif
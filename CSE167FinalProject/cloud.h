#ifndef _CLOUD_H_
#define _CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Object.h"

class Cloud
{
private:
    glm::mat4 model;
    GLuint vao;
    GLuint vbo;
public:
    Cloud();
    ~Cloud();

    void draw(GLuint shaderProgram, glm::mat4 View, glm::mat4 Projection);
};

#endif


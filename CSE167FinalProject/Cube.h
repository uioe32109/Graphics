#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Object.h"

class Cube
{
private:
    glm::mat4 model;
	GLuint vao;
	GLuint vbos[2];
public:
    float length;
    glm::vec3 cube_max;
    glm::vec3 cube_color;
	Cube(float size, glm::vec3 min, glm::vec3 max);
	~Cube();

	void draw(GLuint shaderProgram, glm::mat4 View, glm::mat4 Projection);
	void update(int num);
	void spin(float deg);
    void move(int direction);
    void moveFPV(glm::vec3 direction);
};

#endif


#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Object
{
protected:
	glm::mat4 model;
	glm::vec3 color;
public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }

	virtual void draw() = 0;
	virtual void update(int num) = 0;
    virtual void scale(float degree) = 0;
    virtual void translateXY(glm::vec3 prev, glm::vec3 after) = 0;
    virtual void translateZ(float distance) = 0;
    virtual void rotateMouse(glm::vec3 prev, glm::vec3 after) = 0;
    virtual void translateX(float distance) = 0;
    virtual void translate(glm::vec3 direction) = 0;
};

#endif


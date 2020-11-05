#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> vec_normal;
    std::vector<glm::uvec3> tri_array;
    float x_mid,y_mid,z_mid;
    float x_dif,y_dif,z_dif;
    float x_max=0,x_min=0,y_max=0,y_min=0,z_max=0,z_min=0;
	GLuint vao, vbo;
    GLuint vbo2;
    GLuint ebo;
	GLfloat pointSize;
    int direction;
public:
	PointCloud(std::string objFilename, GLfloat pointSize,glm::vec3 color_setting);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spinz(float deg);
    void spiny(float deg);
    void setDirection(int d);
    std::vector<glm::vec3> getPoints(){
        return points;
    }
    std::vector<glm::vec3> getVec_normal(){
        return vec_normal;
    }
    glm::vec3 getCenterTransformation(){
        return glm::vec3(-x_mid,-y_mid,-z_mid);
    }
    glm::vec3 getScaleTrnasformation(){
        float x_scale = 2/x_dif;
        float y_scale = 2/y_dif;
        float z_scale = 2/z_dif;
        if(x_scale>y_scale){
            if(x_scale>z_scale){
                return glm::vec3(x_scale,x_scale,x_scale);
            }
            else{
                return glm::vec3(z_scale,z_scale,z_scale);
            }
        }
        else{
            if(y_scale>z_scale){
                return glm::vec3(y_scale,y_scale,y_scale);
            }
            else{
                return glm::vec3(z_scale,z_scale,z_scale);
            }
        }
    }
    GLfloat getPointsize(){
        return pointSize;
    }
};

#endif


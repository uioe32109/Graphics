#ifndef _REC_H_
#define _REC_H_

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
#include <iostream>
#include <string>
#include "stb_image.h"
#include "Object.h"

class Rec
{
private:
    glm::mat4 model;
    GLuint vao;
    GLuint vbos[2];
public:
    //Rec(float size, glm::vec3 min, glm::vec3 max);
    float rec_length;
    float rec_height;
    float rec_width;
    unsigned int textureID;
    glm::vec3 rec_max;
    glm::vec3 rec_color;
    std::vector <std::string> textureName;
    Rec(glm::vec3 start_point,float length,float height,float width,std::vector<std::string> texture_list);
    ~Rec();
    void draw(GLuint shaderProgram, glm::vec3 Color, bool check_collision, bool collision_detected, glm::mat4 View, glm::mat4 Projection);
    void loadTexture(std::vector<std::string> faces);
};

#endif

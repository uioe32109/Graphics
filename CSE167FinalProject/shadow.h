#ifndef shadow_h
#define shadow_h
#ifdef __APPLE__

#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <list>
#include <stdio.h>

class Shadow {
public:
    
    unsigned int depthMapFBO;
    unsigned int depthMap;
    
    Shadow();

    GLuint uLightSpaceMatrix;
    GLuint depthMatrixID;
    
    void renderDepth( GLuint shaderProgram );
    
    glm::mat4 depthProjection, depthView;
    glm::mat4 depthSpaceMatrix;
};

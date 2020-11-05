//
//  Node.h
//  CSE 167 FInal Project
//
//  Created by Yilin Cai on 3/12/20.
//  Copyright © 2020 曾啸洋. All rights reserved.
//

#ifndef Node_h
#define Node_h

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

class Node
{
protected:
    int identifier = 0;
public:
    int getIdentifier(){return identifier;}
    void setIdentifier(int i){identifier = i;}
    virtual void draw(glm::mat4 C) = 0;
    virtual void update() = 0;
};

#endif /* Node_h */

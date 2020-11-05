//
//  geometry.cpp
//  CSE 167 HW 0
//
//  Created by 曾啸洋 on 2020/2/21.
//  Copyright © 2020 曾啸洋. All rights reserved.
//

#include "geometry.h"
#include <fstream>
#include <sstream>
#include "stb_image.h"
using namespace std;
Geometry::Geometry(std::string objFilename)
{
    // points and points normal
        points = {};
        normal = {};
        faces = {};
        
     // read obj file by the giving obj name
           ifstream inFile;
           string line;
           inFile.open(objFilename);
           
           // If the file is not open, exit with code 1
           if(!inFile)
           {
               exit(1);
           }
           
           // read the vertex line by line
           else
           {
               while (getline(inFile, line)) {
                   // vertex
                   if(line.substr(0,2) == "v ")
                   {
                       istringstream iss(line.substr(2));
                       double x, y, z, r, g, b;
                       iss >> x;
                       iss >> y;
                       iss >> z;
                       iss >> r;
                       iss >> g;
                       iss >> b;
                       glm::vec3 oneLinePoints = glm::vec3(x,y,z);
                       points.push_back(oneLinePoints);
                  }
                   
                   // vertex normalize
                   else if(line.substr(0,2) == "vn")
                   {
                       istringstream iss(line.substr(3));
                       double x,y,z;
                       iss >> x;
                       iss >> y;
                       iss >> z;
                       glm::vec3 oneLinePoints = glm::vec3(x,y,z);
                       normal.push_back(oneLinePoints);
                   }
                   
                   // read the f value for triangle meshing
                   else if(line.substr(0,2) == "f ")
                   {
                       istringstream iss(line.substr(2));
                       unsigned int x, y, z;
                       char slash;
                       int unusedCoord;
                       iss >> x;
                       iss >> slash >> slash >> unusedCoord;
                       iss >> y;
                       iss >> slash >> slash >> unusedCoord;
                       iss >> z;
                       iss >> slash >> slash >> unusedCoord;
                       
                       // 0-base
                       x--;
                       y--;
                       z--;
                       glm::ivec3 oneFace = glm::ivec3(x,y,z);
                       faces.push_back(oneFace);
                   }
               }
           }
    
    inFile.close();

    /*
     * TODO: Center the model by taking the midpoint of difference between max and min.
     */
     // Set the model matrix to an identity matrix.
    model = glm::mat4(1);
    
    // find the max and min of x, y and z
    max = points[0];
    min = points[0];
    
    for(int i = 0; i < points.size(); i++)
    {
        // update max and min of x
        if (points[i].x > max.x)
        {
            max.x = points[i].x;
        }
        
        else if (points[i].x < min.x)
        {
            min.x = points[i].x;
        }
        
        // update max and min of y
        if (points[i].y > max.y)
        {
            max.y = points[i].y;
        }
        
        else if (points[i].y < min.y)
        {
            min.y = points[i].y;
        }
        
        // update max and min of z
        if (points[i].z > max.z)
        {
            max.z = points[i].z;
        }
        
        else if (points[i].z < min.z)
        {
            min.z = points[i].z;
        }
    }
    
    // find the mid point between max and min coordinates.
    midPoint = 0.5f * (max + min);
    
    // translate model to the mid point
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-midPoint.x,-midPoint.y,-midPoint.z)) * model;
    
    // scale the model in a 2 * 2 * 2 cube
    // check the max difference between 2 and (max - min)
    float diff_x = (max.x - min.x)/2.0f;
    float diff_y = (max.y - min.y)/2.0f;
    float diff_z = (max.z - min.z)/2.0f;
    float scale = 0;
    
    // choose scale
    if(diff_x >= diff_y)
    {
        if(diff_x >= diff_z)
        {
            scale = 1.0f/diff_x;
        }
        else
        {
            scale = 1.0f/diff_z;
        }
    }
    
    else if(diff_y > diff_x)
    {
        if(diff_y >= diff_z)
        {
            scale = 1.0f/diff_y;
        }
        else
        {
            scale = 1.0f/diff_z;
        }
    }
    
    // scale up/down
    model = glm::scale(glm::mat4(1.0f), glm::vec3(scale,scale,scale)) * model;
    
    // scale up the ball a little bit
    model = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f,3.0f,3.0f)) * model;
    
    // scale up the max and min after scale the model up
    max = glm::vec3(3.0, 3.0, 3.0) * max;
    min = glm::vec3(3.0, 3.0, 3.0) * min;
    
    /* triangle mesh and color */
    // Set the color.

    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo[0]);
    glGenBuffers(1, &ebo);

    // Bind to the VAO.
    // This tells OpenGL which data it should be paying attention to
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    
    // new buffer
    glGenBuffers(1, &vbo[1]);
    
    // bind to array buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    
    // pass in the data
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normal.size(), normal.data(), GL_STATIC_DRAW);
    
    // enable the next attribute array
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // pass in the face value to a new vbo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);

    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
    // load texture
    loadTexture(textureName);
}

Geometry::~Geometry()
{
    glDeleteBuffers(1, &vbo[0]);
    glDeleteBuffers(1, &vbo[1]);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C, glm::mat4 View, glm::mat4 Projection)
{
    // choose skybox shader
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "Sphere"), 1);
    //glDepthFunc(GL_LEQUAL);

    // get location of all matrices
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    GLuint debugLoc = glGetUniformLocation(shaderProgram, "debugColor");
    // ... set model, view, projection matrix
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(debugLoc, 0);
    // face culling
    // glFrontFace(GL_CW);

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    //glDepthFunc(GL_LESS);
}

void Geometry::update(glm::mat4 C)
{
}

void Geometry::move(int direction)
{
    // forward
    if(direction == 0)
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f)) * model;
    
        // update midpoint
        midPoint = midPoint + glm::vec3(0.0f,0.0f,-1.0f);
    }
    // backward
    else if(direction == 1)
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,1.0f)) * model;
        // update midpoint
        midPoint = midPoint + glm::vec3(0.0f,0.0f,1.0f);
    }
    
    // left
    else if(direction == 2)
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)) * model;
        // update midpoint
        midPoint = midPoint + glm::vec3(-1.0f,0.0f,0.0f);
    }
    
    // right
    else
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f)) * model;
        
        // update midpoint
        midPoint = midPoint + glm::vec3(1.0f,0.0f,0.0f);
    }
}

// move to a certain direction
void Geometry::moveFPV(glm::vec3 direction)
{
    // update model
    model = glm::translate(glm::mat4(1.0f), direction) * model;
    
    // update midpoint
    midPoint = midPoint + direction;
}

// load texture from jpeg file
void Geometry::loadTexture(std::vector<std::string> faces)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//
//  Rec.cpp
//  CSE 167 FInal Project
//
//  Created by Yilin Cai on 3/10/20.
//  Copyright © 2020 曾啸洋. All rights reserved.
//

#include "Rec.h"
#include <stdio.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif


Rec::Rec(glm::vec3 start_point,float length,float height,float width,std::vector<std::string> texture_list)
{
    // Model matrix. Since the original size of the cube is 2, in order to
    // have a cube of some size, we need to scale the cube by size / 2.
    model = glm::mat4(1.0f);

    // The color of the cube. Try setting it to something else!
    // color = glm::vec3(0.1f, 0.95f, 0.1f);

    /*
     * Cube indices used below.
     *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
     *
     */
    // remember the length, width and height
    rec_width = width;
    rec_height = height;
    rec_length = length;
    rec_max = start_point + glm::vec3(length,height,0);
    
    // rec vertices information
    std::vector<glm::vec3> vertices
    {
        start_point,
        start_point+glm::vec3(0,height,0),
        start_point+glm::vec3(length,height,0),
        start_point+glm::vec3(length,0,0),
        start_point+glm::vec3(0,0,-width),
        start_point+glm::vec3(0,height,-width),
        start_point+glm::vec3(length,height,-width),
        start_point+glm::vec3(length,0,-width),
    };

    // Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
    // and v3 in counter-clockwise order.
    std::vector<glm::ivec3> indices
    {
        // Front face.
        glm::ivec3(0, 1, 2),
        glm::ivec3(2, 3, 0),
        // Back face.
        glm::ivec3(7, 6, 5),
        glm::ivec3(5, 4, 7),
        // Right face.
        glm::ivec3(3, 2, 6),
        glm::ivec3(6, 7, 3),
        // Left face.
        glm::ivec3(4, 5, 1),
        glm::ivec3(1, 0, 4),
        // Top face.
        glm::ivec3(4, 0, 3),
        glm::ivec3(3, 7, 4),
        // Bottom face.
        glm::ivec3(1, 5, 6),
        glm::ivec3(6, 2, 1),
    };

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        vertices.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access vertices through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO. We will use it to store the indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
        indices.data(), GL_STATIC_DRAW);

    // Unbind from the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    loadTexture(texture_list);
}

Rec::~Rec()
{
    // Delete the VBOs and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void Rec::draw(GLuint shaderProgram,glm::vec3 Color, bool check_collision, bool collision_detected, glm::mat4 View, glm::mat4 Projection)
{
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "Sphere"), 1);
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    GLuint debugLoc = glGetUniformLocation(shaderProgram, "debugColor");
    
    // ... set model, view, projection matrix
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(Color));
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    // Draw triangles using the indices in the second VBO, which is an
    // element array buffer.
    // rendering line segment when detecting collision
    if (check_collision)
    {
        if(collision_detected)
        {
            glUniform1i(debugLoc, 1);
            glDrawElements(GL_LINE_STRIP , 36, GL_UNSIGNED_INT, 0);

        }
        else
        {
            glUniform1i(debugLoc, 0);
            glDrawElements(GL_LINE_STRIP , 36, GL_UNSIGNED_INT, 0);
        }
    }
    else
    {
        glUniform1i(debugLoc, 0);
        glDrawElements(GL_TRIANGLES , 36, GL_UNSIGNED_INT, 0);
    }
    // Unbind from the VAO.
    glBindVertexArray(0);
}

// load the texture
void Rec::loadTexture(std::vector<std::string> faces)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}




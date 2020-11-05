//
//  skybox.cpp
//  CSE 167 HW 3
//
//  Created by 曾啸洋 on 2020/2/20.
//  Copyright © 2020 曾啸洋. All rights reserved.
//
#include <stdio.h>
#include "skybox.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

skybox::skybox(float size, glm::mat4 View, glm::mat4 Projection)
{
    // Model matrix. Since the original size of the cube is 2, in order to
    // have a cube of some size, we need to scale the cube by size / 2.
    model = glm::scale(glm::vec3(size / 2.f));
    
    // assign model and view to local variable
    view = View;
    projection = Projection;
    // get the hard coded names
    //textureName;

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

     // The 8 vertices of a cube.
    std::vector<glm::vec3> vertices
    {
        glm::vec3(-500, 500, 500),
        glm::vec3(-500, -500, 500),
        glm::vec3(500, -500, 500),
        glm::vec3(500, 500, 500),
        glm::vec3(-500, 500, -500),
        glm::vec3(-500, -500, -500),
        glm::vec3(500, -500, -500),
        glm::vec3(500, 500, -500)
    };
//       std::vector<glm::vec3> vertices
//        {
//            glm::vec3(1, 1, 1),
//            glm::vec3(-1, -1, 1),
//            glm::vec3(1, -1, 1),
//            glm::vec3(1, 1, 1),
//            glm::vec3(-1, 1, -1),
//            glm::vec3(-1, -1, -1),
//            glm::vec3(1, -1, -1),
//            glm::vec3(1, 1, -1)
//        };

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
    
    // get the textureID information
    loadTexture(textureName);
    
}

skybox::~skybox()
{
    // Delete the VBOs and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void skybox::draw(GLuint glProgram, glm::mat4 View)
{
    
    // setup cloud texture first
    //cloud -> draw(glProgram);
    
    // choose skybox shader
    glUseProgram(glProgram);
    glUniform1i(glGetUniformLocation(glProgram, "Skybox"), 0);
    
    // set the sampler for cloud texture
   // glUniform1i(glGetUniformLocation(glProgram, "Cloud"), 1);
    glDepthFunc(GL_LEQUAL);
    
    // get location of all matrices
    GLuint projectionLoc = glGetUniformLocation(glProgram, "projection");
    GLuint viewLoc = glGetUniformLocation(glProgram, "view");
    //GLuint modelLoc = glGetUniformLocation(glProgram, "model");
    
    // ... set view and projection matrix
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // face culling
    glFrontFace(GL_CW);

    glBindVertexArray(vao);
    
    // bind to skybox texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    // bind to cloud texture
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, cloud->textureID);
    
    // draw the element
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
    // Unbind from the VAO.
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

// load the texture
void skybox::loadTexture(std::vector<std::string> faces)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
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
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

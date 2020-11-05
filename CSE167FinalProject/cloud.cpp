#include "cloud.h"

Cloud::Cloud()
{

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Enable vertex attribute 0.
    // We will be able to access vertices through it.
    // Load Attribute Pointers


    // Unbind from the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}



void Cloud::draw(GLuint shaderProgram, glm::mat4 View, glm::mat4 Projection)
{
    glUseProgram(shaderProgram);
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "P");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "V");
    GLuint timeLoc = glGetUniformLocation(shaderProgram, "time");
    // ... set model, view, projection matrix
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
    glUniform1f(timeLoc, (float)glfwGetTime() * 0.5f);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Unbind from the VAO.
    glBindVertexArray(0);
}

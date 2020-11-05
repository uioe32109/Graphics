#include "Cube.h"

Cube::Cube(float size, glm::vec3 min, glm::vec3 max)
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
    // difference between the max point and min point
    glm::vec3 diff = max - min;
    
    // remenber the length of the cube (x, y, z )
    length = diff.x;
    
    // remember the max
    cube_max = max;
    
    // assign the vertices
    std::vector<glm::vec3> vertices
    {
        glm::vec3(max.x - diff.x, max.y, max.z),
        glm::vec3(max.x - diff.x, max.y - diff.y, max.z),
        glm::vec3(max.x, max.y - diff.y, max.z),
        glm::vec3(max.x, max.y, max.z),
        glm::vec3(max.x - diff.x, max.y, max.z - diff.z),
        glm::vec3(max.x - diff.x, max.y - diff.y, max.z - diff.z),
        glm::vec3(max.x, max.y - diff.y, max.z - diff.z),
        glm::vec3(max.x, max.y, max.z - diff.z)
    };
    
	 // The 8 vertices of a cube.
//	std::vector<glm::vec3> vertices
//	{
//		glm::vec3(-1, 1, 1),
//		glm::vec3(-1, -1, 1),
//		glm::vec3(1, -1, 1),
//		glm::vec3(1, 1, 1),
//		glm::vec3(-1, 1, -1),
//		glm::vec3(-1, -1, -1),
//		glm::vec3(1, -1, -1),
//		glm::vec3(1, 1, -1)
//	};

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
}

Cube::~Cube()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Cube::draw(GLuint shaderProgram, glm::mat4 View, glm::mat4 Projection)
{
    glUseProgram(shaderProgram);
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    // ... set model, view, projection matrix
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(cube_color));
    // Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glDrawElements(GL_LINE_STRIP, 36, GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Cube::update(int num)
{
	// Spin the cube by 1 degree.
//	spin(0.1f);
}

void Cube::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
//	model = glm::rotate(glm::mat4(1), glm::radians(deg),
//		glm::vec3(0.0f, 1.0f, 0.0f)) * model;
}

void Cube::move(int direction)
{
    // forward
    if(direction == 0)
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-1.0f)) * model;
        
        // update max
        cube_max = cube_max + glm::vec3(0.0f,0.0f,-1.0f);
    }
    
    // backward
    else if(direction == 1)
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,1.0f)) * model;
        
        // update max
        cube_max = cube_max + glm::vec3(0.0f,0.0f,1.0f);
    }
    
    // left
    else if(direction == 2)
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f,0.0f,0.0f)) * model;
        
        // update max
        cube_max = cube_max + glm::vec3(-1.0f,0.0f,0.0f);
    }
    
    // right
    else
    {
        // update model
        model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f,0.0f,0.0f)) * model;
        
        // update max
        cube_max = cube_max + glm::vec3(1.0f,0.0f,0.0f);
    }
    
}

// move to a certain direction
void Cube::moveFPV(glm::vec3 direction)
{
    // update model
    model = glm::translate(glm::mat4(1.0f), direction) * model;
    
    // update midpoint
    cube_max = cube_max + direction;
}

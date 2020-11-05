#include "PointCloud.h"
#include <glm/gtx/string_cast.hpp>
using namespace std;

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize,glm::vec3 color_setting)
	: pointSize(pointSize)
{
    FILE* fp;
    int c1,c2,c3;
    float x, y ,z;
    float xn, yn, zn;
    unsigned int xt,yt,zt;
    unsigned int t1,t2,t3;
    float r,g,b;
    fp = std::fopen(objFilename.c_str(),"rb");
    if (fp==NULL) {
        cerr<< "error loading file" << endl; exit(-1);
    }
    while(feof(fp)!=EOF){
        c1 = fgetc(fp);
        if(c1==EOF){
            break;
        }
        c2 = fgetc(fp);
        if(c2==EOF){
            break;
        }
        if (c1=='v' && c2== ' '){
            fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
            if(x>x_max){
                x_max = x;
            }
            else if(x<x_min){
                x_min = x;
            }
            else{}
            if(y>y_max){
                y_max = y;
            }
            else if(y<y_min){
                y_min = y;
            }
            else{}
            if(z>z_max){
                z_max = z;
            }
            else if(z<z_min){
                z_min = z;
            }
            else{}
            points.push_back(glm::vec3(x,y,z));
        }
        else if (c1=='v' && c2 =='n'){
            fscanf(fp, "%f %f %f", &xn, &yn, &zn);
            vec_normal.push_back(glm::vec3(xn,yn,zn));
        }
        else if (c1=='f' && c2 ==' '){
            //printf("Loading faces\n");
            fscanf(fp,"%d//%d %d//%d %d//%d\r",&xt,&t1,&yt,&t2,&zt,&t3);
            tri_array.push_back(glm::uvec3(xt-1,yt-1,zt-1));
        }
        else if (c2 == 'f' && fgetc(fp)==' '){
            fscanf(fp,"%d//%d %d//%d %d//%d\r",&xt,&t1,&yt,&t2,&zt,&t3);
            tri_array.push_back(glm::uvec3(xt-1,yt-1,zt-1));
        }
        else{
            //cout<<char(c1)<<' '<<c2<<endl;
        }
    }
    fclose(fp);
    x_dif = x_max-x_min;
    y_dif = y_max-y_min;
    z_dif = z_max-z_min;
    x_mid = (x_dif)/2+x_min;
    y_mid = (y_dif)/2+y_min;
    z_mid = (z_dif)/2+z_min;
    
    

	 // Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
    color = color_setting;
    
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	/*glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);*/
    
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * tri_array.size(), tri_array.data(), GL_STATIC_DRAW);
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
	/*
	 * TODO: Section 2 and 3. 	 
	 * Following the above example but for vertex normals, 
	 * 1) Generate a new vertex bufferbuffer,
	 * 2) Bind it as a GL_ARRAY_BUFFER type, 
	 * 3) Pass in the data 
	 * 4) Enable the next attribute array (which is 1)
	 * 5) Tell it how to process each vertex using glVertexAttribPointer
	 */
    
    glGenBuffers(1, &vbo2);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vec_normal.size(), vec_normal.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &ebo);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	//glPointSize(pointSize);
	// Draw points 
	//glDrawArrays(GL_POINTS, 0, points.size());
    glDrawElements(GL_TRIANGLES, tri_array.size()*3, GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
    if(scale_pointsize == 2){
        updatePointSize(1);
        scale_pointsize = 0;
    }
    else if(scale_pointsize == 1){
        updatePointSize(-1);
        scale_pointsize = 0;
    }
    else{}
    
    /*if(direction==0){
        spinz(0.1f);
    }
    else{
        spiny(0.1f);
    }*/
}

void PointCloud::updatePointSize(GLfloat size)
{
    pointSize = pointSize + size;
}

void PointCloud::spinz(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
    degree = degree + deg;
    if(degree>=360){
        degree = degree - 360;
    }
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
}

void PointCloud::spiny(float deg)
{
    // Update the model matrix by multiplying a rotation matrix
    degree = degree + deg;
    if(degree>=360){
        degree = degree - 360;
    }
    model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::setDirection(int d)
{
    direction=d;
}


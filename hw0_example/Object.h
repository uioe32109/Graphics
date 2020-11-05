#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Object
{
protected:
    float x_mov = 0;
    float y_mov = 0;
    float z_mov = 0;
	glm::mat4 model;
	glm::vec3 color;
    int is_translated = 0;
    float degree = 0;
    glm::mat4 origin_copy;
    glm::mat4 update_copy;
    int scale_pointsize = 0;
    std::vector<int> scale_history;
    std::vector<float> degree_history;
    std::vector<glm::vec3> rotate_history;
    glm::vec3 up_scale = glm::vec3(1.5,1.5,1.5);
    glm::vec3 down_scale = glm::vec3(0.5,0.5,0.5);
public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
    glm::mat4 getOrigin() {return origin_copy;}
    void setModel(glm::mat4 input){ model = input;}
    void saveOrigin() {origin_copy = model;update_copy=model;}
    glm::mat4 goRight() {
        if(is_translated==0){
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            x_mov+=2;
            is_translated=1;
        }
        return model;
    }
    glm::mat4 goLeft() {
        if(is_translated==0){
            model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
            x_mov-=2;
            is_translated = 1;
        }
        return model;
    }
    
    void wup(glm::vec3 r_matrix){
        y_mov+=1;
        model = glm::rotate(model, glm::radians(-degree), r_matrix);
        model = glm::translate(model, glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(degree), r_matrix);
    }
    
    void sdown(glm::vec3 r_matrix){
        y_mov-=1;
        model = glm::rotate(model, glm::radians(-degree), r_matrix);
        model = glm::translate(model, glm::vec3(0,-1,0));
        model = glm::rotate(model, glm::radians(degree), r_matrix);
    }
    
    void dright(glm::vec3 r_matrix){
        x_mov+=1;
        model = glm::rotate(model, glm::radians(-degree), r_matrix);
        model = glm::translate(model, glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(degree), r_matrix);
    }
    
    void aleft(glm::vec3 r_matrix){
        x_mov-=1;
        model = glm::rotate(model, glm::radians(-degree), r_matrix);
        model = glm::translate(model, glm::vec3(-1,0,0));
        model = glm::rotate(model, glm::radians(degree), r_matrix);
    }
    void reset_position(glm::vec3 r_matrix, glm::vec3 t_matrix){
        model = origin_copy;
        model = glm::translate(model, t_matrix);
        for(float i = 0; i<scale_history.size();i++){
            if (scale_history[i]==0){
                model = glm::scale(model,up_scale);
            }
            else{
                model = glm::scale(model,down_scale);
            }
        }
        model = glm::rotate(model,glm::radians(degree),r_matrix);
        if(t_matrix[0]>0){
            x_mov = 2;
        }
        else{
            x_mov = -2;
        }
        y_mov = 0;
    }
    void reset_shape(){
        model = origin_copy;
        model = glm::translate(model,glm::vec3(x_mov,y_mov,0));
        degree = 0;
        scale_history.clear();
    }
    void increase_point_size(){
        scale_pointsize = 2;
    }
    void decrease_point_size(){
        scale_pointsize = 1;
    }
    
    void scale_up(){
        model = glm::scale(model,up_scale);
        scale_history.push_back(0);
    }
    
    void scale_down(){
        model = glm::scale(model,down_scale);
        scale_history.push_back(1);
    }
    
    void xyTranslate(double x, double y){
        float scale_x = x*0.05;
        float scale_y = -y*0.05;
        int i;
        //model = glm::translate(model,glm::vec3(scale_x,scale_y,0));
        update_copy = glm::translate(update_copy,glm::vec3(scale_x,scale_y,0));
        model = update_copy;
        for(i = 0; i<scale_history.size();i++){
            if (scale_history[i]==0){
                model = glm::scale(model,up_scale);
            }
            else{
                model = glm::scale(model,down_scale);
            }
        }
        for(i = 0;i<degree_history.size();i++){
            model = glm::rotate(model,glm::radians(degree_history[i]),rotate_history[i]);
            //glm::mat4 rotation = glm::rotate(glm::mat4(1),glm::radians(degree_history[i]),rotate_history[i]);
            //model = rotation*model;
        }
        x_mov += scale_x;
        y_mov += scale_y;
    }
    
    void zTranslate(double offset){
        //model = glm::translate(model,glm::vec3(0,0,offset));
        int i ;
        update_copy = glm::translate(update_copy,glm::vec3(0,0,offset));
        model = update_copy;
        for(i = 0; i<scale_history.size();i++){
            if (scale_history[i]==0){
                model = glm::scale(model,up_scale);
            }
            else{
                model = glm::scale(model,down_scale);
            }
        }
        for(i = 0;i<degree_history.size();i++){
            model = glm::rotate(model,glm::radians(degree_history[i]),rotate_history[i]);
        }
        z_mov+=offset;
    }
    void xyzRotate(float degree, glm::vec3 r_t){
        model = glm::rotate(model,glm::radians(degree),r_t);
        //glm::mat4 rotation = glm::rotate(glm::mat4(1),glm::radians(degree),r_t);
        //model = rotation*model;
        degree_history.push_back(degree);
        rotate_history.push_back(r_t);
    }
    void reset_whole(){
        model = origin_copy;
        update_copy = origin_copy;
        x_mov = 0;
        y_mov = 0;
        z_mov = 0;
        scale_history.clear();
        rotate_history.clear();
        degree_history.clear();
    }
    glm::vec3 getxyMov(){
        return glm::vec3(x_mov,y_mov,0);
    }
    
    void lightRotate(float degree, glm::vec3 t){
        glm::mat4 rotation = glm::rotate(glm::mat4(1),glm::radians(degree),t);
        model = rotation*model;
    }
    
	virtual void draw() = 0;
	virtual void update() = 0;
    virtual glm::vec3 getCenterTransformation() = 0;
    virtual glm::vec3 getScaleTrnasformation() = 0;
};

#endif


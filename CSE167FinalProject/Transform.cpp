#include "Transform.h"

Transform::Transform(glm::mat4 M){
    move_matrix = M;
    identifier = 1;
}

Transform::~Transform(){
    printf("delete transform\n");
}

void Transform::draw(glm::mat4 C){
    for(int i = 0;i<child_list.size();i++){
        child_list[i]->draw(C*move_matrix);
    }
}

void Transform::update(){
    printf("call update transform\n");
}

void Transform::addChild(Node* child){
    child_list.push_back(child);
}

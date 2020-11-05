#ifndef Transform_h
#define Transform_h

#include "Node.h"
using namespace std;

class Transform : public Node
{
private:
    std::vector<Node *> child_list;
    glm::mat4 move_matrix;
    
public:
    Transform(glm::mat4 M);
    ~Transform();
    glm::mat4 getC(){return move_matrix;}
    void setC(glm::mat4 C){move_matrix = C;}
    void draw(glm::mat4 C);
    void update();
    void addChild(Node* child);
};
#endif /* Transform_h */

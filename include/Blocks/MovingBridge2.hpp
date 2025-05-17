#ifndef MOVINGBRIDGE2_HPP
#define MOVINGBRIDGE2_HPP

#include "Blocks/Block.hpp"

class MovingBridge2 : public Block{
private:
    glm::vec2 left_pos;
    glm::vec2 right_pos;

public:
    MovingBridge2(std::string defaultPath,glm::vec2 pos, float width, 
        float height, int move_length);
    
    virtual void PhysicProcess(double time) override;
};

#endif
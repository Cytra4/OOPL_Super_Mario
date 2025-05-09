#ifndef MOVINGBRIDGE_HPP
#define MOVINGBRIDGE_HPP

#include "Blocks/Block.hpp"

class MovingBridge : public Block{
private:
    //mode = 0 -> Move down, 1 -> Move up
    int moving_mode;
    glm::vec2 bottom_pos;

public:
    MovingBridge(std::string defaultPath,glm::vec2 pos, float width, 
        float height, int mode, int move_height);
    
    virtual void PhysicProcess(double time) override;
};


#endif
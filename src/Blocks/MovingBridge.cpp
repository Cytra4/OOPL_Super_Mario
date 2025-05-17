#include "Blocks/MovingBridge.hpp"
#include "Util/Logger.hpp"

MovingBridge::MovingBridge(std::string defaultPath, glm::vec2 pos, float width, float height, int mode, int move_height)
: Block(defaultPath, pos, width, height){
    init_pos = pos;
    top_pos = pos;
    top_pos.y = move_height;
    bottom_pos = pos;
    bottom_pos.y = -move_height;

    moving_mode = mode;
    if (moving_mode == 0){
        velocity.y = -150;
    }
    else{
        velocity.y = 150;
    }
    SetJump(true);
}

void MovingBridge::PhysicProcess(double time){
    glm::vec2 bridge_pos = GetAnimationObject()->GetPosition();
    glm::vec2 bridge_velo = velocity;
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = bridge_pos.x;

    new_pos.y = bridge_pos.y + deltaTime * bridge_velo.y;
    if (moving_mode == 0 && new_pos.y < bottom_pos.y){
        new_pos = top_pos;
    }
    else if (moving_mode == 1 && new_pos.y > top_pos.y){
        new_pos = bottom_pos;
    }

    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}
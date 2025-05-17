#include "Blocks/MovingBridge2.hpp"

MovingBridge2::MovingBridge2(std::string defaultPath,glm::vec2 pos, float width, float height, int move_length)
: Block(defaultPath, pos, width, height){
    init_pos = pos;
    right_pos = pos;
    left_pos = pos;
    left_pos.x -= move_length;

    velocity.x = -100;
    SetJump(true);
}

void MovingBridge2::PhysicProcess(double time){
    glm::vec2 bridge_pos = GetAnimationObject()->GetPosition();
    glm::vec2 bridge_velo = velocity;
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.y = bridge_pos.y;

    new_pos.x = bridge_pos.x + deltaTime * bridge_velo.x;
    if (new_pos.x < left_pos.x){
        new_pos = left_pos;
        velocity.x = 100;
    }
    else if (new_pos.x > right_pos.x){
        new_pos = right_pos;
        velocity.x = -100;
    }

    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}
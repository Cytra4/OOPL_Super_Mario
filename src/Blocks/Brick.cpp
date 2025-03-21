#include "Blocks/Brick.hpp"
#include "Util/Logger.hpp"

Brick::Brick(glm::vec2 pos, float width, float height) 
: Block(RESOURCE_DIR"/Sprites/Blocks/Brick1.png", pos, width, height){
    init_pos = pos;
    top_pos = pos;
    top_pos.y += 20;
}

void Brick::SetJump(bool j){
    isJumping = j;
}

bool Brick::IsJumping(){
    return isJumping;
}

void Brick::SetVelocity(glm::vec2 new_velo){
    velocity = new_velo;
}

void Brick::PhysicProcess(double time){
    glm::vec2 brick_pos = GetAnimationObject()->GetPosition();
    glm::vec2 brick_velo = velocity;
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = brick_pos.x;

    if (reachedTop){
        new_pos.y = brick_pos.y - deltaTime*brick_velo.y;
    }
    else{
        new_pos.y = brick_pos.y + deltaTime*brick_velo.y;
    }

    if (new_pos.y >= top_pos.y){
        reachedTop = true;
        new_pos.y = top_pos.y;
    }
    else if (new_pos.y <= init_pos.y){
        reachedTop = false;
        isJumping = false;
        new_pos.y = init_pos.y;
    }
    LOG_DEBUG("New pos:{},{}",new_pos.x,new_pos.y);
    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}

void Brick::ContactBehavior(int choice){
    if (choice == 0){
        if (!isJumping){
            SetJump(true);
        }
    }
    else{
        GetBox().SetActive(false);
        MarkDestroy();
    }
}
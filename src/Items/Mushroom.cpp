#include "Items/Mushroom.hpp"
#include "Util/Logger.hpp"

Mushroom::Mushroom(int direction, int effect, std::string defaultPath, glm::vec2 pos, float width, float height)
: Item(direction, effect, defaultPath, pos, width, height){
    final_pos = pos + glm::vec2{0.0f,33.0f};
}

void Mushroom::PhysicProcess(double time){
    glm::vec2 mush_pos = GetAnimationObject()->GetPosition();
    glm::vec2 mush_velo = GetVelocity();
    CollisionBox b = GetStandingOnBlock();
    double deltaTime = time;

    glm::vec2 new_pos;

    if (reachedTop){
        if (GetDirection() == 0){
            new_pos.x = mush_pos.x - deltaTime*mush_velo.x;
        }
        else{
            new_pos.x = mush_pos.x + deltaTime*mush_velo.x;
        }
        new_pos.y = mush_pos.y + deltaTime*mush_velo.y;

        if (isOnGround()){
            mush_velo.y = 0;

            float floorY = b.GetHeight();
            new_pos.y = GetBox().GetHeight()/2 + b.GetPosition().y + floorY/2;
        }
        mush_velo.y = mush_velo.y + deltaTime*gravity*1.5;
        SetVelocity(mush_velo);
    }
    else{
        new_pos.x = mush_pos.x;
        new_pos.y = mush_pos.y + deltaTime*upSpeed;
        if (new_pos.y >= final_pos.y){
            new_pos.y = final_pos.y;
            reachedTop = true;
        }
    }

    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}
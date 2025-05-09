#include "Characters/Piranha.hpp"

Piranha::Piranha(int type, glm::vec2 position, glm::vec2 top_pos, float width, float height)
: Character(RESOURCE_DIR"/Sprites/Enemies/piranha" + std::to_string(type) + "_1.png",
100, position, width, height){
    this->top_pos = top_pos;
    bottom_pos = position;
    glm::vec2 velo = {0,100};
    SetVelocity(velo);

    std::vector<std::string> default_anim;
    for (int i=1;i<3;i++){
        default_anim.push_back(RESOURCE_DIR"/Sprites/Enemies/piranha" + std::to_string(type) + "_" + std::to_string(i) + ".png");
    }

    GetAnimationObject()->AddNewAnimation(default_anim);
    GetAnimationObject()->SetAnimation(0,100);
    GetAnimationObject()->SetLooping(true);
    GetAnimationObject()->PlayAnimation();
    GetAnimationObject()->SetCurrentAnimation(0);
}

void Piranha::SetDeath(int choice){
    if (choice == 2){
        SetDead(true);
    }
}

void Piranha::PhysicProcess(double time){
    glm::vec2 p_pos = GetAnimationObject()->GetPosition();
    glm::vec2 p_velo = GetVelocity();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos = p_pos;
    if (!stop){
        if (go_up){
            new_pos.y = p_pos.y + deltaTime*p_velo.y;
            if (new_pos.y >= top_pos.y){
                go_up = false;
                new_pos.y = top_pos.y;
                stop = true;
            }
        }
        else{
            new_pos.y = p_pos.y - deltaTime*p_velo.y;
            if (new_pos.y <= bottom_pos.y){
                go_up = true;
                new_pos.y = bottom_pos.y;
                stop = false;
            }
        }
        GetAnimationObject()->SetPosition(new_pos);
        GetBox().SetPosition(new_pos);
    }
    else{
        stayTimer -= time;
        if (stayTimer <= 0){
            stop = false;
            stayTimer = 1.0f;
        }
    }


    if (IsDead()){
        MarkDestroy();
    }
}
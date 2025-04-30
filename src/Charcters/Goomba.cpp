#include "Characters/Goomba.hpp"
#include "Util/Logger.hpp"

Goomba::Goomba(int type, glm::vec2 position, float width, float height)
: Character(RESOURCE_DIR"/Sprites/Enemies/goomba" + std::to_string(type) + "_1.png",
100, position, width, height){
    
    std::vector<std::string> default_anim;
    for (int i=1;i<3;i++){
        default_anim.push_back(RESOURCE_DIR"/Sprites/Enemies/goomba" + std::to_string(type) + "_" + std::to_string(i) + ".png");
    }
    std::vector<std::string> death_anim;
    death_anim.push_back(RESOURCE_DIR"/Sprites/Enemies/goomba" + std::to_string(type) + "_dead.png");

    GetAnimationObject()->AddNewAnimation(default_anim);
    GetAnimationObject()->AddNewAnimation(death_anim);
    GetAnimationObject()->SetAnimation(0,100);
    GetAnimationObject()->SetLooping(true);
    GetAnimationObject()->PlayAnimation();
    GetAnimationObject()->SetCurrentAnimation(0);

    SetFacingRight(false);
};

void Goomba::SetDeath(int choice){
    death = choice;
    if (choice == 1){
        SetVelocity(glm::vec2{0.0f,0.0f});
        GetBox().SetHeight(24.0f);
        auto g_pos = GetBox().GetPosition();
        g_pos.y -= 24;
        GetBox().SetPosition(g_pos);
        GetAnimationObject()->SetPosition(g_pos);
    }
    SetDead(true);
}

void Goomba::Behavior(){
    if (!IsDead()){
        if (IsFacingRight()){
            velocity.x = movespeed;
        }
        else{
            velocity.x = -movespeed;
        }
    }
    AnimationHandle();
}

void Goomba::PhysicProcess(double time){
    glm::vec2 g_pos = GetAnimationObject()->GetPosition();
    glm::vec2 g_velo = GetVelocity();
    CollisionBox b = GetStandingOnBlock();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = g_pos.x + deltaTime*g_velo.x;
    new_pos.y = g_pos.y + deltaTime*g_velo.y;
    
    if (IsDead()){
        if (death == 1){
            deathTimer -= time;
            if (deathTimer < 0){
                MarkDestroy();
            }
        }
        else{
            if (g_pos.y < -400){
                MarkDestroy();
            }
        }
    }
    
    if (IsOnGround()){
        g_velo.y = 0;

        float floorY = b.GetHeight();
        new_pos.y = GetBox().GetHeight()/2 + b.GetPosition().y + floorY/2;
    }

    g_velo.y = g_velo.y + deltaTime*gravity*1.5;
    SetVelocity(g_velo);
    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}

void Goomba::AnimationHandle(){
    int new_animation = -1;
    if (IsDead()){
        if (death == 1){
            new_animation = 1;
        }
        else{
            glm::vec2 scale = glm::vec2(1,-1);
            ani_obj->SetScale(scale);
        }
    }
    else{
        new_animation = 0;
        glm::vec2 scale = (IsFacingRight()) ? glm::vec2(1,1) : glm::vec2(-1,1);
        ani_obj->SetScale(scale);
    }

    int cur = ani_obj->GetCurrentAnimation();
    if (new_animation != -1 && new_animation != cur){
        ani_obj->SetAnimation(new_animation, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(new_animation);
    }
}
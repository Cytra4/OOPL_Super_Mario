#include "Characters/Koopa.hpp"
#include "Util/Logger.hpp"

Koopa::Koopa(int type, glm::vec2 position, float width, float height)
: Character(RESOURCE_DIR"/Sprites/Enemies/koopa" + std::to_string(type) + "_1.png",
100, position, width, height){
    
    std::vector<std::string> default_anim;
    for (int i=1;i<3;i++){
        default_anim.push_back(RESOURCE_DIR"/Sprites/Enemies/koopa" + std::to_string(type) + "_" + std::to_string(i) + ".png");
    }
    std::vector<std::string> death_anim;
    death_anim.push_back(RESOURCE_DIR"/Sprites/Enemies/koopa" + std::to_string(type) + "_dead2.png");

    GetAnimationObject()->AddNewAnimation(default_anim);
    GetAnimationObject()->AddNewAnimation(death_anim);
    GetAnimationObject()->SetAnimation(0,100);
    GetAnimationObject()->SetLooping(true);
    GetAnimationObject()->PlayAnimation();
    GetAnimationObject()->SetCurrentAnimation(0);

    SetFacingRight(false);
};

void Koopa::SetDeath(int choice){
    if (choice == 1){
        if (shelled){
            shell_kicked = !shell_kicked;
        }
        else{
            shelled = true;
        }
        death = 1;
    }
    else if (choice == 2){
        death = 2;
        SetDead(true);
    }
    else{
        death = choice;
        SetDead(false);
        shelled = false;
    }
}

void Koopa::Behavior(){
    if (death == 1){
        GetBox().SetHeight(48);
        if (shelled && !shell_kicked){
            velocity.x = 0.0f;
        }
        else{
            if (IsFacingRight()){
                velocity.x = shelled_speed;
            }
            else{
                velocity.x = -shelled_speed;
            }
        }
    }
    else if (!IsDead()){
        if (IsFacingRight()){
            velocity.x = movespeed;
        }
        else{
            velocity.x = -movespeed;
        }
    }
    AnimationHandle();
}

void Koopa::PhysicProcess(double time){
    glm::vec2 k_pos = GetAnimationObject()->GetPosition();
    glm::vec2 k_velo = GetVelocity();
    CollisionBox b = GetStandingOnBlock();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = k_pos.x + deltaTime*k_velo.x;
    new_pos.y = k_pos.y + deltaTime*k_velo.y;

    if (k_pos.y < -400){
        MarkDestroy();
    }

    // if (death == 1 && !shell_kicked){
    //     reset_timer -= deltaTime;
    //     if (reset_timer <= 0){
    //         reset_timer = 0.5f;
    //         SetDeath(0);
    //     }
    // }

    if (IsOnGround()){
        k_velo.y = 0;

        float floorY = b.GetHeight();
        new_pos.y = GetBox().GetHeight()/2 + b.GetPosition().y + floorY/2;
    }

    k_velo.y = k_velo.y + deltaTime*gravity*1.5;
    SetVelocity(k_velo);
    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);
}

void Koopa::AnimationHandle(){
    int new_animation = -1;
    if (shelled){
        new_animation = 1;
    }
    else{
        new_animation = 0;
        GetBox().SetHeight(69.0f);
        glm::vec2 scale = (IsFacingRight()) ? glm::vec2(1,1) : glm::vec2(-1,1);
        ani_obj->SetScale(scale);
    }

    if (death == 2){
        glm::vec2 scale = glm::vec2(1,-1);
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

bool Koopa::IsKicked(){
    return shell_kicked;
}
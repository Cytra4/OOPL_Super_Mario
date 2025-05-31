#include "Characters/Bowser.hpp"
#include "Util/Logger.hpp"
#include "random"

Bowser::Bowser(glm::vec2 position, float width, float height)
: Character(RESOURCE_DIR"/Sprites/Enemies/bowser1.png",25,position,width,height){
    std::vector<std::string> default_anim;
    for (int i=1;i<3;i++){
        default_anim.push_back(RESOURCE_DIR"/Sprites/Enemies/bowser" + std::to_string(i) + ".png");
    }

    GetAnimationObject()->AddNewAnimation(default_anim);
    GetAnimationObject()->SetAnimation(0,200);
    GetAnimationObject()->SetLooping(true);
    GetAnimationObject()->PlayAnimation();
    GetAnimationObject()->SetCurrentAnimation(0);

    SetHealth(8);
    SetFacingRight(false);

    target_pos = GetBox().GetPosition();
    target_pos.x -= 48;
}

void Bowser::SetDeath(int choice){
    death = choice;
}

void Bowser::Behavior(){
    if (GoLeft){
        velocity.x = -100;
    }
    else{
        velocity.x = 100;
    }
    AnimationHandle();
}

void Bowser::PhysicProcess(double time){
    glm::vec2 b_pos = GetAnimationObject()->GetPosition();
    glm::vec2 b_velo = GetVelocity();
    CollisionBox b = GetStandingOnBlock();
    double deltaTime = time;

    glm::vec2 new_pos;
    new_pos.x = b_pos.x + deltaTime*b_velo.x;
    new_pos.y = b_pos.y + deltaTime*b_velo.y;

    if (IsDead() && b_pos.y < -450){
        MarkDestroy();
    }
    
    if (IsOnGround()){
        b_velo.y = 0;

        float floorY = b.GetHeight();
        new_pos.y = GetBox().GetHeight()/2 + b.GetPosition().y + floorY/2;
    }

    if(!CanBeHurt){
        iFrame -= deltaTime;
        if (iFrame <= 0){
            CanBeHurt = true;
            iFrame = 0.5f;
        }
    }

    b_velo.y = b_velo.y + deltaTime*gravity*1.5;
    SetVelocity(b_velo);
    GetAnimationObject()->SetPosition(new_pos);
    GetBox().SetPosition(new_pos);

    if (!IsDead()){
        jump_timer -= deltaTime;
        if (jump_timer <= 0 && IsOnGround()){
            Jump();
            jump_timer = 1.5f;
        }
        shoot_timer -= deltaTime;
        if (shoot_timer <= 0){
            ShootFireball();
            shoot_timer = 2.0f;
        }
        ChooseNewTarget();
    }
}

void Bowser::AnimationHandle(){
    if (IsDead()){
        glm::vec2 scale = glm::vec2(1,-1);
        ani_obj->SetScale(scale);
    }
    else{
        glm::vec2 scale = (IsFacingRight()) ? glm::vec2(1,1) : glm::vec2(-1,1);
        ani_obj->SetScale(scale);
    }
}

void Bowser::Hurt(){
    if (!IsDead() && CanBeHurt){
        int health = GetHealth();
        health -= 1;
        if (health == 0){
            SetDead(true);
            SetDeath(1);
        }
        CanBeHurt = false;
        SetHealth(health);
    }
}

void Bowser::Jump(){
    velocity.y = jump_velo.y;
}

void Bowser::ShootFireball(){
    std::vector<std::string> default_anim;
    default_anim.reserve(2);
    default_anim.emplace_back(RESOURCE_DIR"/Sprites/Enemies/bowser_fireball1.png");
    default_anim.emplace_back(RESOURCE_DIR"/Sprites/Enemies/bowser_fireball2.png");
    glm::vec2 f_pos = ani_obj->GetPosition();
    if (IsFacingRight()){
        f_pos += glm::vec2{30,0};
    }
    else{
        f_pos -= glm::vec2{30,0};
    }

    std::shared_ptr<Fireball> fireball = std::make_shared<Fireball>(FireballType::BOWSER,
    RESOURCE_DIR"/Sprites/Enemies/bowser_fireball1.png",300 * ani_obj->GetTransform().scale.x, 
    f_pos,72.0f,24.0f);

    auto fb_ani = fireball->GetAnimationObject();
    fb_ani->AddNewAnimation(default_anim);
    fb_ani->SetAnimation(0,200);
    fb_ani->SetLooping(true);
    fb_ani->PlayAnimation();
    fb_ani->SetCurrentAnimation(0);

    if (IsFacingRight()){
        fireball->SetFacingRight(true);
    }
    else{
        fireball->SetFacingRight(false);
    }

    fireballs.push_back(fireball);
}

std::vector<std::shared_ptr<Fireball>> Bowser::GetFireballs(){
    auto obj = fireballs;
    fireballs.clear();
    return obj;
}

void Bowser::ChooseNewTarget(){
    auto b_pos = GetBox().GetPosition();
    if (GoLeft && b_pos.x <= target_pos.x){
        GoLeft = false;
        target_pos.x = b_pos.x + (10 + (std::rand() % 25));
    }
    else if (!GoLeft && b_pos.x >= target_pos.x){
        GoLeft = true;
        target_pos.x = b_pos.x - (15 + (std::rand() % 25)); 
    }
}

void Bowser::FaceToMario(std::shared_ptr<Mario> mario){
    auto m_pos = mario->GetBox().GetPosition();
    auto b_pos = GetBox().GetPosition();
    if (m_pos.x > b_pos.x){
        SetFacingRight(true);
    }
    else{
        SetFacingRight(false);
    }
}
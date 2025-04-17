#include "Items/Fireball.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Logger.hpp"

Fireball::Fireball(FireballType type, std::string ImagePath, int speed, glm::vec2 position, float width, float height){
    ani_obj = std::make_shared<AnimationObject>(ImagePath, position);
    box = CollisionBox(position, width, height);
    this->speed = speed;
    velocity.x = speed;
    this->type = type;
}

FireballType Fireball::GetType(){
    return type;
}

void Fireball::Behavior(double time){
    PhysicProcess(time);
    AnimationHandle();
}

//*TO BE DONE
void Fireball::PhysicProcess(double time){
    glm::vec2 f_pos = ani_obj->GetPosition();
    glm::vec2 f_velo = velocity;
    double deltaTime = time;

    if (!explode){
        if (type == FireballType::MARIO){
            glm::vec2 new_pos;
            new_pos.x = f_pos.x + deltaTime*velocity.x;
            new_pos.y = f_pos.y + deltaTime*velocity.y;
            f_velo.y = f_velo.y + deltaTime*gravity*4;
            ani_obj->SetPosition(new_pos);
            GetBox().SetPosition(new_pos);
            SetVelocity(f_velo);
        } 
        else if (type == FireballType::BOWSER){
    
        }
        else if (type == FireballType::BLOCK){
    
        }
        else{
            LOG_ERROR("UNEXPECTED FIREBALL TYPE?");
        }
    }
}

bool Fireball::OutOfRange(glm::vec2 camPos){
    bool Xoverlap = false;
    bool Yoverlap = false;
    glm::vec2 pos = ani_obj->GetPosition();
    Xoverlap = camPos.x - 384 >= pos.x ||
               camPos.x + 384 <= pos.x;
    Yoverlap = camPos.y - 360 >= pos.y ||
               camPos.y + 360 <= pos.y;
    return Xoverlap || Yoverlap;
}

void Fireball::AnimationHandle(){
    int new_animation = -1;
    int cur = ani_obj->GetCurrentAnimation();
    new_animation = (explode) ? 1 : 0;

    if (cur == 1 && ani_obj->IfAnimationEnds()){
        MarkRemove();
    }

    if (new_animation != -1 && new_animation != cur){
        ani_obj->SetAnimation(new_animation, 25);
        ani_obj->SetLooping(true);
        ani_obj->PlayAnimation();
        ani_obj->SetCurrentAnimation(new_animation);
    }

    glm::vec2 scale = (IsFacingRight()) ? glm::vec2(1,1) : glm::vec2(-1,1);
    ani_obj->SetScale(scale);
}

bool Fireball::IsFacingRight(){
    return facingRight;
}

void Fireball::MarkRemove(){
    remove = true;
}

bool Fireball::IsMarkedRemove(){
    return remove;
}

std::shared_ptr<AnimationObject> Fireball::GetAnimationObject(){
    return ani_obj;
}

CollisionBox& Fireball::GetBox(){
    return box;
}

void Fireball::SetVelocity(glm::vec2 new_velo){
    velocity = new_velo;
}

glm::vec2 Fireball::GetVelocity(){
    return velocity;
}

int Fireball::GetJumppower(){
    return jumpPower;
}

void Fireball::SetExplode(){
    explode = true;
}
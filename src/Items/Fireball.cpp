#include "Items/Fireball.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Logger.hpp"

Fireball::Fireball(FireballType type, std::string ImagePath, int speed, glm::vec2 position, float width, float height){
    ani_obj = std::make_shared<AnimationObject>(ImagePath, position);
    box = CollisionBox(position, width, height);
    this->speed = speed;
    this->type = type;
}

FireballType Fireball::GetType(){
    return type;
}

void Fireball::Behavior(){
    PhysicProcess();
    AnimationHandle();
}

//*TO BE DONE
void Fireball::PhysicProcess(){
    if (type == FireballType::MARIO){

    } 
    else if (type == FireballType::BOWSER){

    }
    else if (type == FireballType::BLOCK){

    }
    else{
        LOG_ERROR("UNEXPECTED FIREBALL TYPE?");
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
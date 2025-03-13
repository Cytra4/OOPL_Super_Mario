#include "Items/Fireball.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Logger.hpp"

Fireball::Fireball(int speed, float width, float height, bool facingRight,
    FireballType type, glm::vec2 pos, std::vector<std::vector<std::string>> paths){
    
    box = CollisionBox(pos,width,height);
    position = pos;
    animations = paths;
    isFacingRight = facingRight;
    this->speed = speed;
    this->type = type;
    SetAnimation(0,100);
}

FireballType Fireball::GetType(){
    return type;
}

//*To be finished, I'm trying my best to avoid this shit that gives me brain damage
void Fireball::PhysicProcess(){
    velocity.x = (isFacingRight) ? speed : speed * -1;
    if (type == FireballType::MARIO){
        
    }
    else if (type == FireballType::BOWSER){

    }
    else{

    }
    AnimationHandle();
}

bool Fireball::OutOfRange(glm::vec2 camPos){
    bool Xoverlap = false;
    bool Yoverlap = false;
    Xoverlap = camPos.x - 384 >= position.x ||
               camPos.x + 384 <= position.x;
    Yoverlap = camPos.y - 360 >= position.y ||
               camPos.y + 360 <= position.y;
    return Xoverlap || Yoverlap;
}

void Fireball::MarkRemove(){
    remove = true;
}

bool Fireball::IsMarkedRemove(){
    return remove;
}

void Fireball::AnimationHandle(){
    int newAnimation = -1;
    newAnimation = (!exploded) ? 0 : 1;

    if (newAnimation != -1 && newAnimation != currentAnimation){
        SetAnimation(newAnimation, 100);
        SetLooping(true);
        PlayAnimation();
        currentAnimation = newAnimation;
    }

    m_Transform.scale = (isFacingRight) ? glm::vec2(1,1) : glm::vec2(-1,1);
}

void Fireball::SetAnimation(int index, int interval){
    std::vector<std::string> animation;
    if (animations.size() < 1 || index >= int(animations.size())){
        LOG_ERROR("ANIMATION SETTING FAILED");
    }
    else{
        animation = animations[index];
        m_Drawable = std::make_shared<Util::Animation>(animation, false, interval, false, 0);
    }
}

void Fireball::SetLooping(bool looping){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->SetLooping(looping);
}

void Fireball::PlayAnimation(){
    auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->Play();
}
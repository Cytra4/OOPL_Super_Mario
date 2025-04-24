#include "Characters/Character.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"
#include "Util/Logger.hpp"

Character::Character(std::string ImagePath, int movespeed, glm::vec2 position, float width, float height){
    this->movespeed = movespeed;
    box = CollisionBox(position, width, height);
    standingOn = CollisionBox(glm::vec2{0,0},-1,-1);
    
    ani_obj = std::make_shared<AnimationObject>(ImagePath,position);
}

void Character::SetVelocity(glm::vec2 velocity){
    this->velocity = velocity;
}

glm::vec2 Character::GetVelocity(){
    return velocity;
}

CollisionBox& Character::GetBox(){
    return box;
}

CollisionBox Character::GetStandingOnBlock(){
    return standingOn;
}

void Character::SetStandingOnBlock(CollisionBox block){
    standingOn = block;
}

std::shared_ptr<AnimationObject> Character::GetAnimationObject(){
    return ani_obj;
}

void Character::SetFacingRight(bool direction){
    facingRight = direction;
}

bool Character::IsFacingRight(){
    return facingRight;
}

void Character::SetOnGround(bool onGround){
    this->onGround = onGround;
}

bool Character::IsOnGround(){
    return onGround;
}

void Character::SetDead(bool dead){
    this->dead = dead;
}

bool Character::IsDead(){
    return dead;
}

void Character::MarkDestroy(){
    markedDestroy = true;
}

bool Character::IsMarkedDestroy(){
    return markedDestroy;
}

void Character::SetHealth(int new_health){
    health = new_health;
}

int Character::GetHealth(){
    return health;
}

void Character::SetActive(){
    active = true;
}

bool Character::IsActive(){
    return active;
}
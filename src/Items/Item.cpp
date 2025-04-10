#include "Items/Item.hpp"

Item::Item(int direction, int effect, std::string defaultPath, glm::vec2 pos, float width, float height){
    this->effect = effect;
    this->direction = direction;
    ani_obj = std::make_shared<AnimationObject>(defaultPath, pos);
    box = CollisionBox(pos, width, height);
}

std::shared_ptr<AnimationObject> Item::GetAnimationObject(){
    return ani_obj;
}

CollisionBox& Item::GetBox(){
    return box;
}

CollisionBox Item::GetStandingOnBlock(){
    return standingOn;
}

void Item::SetStandingOnBlock(CollisionBox block){
    standingOn = block;
}

void Item::MarkDestroy(){
    destroy = true;
}

bool Item::IsMarkedDestroy(){
    return destroy;
}

int Item::GetEffect(){
    return effect;
}

void Item::SetOnGround(bool choice){
    onGround = choice;
}

bool Item::isOnGround(){
    return onGround;
}

bool Item::IsReachedTop(){
    return reachedTop;
}

void Item::SetDirection(int new_dir){
    direction = new_dir;
}

int Item::GetDirection(){
    return direction;
}

void Item::SetVelocity(glm::vec2 velocity){
    this->velocity = velocity;
}

glm::vec2 Item::GetVelocity(){
    return velocity;
}
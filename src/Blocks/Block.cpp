#include "Blocks/Block.hpp"

Block::Block(std::string defaultPath, glm::vec2 pos, float width, float height){
    box = CollisionBox(pos,width,height);
    ani_obj = std::make_shared<AnimationObject>(defaultPath, pos);
}

void Block::MarkDestroy(){
    destroy = true;
}

bool Block::IsMarkedDestroy(){
    return destroy;
}

void Block::SetHasItem(bool choice){
    hasItem = choice;
}

bool Block::HasItem(){
    return hasItem;
}

std::shared_ptr<Item> Block::GetItem(){
    return item;
}

CollisionBox& Block::GetBox(){
    return box;
}

std::shared_ptr<AnimationObject> Block::GetAnimationObject(){
    return ani_obj;
}

glm::vec2 Block::GetVelocity(){
    return velocity;
}

void Block::SetJump(bool j){
    isJumping = j;
}

bool Block::IsJumping(){
    return isJumping;
}

void Block::SetReachedTop(bool s){
    reachedTop = s;
}

bool Block::ReachedTop(){
    return reachedTop;
}
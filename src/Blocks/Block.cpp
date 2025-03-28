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

CollisionBox& Block::GetBox(){
    return box;
}

std::shared_ptr<AnimationObject> Block::GetAnimationObject(){
    return ani_obj;
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

void Block::SpawnItem(){
    
}

bool Block::HasItem(){
    return hasItem;
}
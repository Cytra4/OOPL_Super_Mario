#include "Blocks/Block.hpp"

Block::Block(std::string defaultPath, glm::vec2 pos, float width, float height){
    box = CollisionBox(pos,width,height);
    ani_obj = std::make_shared<AnimationObject>(defaultPath, pos);
}

void Block::MarkDestroy(){
    destroy = true;
}

bool Block::IsMarkedDestory(){
    return destroy;
}

CollisionBox& Block::GetBox(){
    return box;
}

std::shared_ptr<AnimationObject> Block::GetAnimationObject(){
    return ani_obj;
}
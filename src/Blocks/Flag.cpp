#include "Blocks/Flag.hpp"

Flag::Flag(glm::vec2 position, int width, int height){
    ani_obj = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Blocks/flagpole.png",position);
    box = CollisionBox(position,width,height);

    glm::vec2 flag_pos = position;
    flag_pos.x -= 24;
    flag_pos.y += 168;
    flag = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Blocks/flag.png",flag_pos);
}

CollisionBox& Flag::GetBox(){
    return box;
}

std::shared_ptr<AnimationObject> Flag::GetAnimationObject(){
    return ani_obj;
}

std::shared_ptr<AnimationObject> Flag::GetFlagAniObj(){
    return flag;
}

void Flag::MarkFinish(){
    DropFlagFinish = true;
}

bool Flag::isFlagDropDone(){
    return DropFlagFinish;
}

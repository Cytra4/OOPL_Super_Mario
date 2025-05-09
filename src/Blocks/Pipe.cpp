#include "Blocks/Pipe.hpp"

Pipe::Pipe(bool enter, std::string level, int height, glm::vec2 pos){
    canEnter = enter;
    next_level = level;
    this->height = height;
    ani_obj = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Blocks/pipe" + std::to_string(height) + ".png", pos);
    if (height == 1){
        box = CollisionBox(pos, 96, 96);
    }
    else if (height == 2){
        box = CollisionBox(pos, 96, 144);
    }
    else if (height == 3){
        box = CollisionBox(pos, 96, 192);
    }
    else if (height == 4){
        box = CollisionBox(pos, 144, 111);
    }
    else if (height == 5){
        box = CollisionBox(pos, 192, 192);
    }
    else{
        box = CollisionBox(pos, 144, 96);
    }
}

CollisionBox& Pipe::GetBox(){
    return box;
}

std::shared_ptr<AnimationObject> Pipe::GetAnimationObject(){
    return ani_obj;
}

bool Pipe::CanBeEnter(){
    return canEnter;
}

int Pipe::GetHeight(){
    return height;
}

std::string Pipe::GetNextLevel(){
    return next_level;
}
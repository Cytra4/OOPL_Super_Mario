#include "Blocks/Brick.hpp"

Brick::Brick(std::string defaultPath, glm::vec2 pos, float width, float height) 
: Block(defaultPath, pos, width, height){

}

void Brick::ContactBehavior(){
    MarkDestroy();
}
#include "Blocks/Slab.hpp"

Slab::Slab(std::string defaultPath, glm::vec2 pos, float width, float height)
: Block(defaultPath, pos, width, height){

}

void Slab::ContactBehavior(int choice){
    choice = choice;
}
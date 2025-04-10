#ifndef MUSHROOM_HPP
#define MUSHROOM_HPP

#include "Items/Item.hpp"

class Mushroom : public Item{
private:
    glm::vec2 final_pos;
    
public:
    Mushroom(int direction, int effect, std::string defaultPath, glm::vec2 pos, float width, float height);

    virtual void PhysicProcess(double time) override;
};

#endif
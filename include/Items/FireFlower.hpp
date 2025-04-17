#ifndef FIREFLOWER_HPP
#define FIREFLOWER_HPP

#include "Items/Item.hpp"

class FireFlower : public Item{
private:
    glm::vec2 final_pos;
public:
    FireFlower(int effect, std::string defaultPath, glm::vec2 pos, float width, float height);

    virtual void PhysicProcess(double time) override;   
};

#endif
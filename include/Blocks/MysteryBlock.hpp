#ifndef MYSTERYBLOCK_HPP
#define MYSTERYBLOCK_HPP

#include "Blocks/Block.hpp"

//Class for MysteryBlock, there would be two kinds of MysteryBlock:
//1.One with items like mushroom, fireflower and such
//2.One with coin

class MysteryBlock : public Block{
private:
public:
    MysteryBlock(std::string defaultPath, glm::vec2 pos, float width, float height, int count);

    virtual void PhysicProcess(double time) override;

    virtual void ContactBehavior(int choice) override;
};

#endif
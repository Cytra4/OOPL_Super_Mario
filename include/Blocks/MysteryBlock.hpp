#ifndef MYSTERYBLOCK_HPP
#define MYSTERYBLOCK_HPP

#include "Blocks/Block.hpp"

//Class for MysteryBlock, there would be two kinds of MysteryBlock:
//1.One with items like mushroom, fireflower and such
//2.One with coin

class MysteryBlock : public Block{
private:
    bool hasItem = true;
public:
    MysteryBlock(glm::vec2 pos, float width, float height);

    virtual void ContactBehavior(int choice) override;
};

#endif
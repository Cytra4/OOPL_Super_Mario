#ifndef MYSTERYBLOCK_HPP
#define MYSTERYBLOCK_HPP

#include "Blocks/Block.hpp"
#include "Items/Mushroom.hpp"
#include "Items/Coin.hpp"
#include "Items/FireFlower.hpp"

//Class for MysteryBlock, there would be two kinds of MysteryBlock:
//1.One with items like mushroom, fireflower and such
//2.One with coin

class MysteryBlock : public Block{
private:
    std::string itemType;
    int itemCount;
    bool empty = false;

    std::shared_ptr<Util::SFX> mys_SE;
public:
    MysteryBlock(std::string itemType, std::string defaultPath, glm::vec2 pos, float width, float height, int count);

    virtual void SpawnItem(std::shared_ptr<Mario> mario) override;

    virtual void PhysicProcess(double time) override;

    virtual void ContactBehavior(int choice, std::shared_ptr<Mario> mario) override;
};

#endif
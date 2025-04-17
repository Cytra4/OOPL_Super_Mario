#ifndef COIN_HPP
#define COIN_HPP

#include "Items/Item.hpp"

//So this is the class of Coin, and since there's two kind of coins
//1.Coin that pops up when Mario hit the block
//2.Coin that just shows like a block
//I think it's kind of stupid to seperate two kind of coin class,
//so I think I'm just gonna put them in the same class but like different types, ex: type = 0, type = 1

class Coin : public Item{
private:
    int type;
    glm::vec2 final_pos;
    glm::vec2 init_pos;
public:
    Coin(int type, std::string defaultPath, glm::vec2 pos, float width, float height);

    virtual void PhysicProcess(double time) override;    
};

#endif
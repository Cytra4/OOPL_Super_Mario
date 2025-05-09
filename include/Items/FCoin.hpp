#ifndef FCOIN_HPP
#define FCOIN_HPP

#include "Items/Item.hpp"

//Not to be confused with Coin , this Coin works like a floating block, disappears when touched  
class FCoin : public Item{
private:
    int score = 200;
public:
    FCoin(std::string choice, std::string defaultPath, glm::vec2 pos, float width, float height);
};

#endif
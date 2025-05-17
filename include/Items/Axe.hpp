#ifndef AXE_HPP
#define AXE_HPP

#include "Items/Item.hpp"

class Axe : public Item{
private:

public:
    Axe(std::string defaultPath, glm::vec2 pos, float width, float height);
};

#endif
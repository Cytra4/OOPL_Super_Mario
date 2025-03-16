#ifndef BRICK_HPP
#define BRICK_HPP

#include "Blocks/Block.hpp"

class Brick : public Block{
private:

public:
    Brick(std::string defaultPath, glm::vec2 pos, float width, float height);

    virtual void ContactBehavior() override;
};

#endif
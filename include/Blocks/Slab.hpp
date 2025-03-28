#ifndef SLAB_HPP
#define SLAB_HPP

#include "Blocks/Block.hpp"

class Slab : public Block{
private:

public:
    Slab(std::string defaultPath, glm::vec2 pos, float width, float height);
    
    virtual void ContactBehavior(int choice) override;
};

#endif
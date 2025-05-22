#ifndef BRICK_HPP
#define BRICK_HPP

#include "Blocks/Block.hpp"
#include "Util/SFX.hpp"

class Brick : public Block{
private:
    std::shared_ptr<Util::SFX> brick_SE;
public:
    Brick(std::string defaultPath, glm::vec2 pos, float width, float height);

    void SetVelocity(glm::vec2 new_velo);

    virtual void PhysicProcess(double time) override;

    virtual void ContactBehavior(int choice, std::shared_ptr<Mario> mario) override;
};

#endif
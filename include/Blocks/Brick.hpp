#ifndef BRICK_HPP
#define BRICK_HPP

#include "Blocks/Block.hpp"

class Brick : public Block{
private:
    bool isJumping = false;
    bool reachedTop = false;
    glm::vec2 init_pos;
    glm::vec2 top_pos;
    glm::vec2 velocity = {0.0f,150.0f};

public:
    Brick(glm::vec2 pos, float width, float height);

    void SetJump(bool j);

    bool IsJumping();

    void SetVelocity(glm::vec2 new_velo);

    void PhysicProcess(double time);

    virtual void ContactBehavior(int choice) override;
};

#endif
#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include "Characters/Character.hpp"

class Goomba : public Character{
private:
    //So there will be two kinds of death: 1. Die from jump , 2. Die from fireball
    int death = 0;
    bool markedDestroy = false;

    //When Goomba dies from Mario's jump, enables the timer
    //Goomba will be marked destroy when timer runs up
    double deathTimer = 1.5f;
public:
    Goomba(int type, glm::vec2 position, float width, float height);

    void SetDeath(int choice);

    virtual void Behavior() override;

    virtual void PhysicProcess(double time) override;

    virtual void AnimationHandle() override;
};

#endif

//Animations:
//0: default
//1: dead
#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include "Characters/Character.hpp"
#include "GameUtils/IEnemyAttribute.hpp"

class Goomba : public Character , public IEnemyAttribute{
private:

public:
    Goomba(int type, glm::vec2 position, float width, float height);

    virtual void SetDeath(int choice) override;

    virtual void Behavior() override;

    virtual void PhysicProcess(double time) override;

    virtual void AnimationHandle() override;
};

#endif

//Animations:
//0: default
//1: dead
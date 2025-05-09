#ifndef PIRANHA_HPP
#define PIRANHA_HPP

#include "Characters/Character.hpp"
#include "GameUtils/IEnemyAttribute.hpp"

class Piranha : public Character , public IEnemyAttribute{
private:
    glm::vec2 top_pos;
    glm::vec2 bottom_pos;
    double stayTimer = 2.0f;
    bool go_up = true;
    bool stop = false;
public:
    Piranha(int type, glm::vec2 position, glm::vec2 top_pos, float width, float height);

    virtual void SetDeath(int choice) override;

    virtual void PhysicProcess(double time) override;
};

#endif
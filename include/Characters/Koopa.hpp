#ifndef KOOPA_HPP
#define KOOPA_HPP

#include "Characters/Character.hpp"
#include "GameUtils/IEnemyAttribute.hpp"
#include "Util/SFX.hpp"

class Koopa : public Character , public IEnemyAttribute{
private:
    //When Mario jumps it, Koopa turns to shell(shelled = true)
    //if Koopa gets jumped again, Koopa will start moving around at high speed (shell_kicked = true) 
    bool shelled = false;
    bool shell_kicked = false;
    double shelled_speed = 600.0f;

public:
    Koopa(int type, glm::vec2 position, float width, float height);

    virtual void SetDeath(int choice) override;

    virtual void Behavior() override;

    virtual void PhysicProcess(double time) override;

    virtual void AnimationHandle() override;

    bool IsKicked();
};

#endif
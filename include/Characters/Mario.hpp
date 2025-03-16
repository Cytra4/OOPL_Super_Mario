#ifndef MARIO_HPP
#define MARIO_HPP

#include "Character.hpp"
#include "Items/Fireball.hpp"

class Mario : public Character{
public:
    enum class Mode {
        SMALL,
        BIG,
        FIRE,
    };

    Mario(int jump, int movespeed, glm::vec2 position, float width, float height);

    Mode GetMarioMode();

    void SetMarioMode(Mario::Mode mode);

    bool IsRunning();

    virtual void Behavior() override;

    virtual void PhysicProcess(double time) override;

    virtual void AnimationHandle() override;

    virtual void Hurt() override;

    //Based on mario's mode, update collision box, health
    void StateUpdate(Mode new_mode);

    //Based on mario's mode, update his animation path (i guess)
    void AnimationUpdate(std::string new_mode);

    void ShootFireball();

    std::vector<std::shared_ptr<Fireball>> GetFireballs();

private:
    int jumpPower;
    int jumpFallGravity = 1;
    Mode mario_mode;
    bool isRunning = false;
    bool canShootFireballs = false;
    std::vector<std::shared_ptr<Fireball>> fireballs;

    bool isJumping = false;
};

// Animations:
// 0: Mario run
// 1: Mario jump
// 2: Mario dead
#endif
#ifndef MARIO_HPP
#define MARIO_HPP

#include "Character.hpp"
#include "Items/Fireball.hpp"
#include "Util/SFX.hpp"

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

    void DestroyMarkedFireball();

    std::vector<std::shared_ptr<Fireball>> GetFireballs();

    void AddCoin();

    void SetCoin(int coin);

    int GetCoin();

    void AddScore(int add);

    void SetScore(int score);

    int GetScore();

    void AddLive();

    void SetLive(int live);

    int GetLive();

    std::shared_ptr<Util::SFX> Get_SE();

private:
    int coins = 0;
    int score = 0;
    int lives = 3;

    int jumpPower;
    int jumpFallGravity = 1;
    Mode mario_mode;
    bool isRunning = false;
    bool isJumping = false;
    bool isJumpKeyHeld = false;
    bool canShootFireballs = false;
    double fireball_CD_timer = 0;
    bool canTakeDamage = true;
    double iFrames = 2.5f;
    std::vector<std::shared_ptr<Fireball>> fireballs;

    std::shared_ptr<Util::SFX> mario_SE;
};

// Animations:
// 0: Mario run
// 1: Mario jump
// 2: Mario dead
// 3: Mario climb
#endif
#ifndef BOWSER_HPP
#define BOWSER_HPP

#include "Characters/Character.hpp"
#include "Characters/Mario.hpp"
#include "GameUtils/IEnemyAttribute.hpp"
#include "Items/Fireball.hpp"

class Bowser : public Character, public IEnemyAttribute{
private:
    glm::vec2 jump_velo = {0,250};
    
    glm::vec2 target_pos;

    std::vector<std::shared_ptr<Fireball>> fireballs; 

    //Could be adjusted
    double jump_timer = 1.5f;
    double shoot_timer = 2.0f;
    double iFrame = 0.5f;
    bool CanBeHurt = true;
    bool GoLeft = true;

public:
    Bowser(glm::vec2 position, float width, float height);

    virtual void SetDeath(int choice) override;

    virtual void Behavior() override;

    virtual void PhysicProcess(double time) override;
    
    virtual void AnimationHandle() override;

    virtual void Hurt() override;

    void Jump();

    void ShootFireball();

    std::vector<std::shared_ptr<Fireball>> GetFireballs();

    void ChooseNewTarget();

    void FaceToMario(std::shared_ptr<Mario> mario);
};

#endif
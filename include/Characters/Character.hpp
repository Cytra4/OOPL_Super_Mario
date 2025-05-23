#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "GameUtils/AnimationObject.hpp"
#include "GameUtils/CollisionBox.hpp"
#include <string>

class Character{
private:
    CollisionBox box;
    //This is for saving the block that the character is standing on for position fixing in physic process
    CollisionBox standingOn;
    bool onGround = false;
    bool dead = false;
    bool facingRight = true;
    bool markedDestroy = false;
    bool active = false;
    int health = 1;

protected:
    //Move speed for the character
    int movespeed;

    //basically where the character move
    glm::vec2 velocity = {0,0};

    //the gravity of character
    //right now -500 seems to be the most legit value (could be changed)
    int gravity = -500;

    std::shared_ptr<AnimationObject> ani_obj;

    bool isVisible = true;

public:
    Character(std::string ImagePath, int movespeed, glm::vec2 position, float width, float height);

    void SetVelocity(glm::vec2 velocity);

    glm::vec2 GetVelocity();

    CollisionBox& GetBox();

    CollisionBox GetStandingOnBlock();

    void SetStandingOnBlock(CollisionBox block);

    std::shared_ptr<AnimationObject> GetAnimationObject();

    //How the character move
    //ex: Mario can move around and jump, Goombas only move l/r
    virtual void Behavior(){};

    //how the character's physic works (velocity, gravity...)
    //*time=time is just that cmake warning hurts my eye
    virtual void PhysicProcess(double time){time = time;};

    //how the character's animation is being handled
    virtual void AnimationHandle(){};

    //how the character being affected when hurt
    virtual void Hurt(){};

    void SetFacingRight(bool direction);

    bool IsFacingRight();
    
    void SetOnGround(bool onGround);

    bool IsOnGround();

    void SetDead(bool dead);
    
    bool IsDead();

    void MarkDestroy();

    bool IsMarkedDestroy();

    void SetHealth(int new_health);

    int GetHealth();

    void SetActive();

    bool IsActive();
};

#endif
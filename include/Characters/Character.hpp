#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Util/GameObject.hpp"
#include "GameUtils/CollisionBox.hpp"
#include <string>

class Character : public Util::GameObject{
private:
    //Idea right now is character start with defaultSprite
    //sprite will change based on behavior, and switch to different animation or smth
    std::string defaultSprite;
    std::vector<std::vector<std::string>> animationPaths;

    CollisionBox box;
    //This is for saving the block that the character is standing on for position handling in physic process
    //This is CollisionBox for now, but would be changed to Block class in the future
    CollisionBox standingOn;
    bool onGround = false;
    bool dead = false;
    bool facingRight = true;
    int health = 1;

protected:
    //The index of current animation setted
    //*this is to prevent same animation keep being set
    int current_animation = -1;

    //Move speed for the character
    int movespeed;
    //basically where the character move
    glm::vec2 velocity = {0,0};
    //the gravity of character
    //right now -500 seems the most legit value (could be changed)
    int gravity = -500;

public:
    Character(std::string ImagePath, int movespeed, glm::vec2 position, float width, float height);

    std::string GetDefaultSprite();

    void SetDefaultSprite(std::string sprite);

    //Add new animation to animationPaths 
    void AddNewAnimation(std::vector<std::string> animation);

    //Choose which animation is going to play, nothing to do with modify
    void SetAnimation(int index, int interval);

    std::vector<std::vector<std::string>>& GetAnimationPaths();
    
    void SetLooping(bool looping);

    bool IfAnimationEnds();
    
    bool IsPlaying();

    void PlayAnimation();

    void PauseAnimation();

    glm::vec2 GetPosition();

    void SetVelocity(glm::vec2 velocity);

    glm::vec2 GetVelocity();

    bool GetVisibility();

    void SetPosition(const glm::vec2& Position);

    CollisionBox& GetBox();

    CollisionBox GetStandingOnBlock();
    //* ↑↓ These two functions should be edited after block class is finished
    void SetStandingOnBlock(CollisionBox block);

    //How the character move
    //ex: Mario can move around and jump, Goombas only move l/r
    virtual void Behavior(){};

    //how the character's physic works (velocity, gravity...)
    //*time=time is just gcc error hurts my eye
    virtual void PhysicProcess(double time){time = time;};

    //how the character's animation being handled
    virtual void AnimationHandle(){};

    //how the character being affected when hurt
    virtual void Hurt(){};

    void SetFacingRight(bool direction);

    bool IsFacingRight();
    
    void SetOnGround(bool onGround);

    bool IsOnGround();

    void SetDead(bool dead);
    
    bool IsDead();

    void SetHealth(int new_health);

    int GetHealth();
};

#endif
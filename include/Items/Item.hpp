#ifndef ITEM_HPP
#define ITEM_HPP

#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"

class Item{
private:
    std::shared_ptr<AnimationObject> ani_obj;
    CollisionBox box;
    CollisionBox standingOn;
    glm::vec2 velocity = {200.0f,0.0f};
    bool onGround = false;
    bool destroy = false;
    //How this item will change mario's state, ex:1 -> Turns to Big Mario
    int effect;
    //Where the item will go, direction = 0 -> Go left, =1 -> Go right
    int direction;
    //*This is for future use
    int score;

protected:
    bool reachedTop = false;
    float gravity = -500.0f;
    float upSpeed = 150.0f;

public:
    Item(int direction, int effect, std::string defaultPath, glm::vec2 pos, float width, float height);

    std::shared_ptr<AnimationObject> GetAnimationObject();

    CollisionBox& GetBox();

    CollisionBox GetStandingOnBlock();

    void SetStandingOnBlock(CollisionBox block);

    virtual void PhysicProcess(double time){time = time;};

    void MarkDestroy();

    bool IsMarkedDestroy();

    int GetEffect();

    void SetOnGround(bool choice);

    bool isOnGround();

    bool IsReachedTop();

    void SetDirection(int new_dir);

    int GetDirection();

    void SetVelocity(glm::vec2 velocity);

    glm::vec2 GetVelocity();
};

#endif
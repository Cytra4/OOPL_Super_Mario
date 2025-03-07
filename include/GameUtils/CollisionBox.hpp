#ifndef COLLISIONBOX_HPP
#define COLLISIONBOX_HPP

#include "pch.hpp"

//Collision box for blocks, characters ... gameobjects
class CollisionBox{
public:
    enum class State {
        NONE,
        COLLISION_ON_TOP,
        COLLISION_ON_BOTTOM,
        COLLISION_ON_LEFT,
        COLLISION_ON_RIGHT
    };

    CollisionBox() = default;
    CollisionBox(glm::vec2 objPos,float width, float height);

    //Check if itself collides with the other collision box
    //if collides, change itself's currentState

    //Right now my idea is that this would work with PhysicProcess() function of Character
    //the state of the CollisionBox would affect the character's velocity
    bool ifCollide(CollisionBox object);

    void SetState(State newState);

    State GetCurrentState();

    void SetWidth(float newWidth);

    float GetWidth();

    void SetHeight(float newHeight);

    float GetHeight();

    void SetPosition(glm::vec2 newPos);

    glm::vec2 GetPosition();

private:
    float width;
    float height;
    glm::vec2 position;
    State currentState = State::NONE;
};

#endif
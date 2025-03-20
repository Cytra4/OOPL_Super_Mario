#ifndef COLLISIONBOX_HPP
#define COLLISIONBOX_HPP

#include "pch.hpp"

//Collision box for blocks, characters ... gameobjects

class CollisionBox{
public:
    enum class State {
        NONE,
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

    CollisionBox() = default;
    CollisionBox(glm::vec2 objPos,float width, float height);

    //Check if itself collides with the other collision box
    //Probably the core function of this class

    //Right now my idea is that this would work with PhysicProcess() function of Character
    //the state of the CollisionBox would affect the character's velocity

    //*I'm still thinking which way I should write this function:
    // 1.change the state and return T/F
    //2.Simply returns the collision state (currentState just won't exist or smth)
    bool ifCollide(CollisionBox object);

    void SetState(State newState);

    State GetCurrentState();

    void SetWidth(float newWidth);

    float GetWidth();

    void SetHeight(float newHeight);

    float GetHeight();

    void SetPosition(glm::vec2 newPos);

    glm::vec2 GetPosition();

    void SetActive(bool ac);

    bool IsActive();

private:
    float width;
    float height;
    glm::vec2 position;
    bool isActive = true;
    State currentState = State::NONE;
};

#endif
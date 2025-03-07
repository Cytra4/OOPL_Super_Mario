#include "GameUtils/CollisionBox.hpp"
#include "Util/Logger.hpp"

CollisionBox::CollisionBox(glm::vec2 objPos, float width, float height){
    this->width = width;
    this->height = height;
    position = objPos;
}

bool CollisionBox::ifCollide(CollisionBox object){
    float x1 = position.x;
    float y1 = position.y;
    float w1 = width;
    float h1 = height;

    float x2 = object.GetPosition().x;
    float y2 = object.GetPosition().y;
    float w2 = object.GetWidth();
    float h2 = object.GetHeight();

    //collision on x axis
    bool collisionX = (x1 + w1/2 > x2 - w2/2) && (x1 - w1/2 < x2 + w2/2);
    bool collisionY = (y1 + h1/2 > y2 - h2/2) && (y1 - h1/2 < y2 + h2/2);
    
    //both axis need to have collision
    //if only one axis collides but other doesn't,then it would only be alignment instead of actual collision
    if (collisionX && collisionY){
        float overlapLeft = (x2 + w2 / 2) - (x1 - w1 / 2);
        float overlapRight = (x1 + w1 / 2) - (x2 - w2 / 2);
        float overlapTop = (y1 + h1 / 2) - (y2 - h2 / 2);
        float overlapBottom = (y2 + h2 / 2) - (y1 - h1 / 2);

        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        if (minOverlap == overlapTop) {
            currentState = State::COLLISION_ON_TOP;
            //LOG_DEBUG("COLLIDE TOP");
        } else if (minOverlap == overlapBottom) {
            currentState = State::COLLISION_ON_BOTTOM;
            //LOG_DEBUG("COLLIDE BOTTOM");
        } else if (minOverlap == overlapLeft) {
            currentState = State::COLLISION_ON_LEFT;
            //LOG_DEBUG("COLLIDE LEFT");
        } else if (minOverlap == overlapRight) {
            currentState = State::COLLISION_ON_RIGHT;
            //LOG_DEBUG("COLLIDE RIGHT");
        }

        return true;
    }

    currentState = State::NONE;
    //LOG_DEBUG("NO COLLISION");
    return false;
}

void CollisionBox::SetState(State newState){
    currentState = newState;
}

CollisionBox::State CollisionBox::GetCurrentState(){
    return currentState;
}

void CollisionBox::SetWidth(float newWidth){
    width = newWidth;
}

float CollisionBox::GetWidth(){
    return width;
}

void CollisionBox::SetHeight(float newHeight){
    height = newHeight;
}

float CollisionBox::GetHeight(){
    return height;
}

void CollisionBox::SetPosition(glm::vec2 newPos){
    position = newPos;
}

glm::vec2 CollisionBox::GetPosition(){
    return position;
}
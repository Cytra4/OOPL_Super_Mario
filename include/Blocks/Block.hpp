#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"

//Base class for all the blocks
class Block{
private:
    CollisionBox box;
    std::shared_ptr<AnimationObject> ani_obj;
    bool destroy = false;

public:
    Block(std::string defaultPath, glm::vec2 pos, float width, float height);

    void MarkDestroy();

    bool IsMarkedDestory();
    
    virtual void ContactBehavior(){};

    CollisionBox& GetBox();

    std::shared_ptr<AnimationObject> GetAnimationObject();
};

#endif
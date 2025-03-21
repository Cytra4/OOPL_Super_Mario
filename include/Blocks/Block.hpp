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
    
    //The idea is that Block would have different ContactBehavior based on the given choice
    //ex: A brick block, if choice = 0, the brick will jump up a bit and fall down back to original position
    //if choice = 1, the brick will break
    virtual void ContactBehavior(int choice){choice = choice;};

    CollisionBox& GetBox();

    std::shared_ptr<AnimationObject> GetAnimationObject();
};

#endif
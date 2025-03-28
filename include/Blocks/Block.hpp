#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"

//Base class for all the blocks
class Block{
private:
    CollisionBox box;
    std::shared_ptr<AnimationObject> ani_obj;
    //Here's missing a shared pointer of item (that the block has)

    bool destroy = false;
    bool isJumping = false;
    bool reachedTop = false;

protected:
    glm::vec2 init_pos;
    glm::vec2 top_pos;
    glm::vec2 velocity = {0.0f,150.0f};
    bool hasItem;
    int itemCount;

public:
    Block(std::string defaultPath, glm::vec2 pos, float width, float height);

    void MarkDestroy();

    bool IsMarkedDestroy();
    
    //The idea is that Block would have different ContactBehavior based on the given choice
    //ex: A brick block, if choice = 0, the brick will jump up a bit and fall down back to original position
    //if choice = 1, the brick will break
    virtual void ContactBehavior(int choice){choice = choice;};

    virtual void PhysicProcess(double time){time = time;}

    CollisionBox& GetBox();

    std::shared_ptr<AnimationObject> GetAnimationObject();

    void SetJump(bool j);

    bool IsJumping();

    void SetReachedTop(bool s);

    bool ReachedTop();

    //*TO BE DONE
    void SpawnItem();

    bool HasItem();
};

#endif
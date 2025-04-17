#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"
#include "Items/Item.hpp"
#include "Characters/Mario.hpp"

//Base class for all the blocks
class Block{
private:
    CollisionBox box;
    std::shared_ptr<AnimationObject> ani_obj;
    bool destroy = false;
    bool isJumping = false;
    bool reachedTop = false;

protected:
    glm::vec2 init_pos;
    glm::vec2 top_pos;
    glm::vec2 velocity = {0.0f,150.0f};
    bool hasItem = false;
    std::shared_ptr<Item> item;

public:
    Block(std::string defaultPath, glm::vec2 pos, float width, float height);

    void MarkDestroy();

    bool IsMarkedDestroy();
    
    //The idea is that Block would have different ContactBehavior based on the given choice
    //ex: A brick block, if choice = 0, the brick will jump up a bit and fall down back to original position
    //if choice = 1, the brick will break
    virtual void ContactBehavior(int choice, std::shared_ptr<Mario> mario){choice = choice;};

    virtual void PhysicProcess(double time){time = time;}

    virtual void SpawnItem(std::shared_ptr<Mario> mario){mario = mario;};

    void SetHasItem(bool choice);
    
    //Will be use to check if the item is spawned, if item spawned, throw the item to MapManager and CollisionManager
    bool HasItem();
    
    std::shared_ptr<Item> GetItem();

    CollisionBox& GetBox();

    std::shared_ptr<AnimationObject> GetAnimationObject();

    void SetJump(bool j);

    bool IsJumping();

    void SetReachedTop(bool s);

    bool ReachedTop();
};

#endif
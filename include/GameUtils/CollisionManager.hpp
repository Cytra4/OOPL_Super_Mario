#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include "pch.hpp"
#include "Blocks/Block.hpp"
#include "Characters/Mario.hpp"

//This class will be use to handle all the collision of characters, blocks and other objects with collision box 

class CollisionManager{
private:
    std::shared_ptr<Mario> mario;
    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<CollisionBox> floor_boxes;
    float map_barrier;
    bool c_flag;

    //Missing objects:
    //1.Vector storing all the enemies
    //2.Vector storing all the spawned items

public:
    CollisionManager(std::shared_ptr<Mario> mario, std::vector<std::shared_ptr<Block>> blocks,
    std::vector<CollisionBox> floor_boxes, glm::vec2 mapSize);

    //The main function that CollisionManager will do in every Update()
    void UpdateProcess(double time);
    
    //Check the collision of character and blocks
    void BlockCollisionProcess(std::shared_ptr<Character> character, double time);

    //Check the collision of character and floor_boxes/barrier
    void OtherCollisionProcess(std::shared_ptr<Character> character);
    
    //Check the collision of Mario and Enemies
    void EnemyCollisionProcess();

    //↑↓ *TO BE DONE
    
    //Check the collision of Mario and items
    void ItemCollisionProcess();
    
    //Remove blocks that are marked destroy to reduce process time
    void RemoveMarkedObject();

    //Remove enemies that are dead to reduce process time
    //*I'm still thinking if this function is necessary 
    //Since I guess this can also be handled in RemoveMarkedObject()
    void RemoveDeadEnemy();
};

#endif
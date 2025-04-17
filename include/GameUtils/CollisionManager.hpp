#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include "pch.hpp"
#include "Blocks/Block.hpp"
#include "Blocks/Pipe.hpp"
#include "Items/Item.hpp"
#include "Characters/Mario.hpp"

//This class will be use to handle all the collision of characters, blocks and other objects with collision box 

class CollisionManager{
private:
    std::shared_ptr<Mario> mario;
    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<std::shared_ptr<Pipe>> pipes;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<CollisionBox> floor_boxes;
    std::queue<std::shared_ptr<Fireball>> m_fireballs;
    float map_barrier;
    bool c_flag;
    bool i_flag;

    //Missing objects:
    //1.Vector storing all the enemies
    //2.Vector storing all the spawned items

public:
    CollisionManager(std::shared_ptr<Mario> mario, std::vector<std::shared_ptr<Block>> blocks,
    std::vector<std::shared_ptr<Pipe>> pipes, std::vector<CollisionBox> floor_boxes, glm::vec2 mapSize);

    //The main function that CollisionManager will do in every Update()
    void UpdateProcess(double time, glm::vec2 CameraPosition);
    
    //Check the collision of character and blocks
    void BlockCollisionProcess(std::shared_ptr<Character> character, double time, int mode);

    void BlockCollisionProcess(std::shared_ptr<Item> item);

    void BlockCollisionProcess(std::shared_ptr<Fireball> fb);

    //Check the collision of character and floor_boxes/barrier
    void OtherCollisionProcess(std::shared_ptr<Character> character);
    
    void OtherCollisionProcess(std::shared_ptr<Item> item);

    void OtherCollisionProcess(std::shared_ptr<Fireball> fb);

    //Check the collision of Mario and Enemies
    void EnemyCollisionProcess();

    //↑↓ *TO BE DONE
    
    //Check the collision of Mario and items
    void ItemCollisionProcess();
    
    //Remove stuff that are marked destroy to reduce process time
    void RemoveMarkedObject();

    //Remove enemies that are dead to reduce process time
    //*I'm still thinking if this function is necessary 
    //Since I guess this can also be handled in RemoveMarkedObject()
    void RemoveDeadEnemy();

    void AddItem(std::shared_ptr<Item> item);

    void SetFireballs(std::queue<std::shared_ptr<Fireball>> fireballs);
};

#endif
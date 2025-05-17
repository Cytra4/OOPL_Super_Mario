#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include "pch.hpp"
#include "Blocks/Block.hpp"
#include "Blocks/Pipe.hpp"
#include "Blocks/Flag.hpp"
#include "Blocks/FireBar.hpp"
#include "Items/Item.hpp"
#include "Characters/Mario.hpp"
#include "Characters/Goomba.hpp"
#include "Characters/Koopa.hpp"
#include "Characters/Piranha.hpp"
#include "Characters/Bowser.hpp"

//This class will be use to handle all the collision of characters, blocks and other objects with collision box 

class CollisionManager{
private:
    std::shared_ptr<Mario> mario;
    std::vector<std::shared_ptr<Goomba>> goombas;
    std::vector<std::shared_ptr<Koopa>> koopas;
    std::vector<std::shared_ptr<Piranha>> piranhas;
    std::vector<std::shared_ptr<Bowser>> bowser;

    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<std::shared_ptr<FireBar>> firebars;
    std::vector<std::shared_ptr<Pipe>> pipes;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<CollisionBox> floor_boxes;
    std::shared_ptr<Flag> flag;
    
    std::queue<std::shared_ptr<Fireball>> m_fireballs;
    std::queue<std::shared_ptr<Fireball>> b_fireballs;

    std::vector<float> map_barriers = {};
    bool c_flag;
    bool i_flag;
    bool has_flag = false;
    bool level_clear = false;
    bool game_clear = false;
    bool enter_secret_level = false;

    //I just realized 1-2 has more than 1 secret pipe, so added this to identify
    std::string pipeEnterLevel;

public:
    CollisionManager(std::shared_ptr<Mario> mario, 
    std::vector<std::shared_ptr<Block>> blocks, std::vector<std::shared_ptr<Pipe>> pipes, 
    std::vector<CollisionBox> floor_boxes, glm::vec2 mapSize);

    //The main function that CollisionManager will do in every Update()
    void UpdateProcess(double time, glm::vec2 CameraPosition);
    
    //Check the collision of character and blocks
    void BlockCollisionProcess(std::shared_ptr<Character> character, int mode);

    void BlockCollisionProcess(std::shared_ptr<Item> item);

    void BlockCollisionProcess(std::shared_ptr<Fireball> fb);

    //Check the collision of character and floor_boxes/barrier
    void OtherCollisionProcess(std::shared_ptr<Character> character, int mode);
    
    void OtherCollisionProcess(std::shared_ptr<Item> item);

    void OtherCollisionProcess(std::shared_ptr<Fireball> fb, int type);

    //Check the collision of Mario and Enemies
    void EnemyCollisionProcess();

    //Check collision between Enemies
    void EECollisionProcess(std::shared_ptr<Goomba> goomba, int index);

    void EECollisionProcess(std::shared_ptr<Koopa> koopa, int index);

    //Check the collision of Mario and items
    void ItemCollisionProcess();
    
    //Remove stuff that are marked destroy to reduce process time
    void RemoveMarkedObject();

    void AddItem(std::shared_ptr<Item> item);

    void SetMFireballs(std::queue<std::shared_ptr<Fireball>> fireballs);

    void SetBFireballs(std::queue<std::shared_ptr<Fireball>> fireballs);

    void SetGoombas(std::vector<std::shared_ptr<Goomba>> goombas);

    void SetKoopas(std::vector<std::shared_ptr<Koopa>> koopas);

    void SetPiranhas(std::vector<std::shared_ptr<Piranha>> piranhas);

    void SetBowser(std::vector<std::shared_ptr<Bowser>> bowser);

    void SetFlag(std::shared_ptr<Flag> flag);

    void SetItem(std::vector<std::shared_ptr<Item>> items);
    
    void SetFirebar(std::vector<std::shared_ptr<FireBar>> firebars);

    bool IsLevelCleared();

    bool EnterSecretLevel();

    bool IsGameCleared();

    bool GetCFlag();

    void SetCFlag(bool choice);

    void ResetStates();

    //This will be used to test if the enemy is in Camera range
    //if not then enemy will not be doing PhysicProcess() (or Behavior() too)
    bool EnemyInCameraRange(std::shared_ptr<Character> enemy, glm::vec2 mario_pos);

    void UpdateLBarrier(std::string level);

    //For 1_4
    void SetRBarrier(float new_pos);

    std::string GetPipeEnterLevel();
};

#endif
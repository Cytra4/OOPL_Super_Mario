#ifndef MAPMANAGER_HPP
#define MAPMANAGER_HPP

#include "GameUtils/Background.hpp"
#include "GameUtils/CollisionManager.hpp"
#include "Blocks/Brick.hpp"
#include "Blocks/MysteryBlock.hpp"
#include "Blocks/FireBar.hpp"
#include "Blocks/Slab.hpp"
#include "Blocks/Pipe.hpp"
#include "Blocks/Flag.hpp"
#include "Blocks/MovingBridge.hpp"
#include "Blocks/MovingBridge2.hpp"
#include "Items/Item.hpp"
#include "Items/FCoin.hpp"
#include "Items/Axe.hpp"
#include "Characters/Mario.hpp"
#include "Characters/Goomba.hpp"
#include "Characters/Koopa.hpp"
#include "Characters/Piranha.hpp"
#include "Characters/Bowser.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp"

class MapManager{
private:
    std::string level;
    std::vector<std::shared_ptr<Block>> blocks_store;
    std::vector<std::shared_ptr<Block>> bridge_store; //<- 1-4 bridge block, not the moving bridge
    std::vector<std::shared_ptr<FireBar>> firebars_store;
    std::vector<std::shared_ptr<Pipe>> pipes_store;
    std::vector<std::shared_ptr<Item>> items_store;
    
    std::vector<std::shared_ptr<Goomba>> goombas_store;
    std::vector<std::shared_ptr<Koopa>> koopas_store;
    std::vector<std::shared_ptr<Piranha>> piranhas_store;
    std::vector<std::shared_ptr<Bowser>> bowser_store;

    std::queue<std::shared_ptr<Fireball>> m_fireballs_store;
    std::queue<std::shared_ptr<Fireball>> b_fireballs_store;
    
    std::vector<CollisionBox> floor_boxes;
    std::shared_ptr<Flag> flag;
    std::shared_ptr<Background> background;
    std::shared_ptr<AnimationObject> castle;
    std::shared_ptr<AnimationObject> castle_flag;

    float barrier;
    bool hasCastle;
    bool hasCastleFlag;
    bool hasFlag;
    
public:
    MapManager(std::string level);

    void SetCurrentLevel(std::string level);

    std::string GetCurrentLevel();

    float GetBarriers();

    std::vector<std::shared_ptr<Block>> GetBlocks();

    std::vector<std::shared_ptr<Pipe>> GetPipes();

    std::shared_ptr<Background> GetBackground();

    std::vector<CollisionBox> GetFloors();

    std::vector<std::shared_ptr<Item>> GetItems();

    std::vector<std::shared_ptr<Goomba>> GetGoombas();

    std::vector<std::shared_ptr<Koopa>> GetKoopas();

    std::vector<std::shared_ptr<Piranha>> GetPiranhas();

    std::queue<std::shared_ptr<Fireball>> GetMFireballs();

    std::vector<std::shared_ptr<FireBar>> GetFirebars();

    std::vector<std::shared_ptr<Bowser>> GetBowser();

    std::shared_ptr<Flag> GetFlag();

    std::shared_ptr<AnimationObject> GetCastleFlag();

    std::vector<std::shared_ptr<Block>> GetCastleBridge();

    void SwitchLevel(std::string new_level);

    //Read the data, construct background, blocks
    void MapDataInitialize();

    //Add all the blocks, background to renderer
    void DrawMap(Util::Renderer& renderer);

    //Delete all the blocks, background, and remove them from renderer
    void ClearMap(Util::Renderer& renderer);

    //Checking if MysteryBlock has spawned item
    //Checking the fireballs
    void UpdateMap(Util::Renderer& renderer, std::shared_ptr<CollisionManager> CManager, std::shared_ptr<Mario> mario);

    void DestroyMarkedObject(Util::Renderer& renderer);

    //When item goes out of range, mark destroy it
    void OutOfRangeMarkDestroy(glm::vec2 cam_pos);
    
    void RemoveBridge(Util::Renderer& renderer);
};

#endif
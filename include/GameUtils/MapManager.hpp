#ifndef MAPMANAGER_HPP
#define MAPMANAGER_HPP

#include "GameUtils/Background.hpp"
#include "GameUtils/CollisionManager.hpp"
#include "Blocks/Brick.hpp"
#include "Blocks/MysteryBlock.hpp"
#include "Blocks/Slab.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp"

//This class will be use to draw map and blocks for the level
//Right now my idea is:
//MapManager will be given a level name(ex: 1_1), it will draw the background of 1_1
//Based on the level, it will also read the responding text file (ex: level_1_1.txt) 
//and create the blocks (class, position, collision box)

//MapManager will probably be also be used for initialize the maps objects to pass to other classes
//like CollisionManager and such

//*TO BE DONE

class MapManager{
private:
    std::string level;
    std::vector<std::shared_ptr<Block>> blocks_store;
    std::vector<std::shared_ptr<CollisionBox>> floor_boxes;
    std::shared_ptr<Background> background;
    float barrier;

public:
    MapManager(std::string level);

    void SetCurrentLevel(std::string level);

    std::string GetCurrentLevel();

    float GetBarriers();

    std::vector<std::shared_ptr<Block>> GetBlocks();

    std::shared_ptr<Background> GetBackground();

    //Read the text file, construct background, blocks
    void MapDataInitialize();

    //Add all the blocks, background to renderer
    void DrawMap(Util::Renderer& renderer);

    //Delete all the blocks, background, and remove them from renderer
    void ClearMap(Util::Renderer& renderer);

    void DestroyMarkedObject(Util::Renderer& renderer);
};

#endif
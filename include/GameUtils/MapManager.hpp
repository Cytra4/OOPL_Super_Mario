#ifndef MAPMANAGER_HPP
#define MAPMANAGER_HPP

#include "GameUtils/Background.hpp"
#include "GameUtils/CollisionManager.hpp"
#include "Blocks/Brick.hpp"
#include "Blocks/MysteryBlock.hpp"
#include "Blocks/Slab.hpp"
#include "Blocks/Pipe.hpp"
#include "Blocks/Flag.hpp"
#include "Util/Renderer.hpp"
#include "pch.hpp"

class MapManager{
private:
    std::string level;
    std::vector<std::shared_ptr<Block>> blocks_store;
    std::vector<std::shared_ptr<Pipe>> pipes_store;
    std::vector<CollisionBox> floor_boxes;
    std::shared_ptr<Flag> flag;
    std::shared_ptr<Background> background;

    float barrier;

public:
    MapManager(std::string level);

    void SetCurrentLevel(std::string level);

    std::string GetCurrentLevel();

    float GetBarriers();

    std::vector<std::shared_ptr<Block>> GetBlocks();

    std::vector<std::shared_ptr<Pipe>> GetPipes();

    std::shared_ptr<Background> GetBackground();

    std::vector<CollisionBox> GetFloors();

    //Read the data, construct background, blocks
    void MapDataInitialize();

    //Add all the blocks, background to renderer
    void DrawMap(Util::Renderer& renderer);

    //Delete all the blocks, background, and remove them from renderer
    void ClearMap(Util::Renderer& renderer);

    void DestroyMarkedObject(Util::Renderer& renderer);
};

#endif
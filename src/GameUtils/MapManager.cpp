#include "GameUtils/MapManager.hpp"
#include "GameUtils/MapDataHolder.hpp"
#include "Util/LoadTextFile.hpp"
#include "Util/Logger.hpp"

MapManager::MapManager(std::string level){
    this->level = level;
}

void MapManager::SetCurrentLevel(std::string level){
    this->level = level;
}

float MapManager::GetBarriers(){
    return barrier;
}

std::vector<std::shared_ptr<Block>> MapManager::GetBlocks(){
    return blocks_store;
}

std::vector<std::shared_ptr<Pipe>> MapManager::GetPipes(){
    return pipes_store;
}

std::shared_ptr<Background> MapManager::GetBackground(){
    return background;
}

std::vector<CollisionBox> MapManager::GetFloors(){
    return floor_boxes;
}

void MapManager::MapDataInitialize(){
    std::vector<std::vector<int>> mapData = MapDataHolder::GetBlockData(level); 
    background = std::make_shared<Background>(RESOURCE_DIR"/Maps/level" + level + ".png");
    std::string choice = std::to_string(level[2] - '0');

    glm::vec2 startingPoint = glm::vec2{-1 * background->GetScaledSize().x/2, background->GetScaledSize().y/2};
    for (int i=0;i<int(mapData.size());i++){
        for (int j=0;j<int(mapData[0].size());j++){
            int block_type = mapData[i][j];
            glm::vec2 block_pos = glm::vec2{startingPoint.x + 48*j + 24, startingPoint.y - 48*i - 24};
            if (block_type == 1){
                std::shared_ptr<MysteryBlock> new_block = std::make_shared<MysteryBlock>(RESOURCE_DIR"/Sprites/Blocks/empty" + choice + ".png", block_pos, 48,48, 1);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 2){
                std::shared_ptr<Brick> new_block = std::make_shared<Brick>(RESOURCE_DIR"/Sprites/Blocks/brick" + choice +".png", block_pos, 48, 48);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 3){
                std::shared_ptr<Slab> new_block = std::make_shared<Slab>(RESOURCE_DIR"/Sprites/Blocks/slab" + choice + ".png", block_pos, 48, 48);
                blocks_store.push_back(new_block);
            }
        }
    }

    std::vector<std::vector<int>> floorData = MapDataHolder::GetFloorData(level);
    for (int i=0;i<int(floorData.size());i++){
        CollisionBox new_box = CollisionBox(glm::vec2{floorData[i][0],floorData[i][1]},floorData[i][2],floorData[i][3]);
        floor_boxes.push_back(new_box);
    }

    std::vector<std::vector<float>> pipeData = MapDataHolder::GetPipeData(level);
    for (int i=0;i<int(pipeData.size());i++){
        std::shared_ptr<Pipe> new_pipe = std::make_shared<Pipe>(false, "0", pipeData[i][0], glm::vec2{pipeData[i][1],pipeData[i][2]});
        pipes_store.push_back(new_pipe);
    }

    glm::vec2 flagData = MapDataHolder::GetFlagPosition(level);
    flag = std::make_shared<Flag>(flagData, 48, 432);
}

void MapManager::DrawMap(Util::Renderer& renderer){
    background->DrawBackground();
    renderer.AddChild(background);

    for (int i=0;i<int(blocks_store.size());i++){
        renderer.AddChild(blocks_store[i]->GetAnimationObject());
    }

    for (int i=0;i<int(pipes_store.size());i++){
        renderer.AddChild(pipes_store[i]->GetAnimationObject());
    }

    renderer.AddChild(flag->GetAnimationObject());
    renderer.AddChild(flag->GetFlagAniObj());
}

void MapManager::ClearMap(Util::Renderer& renderer){
    for (int i=0;i<int(blocks_store.size());i++){
        renderer.RemoveChild(blocks_store[i]->GetAnimationObject());
    }
    renderer.RemoveChild(background);
    blocks_store.clear();
    floor_boxes.clear();
}

void MapManager::DestroyMarkedObject(Util::Renderer& renderer){
    for (int i=0;i<int(blocks_store.size());i++){
        if (blocks_store[i]->IsMarkedDestroy()){
            renderer.RemoveChild(blocks_store[i]->GetAnimationObject());
        }
    }
    blocks_store.erase(std::remove_if(blocks_store.begin(), blocks_store.end(),
        [](const std::shared_ptr<Block>& block) {
            return block->IsMarkedDestroy(); 
        }),
        blocks_store.end());
}
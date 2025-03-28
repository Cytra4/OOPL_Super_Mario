#include "GameUtils/MapManager.hpp"
#include "GameUtils/MapDataHolder.hpp"
#include "Util/LoadTextFile.hpp"
#include "Util/Logger.hpp"
#include <sstream>

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

std::shared_ptr<Background> MapManager::GetBackground(){
    return background;
}

void MapManager::MapDataInitialize(){
    std::vector<std::vector<int>> mapData = MapDataHolder::GetMapData(level); 
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
}

//TO BE USED
// std::string MapData = Util::LoadTextFile(levelPath);
// std::istringstream stream(MapData);
// std::string line;
// while (std::getline(stream, line)) {
//     std::istringstream lineStream(line);
//     std::string type;
//     double pos_x,pos_y,width,height;

//     lineStream >> type >> pos_x >> pos_y;
//     if (type == "Brick"){
//         std::shared_ptr<Brick> new_brick 
//         = std::make_shared<Brick>(RESOURCE_DIR"/Sprites/Blocks/Brick1.png", glm::vec2{pos_x, pos_y}, 48, 48);
//         blocks_store.push_back(new_brick);
//     }
//     else if (type == "C_Brick"){

//     }
//     else if (type == "C_Mystery"){
//         std::shared_ptr<MysteryBlock> new_mystery 
//         = std::make_shared<MysteryBlock>(RESOURCE_DIR"/Sprites/Blocks/empty1.png", glm::vec2{pos_x, pos_y}, 48,48, 1);
//         blocks_store.push_back(new_mystery);
//     }
//     else if (type == "M_Mystery"){

//     }
//     else if (type == "F_Mystery"){

//     }
//     else if (type == "Slab"){

//     }
//     else if (type == "Pipe"){

//     }
//     else if (type == "H_Pipe"){

//     }
//     else{
//         lineStream >> width >> height;
//         std::shared_ptr<CollisionBox> new_floor
//         =std::make_shared<CollisionBox>(glm::vec2{pos_x, pos_y}, width, height);
//         floor_boxes.push_back(new_floor);
//     }
// }

void MapManager::DrawMap(Util::Renderer& renderer){
    background->DrawBackground();
    renderer.AddChild(background);

    for (int i=0;i<int(blocks_store.size());i++){
        renderer.AddChild(blocks_store[i]->GetAnimationObject());
    }

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
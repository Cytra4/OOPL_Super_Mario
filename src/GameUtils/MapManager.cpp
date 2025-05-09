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

std::vector<std::shared_ptr<Item>> MapManager::GetItems(){
    return items_store;
}

std::vector<std::shared_ptr<Goomba>> MapManager::GetGoombas(){
    return goombas_store;
}

std::vector<std::shared_ptr<Koopa>> MapManager::GetKoopas(){
    return koopas_store;
}

std::vector<std::shared_ptr<Piranha>> MapManager::GetPiranhas(){
    return piranhas_store;
}

std::queue<std::shared_ptr<Fireball>> MapManager::GetMFireballs(){
    return m_fireballs_store;
}

std::shared_ptr<Flag> MapManager::GetFlag(){
    return flag;
}

std::shared_ptr<AnimationObject> MapManager::GetCastleFlag(){
    return castle_flag;
}

void MapManager::SwitchLevel(std::string new_level){
    this->level = new_level;
}

void MapManager::MapDataInitialize(){
    std::vector<std::vector<int>> mapData = MapDataHolder::GetBlockData(level); 
    background = std::make_shared<Background>(RESOURCE_DIR"/Maps/level" + level + ".png");
    std::string choice = std::to_string(level[2] - '0');
    if (level == "1_1A"){choice = "2";}
    if (level == "1_2C"){choice = "1";}

    glm::vec2 startingPoint = glm::vec2{-1 * background->GetScaledSize().x/2, background->GetScaledSize().y/2};
    for (int i=0;i<int(mapData.size());i++){
        for (int j=0;j<int(mapData[0].size());j++){
            int block_type = mapData[i][j];
            glm::vec2 block_pos = glm::vec2{startingPoint.x + 48*j + 24, startingPoint.y - 48*i - 24};
            if (block_type == 1){
                //Mystery Block with coin
                std::shared_ptr<MysteryBlock> new_block = std::make_shared<MysteryBlock>("Coin", RESOURCE_DIR"/Sprites/Blocks/empty" + choice + ".png", block_pos, 48,48, 1);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 2){
                //Brick
                std::shared_ptr<Brick> new_block = std::make_shared<Brick>(RESOURCE_DIR"/Sprites/Blocks/brick" + choice +".png", block_pos, 48, 48);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 3){
                //Slab
                std::shared_ptr<Slab> new_block = std::make_shared<Slab>(RESOURCE_DIR"/Sprites/Blocks/slab" + choice + ".png", block_pos, 48, 48);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 4){
                //Mushroom/Flower
                std::shared_ptr<MysteryBlock> new_block = std::make_shared<MysteryBlock>("Mushroom", RESOURCE_DIR"/Sprites/Blocks/empty" + choice + ".png", block_pos, 48,48, 1);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 5){
                //1UP
                std::shared_ptr<MysteryBlock> new_block = std::make_shared<MysteryBlock>("Mushroom2", RESOURCE_DIR"/Sprites/Blocks/empty" + choice + ".png", block_pos, 48,48, 1);
                blocks_store.push_back(new_block);
            }
            else if (block_type == 6){
                //Goomba
                int type;
                if (level == "1_1"){type = 1;}
                else if (level == "1_2"){type = 2;}
                else{type = 3;}
                std::shared_ptr<Goomba> new_goomba = std::make_shared<Goomba>(type, block_pos, 48, 48);
                goombas_store.push_back(new_goomba);
            }
            else if (block_type == 7){
                //Koopa
                int type;
                if (level == "1_1"){type = 1;}
                else{type = 2;}
                std::shared_ptr<Koopa> new_koopa = std::make_shared<Koopa>(type, block_pos, 35, 69);
                koopas_store.push_back(new_koopa);
            }
            else if (block_type == 8){
                //Floating Coins
                std::shared_ptr<FCoin> new_coin = std::make_shared<FCoin>(choice, RESOURCE_DIR"/Sprites/Blocks/coin" + choice + "_" + "1.png",
                    block_pos,48,48);
                items_store.push_back(new_coin);
            }
            else if (block_type == 9){
                //Bridge moving down
                std::shared_ptr<MovingBridge> new_bridge = std::make_shared<MovingBridge>(RESOURCE_DIR"/Sprites/Blocks/bridge1.png",
                    block_pos,144,24,0,380);
                blocks_store.push_back(new_bridge);
            }
            else if (block_type == 10){
                //Bridge moving up
                std::shared_ptr<MovingBridge> new_bridge = std::make_shared<MovingBridge>(RESOURCE_DIR"/Sprites/Blocks/bridge1.png",
                    block_pos,144,24,1,380);
                blocks_store.push_back(new_bridge);
            }
        }
    }

    std::vector<std::vector<int>> floorData = MapDataHolder::GetFloorData(level);
    for (int i=0;i<int(floorData.size());i++){
        CollisionBox new_box = CollisionBox(glm::vec2{floorData[i][0],floorData[i][1]},floorData[i][2],floorData[i][3]);
        floor_boxes.push_back(new_box);
    }

    std::vector<std::vector<float>> pipeData = MapDataHolder::GetPipeData(level);
    if (pipeData[0][0] != 0){
        for (int i=0;i<int(pipeData.size());i++){
            std::shared_ptr<Pipe> new_pipe = std::make_shared<Pipe>(false, "0", pipeData[i][0], glm::vec2{pipeData[i][1],pipeData[i][2]});
            new_pipe->GetAnimationObject()->SetZIndex(60);
            pipes_store.push_back(new_pipe);
        }
    }

    std::string secret_level;
    std::vector<std::vector<double>> hiddenPipeData = MapDataHolder::GetHiddenPipeData(level);
    for (int i=0;i<int(hiddenPipeData.size());i++){
        if (hiddenPipeData[i][0] != 0){
            if (level == "1_1"){
                secret_level = "1_1A";
            }
            else if (level == "1_1A"){
                secret_level = "1_1";
            }
            else if (level == "1_2"){
                if (i == 0){secret_level = "1_2B";}
                else if (i == 1){secret_level = "1_2C";}
                else{secret_level = "1_1";}
            }
            else if (level == "1_2B"){
                secret_level = "1_2";
            }
            else{
                secret_level = "None";
            }
            std::shared_ptr<Pipe> new_hidden_pipe = std::make_shared<Pipe>(true, secret_level, hiddenPipeData[i][0], 
                glm::vec2{hiddenPipeData[i][1],hiddenPipeData[i][2]});
            new_hidden_pipe->GetAnimationObject()->SetZIndex(60);
            pipes_store.push_back(new_hidden_pipe);
        }
    }

    glm::vec2 flagData = MapDataHolder::GetFlagPosition(level);
    if (flagData != glm::vec2{0,0}){
        flag = std::make_shared<Flag>(flagData, 5, 432);
        hasFlag = true;
    }

    glm::vec2 castleData = MapDataHolder::GetCastlePosition(level);
    if (castleData != glm::vec2{0,0}){
        castle = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Blocks/castle.png", castleData);
        hasCastle = true;
    }

    glm::vec2 castleflagData = MapDataHolder::GetCastleFlagPosition(level);
    if (castleflagData != glm::vec2{0,0}){
        castle_flag = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Blocks/castle_flag.png",castleflagData);
        castle_flag->SetZIndex(castle->GetZIndex()-1);
        hasCastleFlag = true;
    }

    std::vector<std::vector<int>> piranhaData = MapDataHolder::GetPiranhaSetting(level);
    if (piranhaData[0][0] != 0){
        int type;
        if (level == "1_2"){type = 2;}
        else{type = 1;}
        
        for (int i=0;i<int(piranhaData.size());i++){
            glm::vec2 pos = glm::vec2{piranhaData[i][1],piranhaData[i][2]};
            glm::vec2 top_pos = pos + glm::vec2{0,48*piranhaData[i][0] + 35};
            std::shared_ptr<Piranha> new_piranha = std::make_shared<Piranha>(type, pos, top_pos, 48, 55);
            piranhas_store.push_back(new_piranha);
        }
    }
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

    for (int i=0;i<int(items_store.size());i++){
        renderer.AddChild(items_store[i]->GetAnimationObject());
    }

    for (int i=0;i<int(goombas_store.size());i++){
        renderer.AddChild(goombas_store[i]->GetAnimationObject());
    }

    for (int i=0;i<int(koopas_store.size());i++){
        renderer.AddChild(koopas_store[i]->GetAnimationObject());
    }

    for (int i=0;i<int(piranhas_store.size());i++){
        renderer.AddChild(piranhas_store[i]->GetAnimationObject());
    }

    if (hasFlag){
        renderer.AddChild(flag->GetAnimationObject());
        renderer.AddChild(flag->GetFlagAniObj());
    }

    if (hasCastle){
        renderer.AddChild(castle);
    }

    if (hasCastleFlag){
        renderer.AddChild(castle_flag);
    }
}

void MapManager::ClearMap(Util::Renderer& renderer){
    for (int i=0;i<int(blocks_store.size());i++){
        renderer.RemoveChild(blocks_store[i]->GetAnimationObject());
    }
    for (int i=0;i<int(pipes_store.size());i++){
        renderer.RemoveChild(pipes_store[i]->GetAnimationObject());
    }
    for (int i=0;i<int(items_store.size());i++){
        renderer.RemoveChild(items_store[i]->GetAnimationObject());
    }
    for (int i=0;i<int(goombas_store.size());i++){
        renderer.RemoveChild(goombas_store[i]->GetAnimationObject());
    }
    for (int i=0;i<int(koopas_store.size());i++){
        renderer.RemoveChild(koopas_store[i]->GetAnimationObject());
    }
    for (int i=0;i<int(piranhas_store.size());i++){
        renderer.RemoveChild(piranhas_store[i]->GetAnimationObject());
    }

    int fireballs_size = m_fireballs_store.size();
    for (int i = 0; i < fireballs_size; i++) {
        auto fb = m_fireballs_store.front();
        m_fireballs_store.pop();
        renderer.RemoveChild(fb->GetAnimationObject());
    }

    renderer.RemoveChild(background);
    if (hasFlag){
        renderer.RemoveChild(flag->GetAnimationObject());
        renderer.RemoveChild(flag->GetFlagAniObj());
    }

    if (hasCastle){
        renderer.RemoveChild(castle);
    }
    
    if (hasCastleFlag){
        renderer.RemoveChild(castle_flag);
    }

    items_store.clear();
    blocks_store.clear();
    pipes_store.clear();
    floor_boxes.clear();
    goombas_store.clear();
    koopas_store.clear();
    piranhas_store.clear();
    if (hasFlag){
        flag->GetBox().SetActive(false);
        flag->GetFlagAniObj()->SetVisible(false);
        flag->GetAnimationObject()->SetVisible(false);   
    }
}

void MapManager::UpdateMap(Util::Renderer& renderer, std::shared_ptr<CollisionManager> CManager, std::shared_ptr<Mario> mario){
    for (int i=0;i<int(blocks_store.size());i++){
        if (blocks_store[i]->HasItem()){
            renderer.AddChild(blocks_store[i]->GetItem()->GetAnimationObject());
            CManager->AddItem(blocks_store[i]->GetItem());
            items_store.push_back(blocks_store[i]->GetItem());
            blocks_store[i]->SetHasItem(false);
        }
    }

    auto m_fireballs = mario->GetFireballs();
    for (auto fb : m_fireballs){
        m_fireballs_store.push(fb);
        renderer.AddChild(fb->GetAnimationObject());
    }

    CManager->SetFireballs(m_fireballs_store);
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

    for (int i=0;i<int(items_store.size());i++){
        if (items_store[i]->IsMarkedDestroy()){
            renderer.RemoveChild(items_store[i]->GetAnimationObject());
        }
    }
    items_store.erase(std::remove_if(items_store.begin(), items_store.end(),
        [](const std::shared_ptr<Item>& item) {
            return item->IsMarkedDestroy(); 
        }),
        items_store.end());

    for (int i=0;i<int(goombas_store.size());i++){
        if (goombas_store[i]->IsMarkedDestroy()){
            renderer.RemoveChild(goombas_store[i]->GetAnimationObject());
        }
    }
    goombas_store.erase(std::remove_if(goombas_store.begin(), goombas_store.end(),
        [](const std::shared_ptr<Goomba>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        goombas_store.end());

    for (int i=0;i<int(koopas_store.size());i++){
        if (koopas_store[i]->IsMarkedDestroy()){
            renderer.RemoveChild(koopas_store[i]->GetAnimationObject());
        }
    }
    koopas_store.erase(std::remove_if(koopas_store.begin(), koopas_store.end(),
        [](const std::shared_ptr<Koopa>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        koopas_store.end());

    for (int i=0;i<int(piranhas_store.size());i++){
        if (piranhas_store[i]->IsMarkedDestroy()){
            renderer.RemoveChild(piranhas_store[i]->GetAnimationObject());
        }
    }
    piranhas_store.erase(std::remove_if(piranhas_store.begin(), piranhas_store.end(),
        [](const std::shared_ptr<Piranha>& enemy) {
            return enemy->IsMarkedDestroy(); 
        }),
        piranhas_store.end());

    int fireballs_size = m_fireballs_store.size();
    for (int i = 0; i < fireballs_size; i++) {
        auto fb = m_fireballs_store.front();
        m_fireballs_store.pop();
        if (fb->IsMarkedRemove()) {
            renderer.RemoveChild(fb->GetAnimationObject());
            continue;
        }
        m_fireballs_store.push(fb);
    }
}

void MapManager::OutOfRangeMarkDestroy(glm::vec2 cam_pos){
    for (int i=0;i<int(items_store.size());i++){
        bool Xoverlap = false;
        bool Yoverlap = false;
        glm::vec2 pos = items_store[i]->GetAnimationObject()->GetPosition();
        Xoverlap = cam_pos.x - 450 >= pos.x || 
                cam_pos.x + 450 <= pos.x;
        Yoverlap = cam_pos.y - 360 >= pos.y ||
                cam_pos.y + 360 <= pos.y;
        if ((Xoverlap || Yoverlap) && items_store[i]->IsReachedTop()){
            items_store[i]->MarkDestroy();
        }
    }

    for (int i=0;i<int(koopas_store.size());i++){
        bool Xoverlap = false;
        bool Yoverlap = false;
        glm::vec2 pos = koopas_store[i]->GetAnimationObject()->GetPosition();
        Xoverlap = cam_pos.x - 500 >= pos.x || 
                cam_pos.x + 500 <= pos.x;
        Yoverlap = cam_pos.y - 360 >= pos.y ||
                cam_pos.y + 360 <= pos.y;
        if (Xoverlap || Yoverlap){
            if (koopas_store[i]->GetDeath() != 0){
                koopas_store[i]->MarkDestroy();
            }
        }
    }
}
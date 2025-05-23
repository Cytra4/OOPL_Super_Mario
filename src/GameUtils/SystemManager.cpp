#include "GameUtils/SystemManager.hpp"
#include "Util/Logger.hpp"

SystemManager::SystemManager(){
    VariableInit();
    ScreenInit();
    UIInit();
    OtherTextInit();

    sys_SE = std::make_shared<Util::SFX>(RESOURCE_DIR"/SoundEffects/System/beep.wav");
    sys_BGM = std::make_shared<Util::BGM>(RESOURCE_DIR"/BGM/GroundTheme.mp3");
}

void SystemManager::VariableInit(){
    mario_score = 0;
    mario_score_t = "0000000";
    mario_coins = 0;
    mario_coins_t = "00";
    mario_lives = 3;
    mario_lives_t = "3";
    game_time = 300;
    game_time_t = "300";
    current_level = "1_1";
    game_over = false;
}

void SystemManager::ScreenInit(){
    title_screen = std::make_shared<AnimationObject>(RESOURCE_DIR"/Maps/start_screen.png",glm::vec2{0,0});
    title_screen->SetZIndex(65);
    loading_screen = std::make_shared<AnimationObject>(RESOURCE_DIR"/Maps/loading_screen.png",glm::vec2{0,0});
    loading_screen->SetZIndex(65);
    gameover_screen = std::make_shared<AnimationObject>(RESOURCE_DIR"/Maps/gameover_screen.png",glm::vec2{0,0});
    gameover_screen->SetZIndex(65);
    timesup_screen = std::make_shared<AnimationObject>(RESOURCE_DIR"/Maps/timeup_screen.png",glm::vec2{0,0});
    timesup_screen->SetZIndex(65);
}

void SystemManager::UIInit(){
    UI_format = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Other/mario_ui.png",glm::vec2{0,288});
    UI_format->SetZIndex(70);

    glm::vec2 w_pos = {-324,276};
    std::shared_ptr<AnimationObject> num;
    for (int i=0;i<7;i++){
        num = std::make_shared<AnimationObject>(ConvertNumberToPath("0"),w_pos);
        num->SetZIndex(70);
        mario_score_display.push_back(num);
        w_pos.x += 24;
    }

    w_pos.x += 48;
    std::vector<std::string> coin_anim;
    for (int i=1;i<4;i++){
        coin_anim.push_back(RESOURCE_DIR"/Sprites/Other/ui_coin" + std::to_string(i) + ".png");
    }
    coin = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Other/ui_coin1.png",w_pos);
    coin->SetZIndex(70);
    coin->AddNewAnimation(coin_anim);
    coin->SetAnimation(0,200);
    coin->SetLooping(true);
    coin->PlayAnimation();
    coin->SetCurrentAnimation(0);

    w_pos.x += 48;
    num = std::make_shared<AnimationObject>(ConvertNumberToPath("0"),w_pos);
    num->SetZIndex(70);
    coin_amount_display.push_back(num);
    w_pos.x += 24;
    num = std::make_shared<AnimationObject>(ConvertNumberToPath("0"),w_pos);
    num->SetZIndex(70);
    coin_amount_display.push_back(num);

    w_pos.x += 120;
    num = std::make_shared<AnimationObject>(ConvertNumberToPath("1"),w_pos);
    num->SetZIndex(70);
    level_display.push_back(num);
    w_pos.x += 48;
    num = std::make_shared<AnimationObject>(ConvertNumberToPath("1"),w_pos);
    num->SetZIndex(70);
    level_display.push_back(num);

    w_pos.x += 120;
    for (int i=0;i<3;i++){
        num = std::make_shared<AnimationObject>(ConvertNumberToPath("0"),w_pos);
        num->SetZIndex(70);
        remain_time_display.push_back(num);
        w_pos.x += 24;
    }
}

void SystemManager::OtherTextInit(){
    glm::vec2 t_pos = {12,-204};
    std::shared_ptr<AnimationObject> num;
    for (int i=0;i<7;i++){
        num = std::make_shared<AnimationObject>(ConvertNumberToPath("0"),t_pos);
        num->SetZIndex(70);
        top_score_display.push_back(num);
        t_pos.x += 24;
    }

    t_pos = {36,108};
    num = std::make_shared<AnimationObject>(ConvertNumberToPath("1"),t_pos);
    num->SetZIndex(70);
    level_display2.push_back(num);
    t_pos.x += 48;
    num = std::make_shared<AnimationObject>(ConvertNumberToPath("1"),t_pos);
    num->SetZIndex(70);
    level_display2.push_back(num);

    t_pos = {60,12};
    mario_lives_display = std::make_shared<AnimationObject>(ConvertNumberToPath("3"),t_pos);
    mario_lives_display->SetZIndex(70);

    t_pos = {3456,96};
    num = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Other/clear_message1.png",t_pos);
    num->SetZIndex(70);
    game_clear_message_display.push_back(num);
    t_pos.y -= 108;
    num = std::make_shared<AnimationObject>(RESOURCE_DIR"/Sprites/Other/clear_message2.png",t_pos);
    game_clear_message_display.push_back(num);
    num->SetZIndex(70);
}

void SystemManager::ResetMVariables(){
    mario_score = 0;
    mario_score_t = "0000000";
    mario_coins = 0;
    mario_coins_t = "00";
    mario_lives = 3;
    mario_lives_t = "3";

    game_over = false;
}

void SystemManager::ResetTimer(){
    game_time = 300;
    game_time_t = "300";
    time_up = false;
}

void SystemManager::PlayBeep(){
    sys_SE->LoadMedia(RESOURCE_DIR"/SoundEffects/System/beep.wav");
    sys_SE->SetVolume(30);
    sys_SE->Play();
}

void SystemManager::PlayGameOver(){
    sys_SE->LoadMedia(RESOURCE_DIR"/SoundEffects/System/gameover.wav");
    sys_SE->SetVolume(80);
    sys_SE->Play();
}

void SystemManager::PlayBGM(){
    if (current_level == "1_2A"){
        return;
    }

    if (current_level == "1_1" || current_level == "1_2C"){
        sys_BGM->LoadMedia(RESOURCE_DIR"/BGM/GroundTheme.mp3");
        sys_BGM->SetVolume(80);
        sys_BGM->Play();
    }
    else if (current_level == "1_1A" || current_level == "1_2" || current_level == "1_2B"){
        sys_BGM->LoadMedia(RESOURCE_DIR"/BGM/UndergroundTheme.mp3");
        sys_BGM->SetVolume(80);
        sys_BGM->Play();
    }
    else if (current_level == "1_4"){
        sys_BGM->LoadMedia(RESOURCE_DIR"/BGM/CastleTheme.mp3");
        sys_BGM->SetVolume(80);
        sys_BGM->Play();
    }
}

void SystemManager::PauseBGM(){
    sys_BGM->Pause();
}

void SystemManager::PlayComplete(){
    if (current_level != "1_4"){
        sys_BGM->LoadMedia(RESOURCE_DIR"/BGM/LevelComplete.mp3");
    }
    else{
        sys_BGM->LoadMedia(RESOURCE_DIR"/BGM/CastleComplete.mp3");
    }
    sys_BGM->SetVolume(80);
    sys_BGM->Play(0);
}

void SystemManager::ShowTitleScreen(Util::Renderer& renderer){
    if (!title_drawn){
        renderer.AddChild(title_screen);
        for (int i=0;i<int(top_score_display.size());i++){
            renderer.AddChild(top_score_display[i]);
        }
        title_drawn = true;
    }
    else{
        title_screen->SetVisible(true);
    }
}

void SystemManager::ClearTitleScreen(Util::Renderer& renderer){
    if (title_drawn){
        renderer.RemoveChild(title_screen);
        for (int i=0;i<int(top_score_display.size());i++){
            renderer.RemoveChild(top_score_display[i]);
        }
        title_drawn = false;
    }
}

void SystemManager::ShowLoadingScreen(Util::Renderer& renderer){
    if (!loading_drawn){
        renderer.AddChild(loading_screen);
        for (int i=0;i<int(level_display2.size());i++){
            renderer.AddChild(level_display2[i]);
        }
        renderer.AddChild(mario_lives_display);
        loading_drawn = true;
    }
}

void SystemManager::ClearLoadingScreen(Util::Renderer& renderer){
    if (loading_drawn){
        renderer.RemoveChild(loading_screen);
        for (int i=0;i<int(level_display2.size());i++){
            renderer.RemoveChild(level_display2[i]);
        }
        renderer.RemoveChild(mario_lives_display);
        loading_drawn = false;
    }
}

void SystemManager::ShowGameOverScreen(Util::Renderer& renderer){
    if (!gameover_drawn){
        renderer.AddChild(gameover_screen);
        gameover_drawn = true;
    }
}

void SystemManager::ClearGameOverScreen(Util::Renderer& renderer){
    if (gameover_drawn){
        renderer.RemoveChild(gameover_screen);
        gameover_drawn = false;
    }
}

void SystemManager::ShowTimeUpScreen(Util::Renderer& renderer){
    if (!timeup_drawn){
        renderer.AddChild(timesup_screen);
        timeup_drawn = true;
    }
}

void SystemManager::ClearTimeUpScreen(Util::Renderer& renderer){
    if (timeup_drawn){
        renderer.RemoveChild(timesup_screen);
        timeup_drawn = false;
    }
}

void SystemManager::ShowGameClearMessage(Util::Renderer& renderer, int index){
    if (message_drawn < index){
        message_drawn = index;
        renderer.AddChild(game_clear_message_display[index]);
    }
}
    
void SystemManager::ClearGameClearMessage(Util::Renderer& renderer){
    renderer.RemoveChild(game_clear_message_display[0]);
    renderer.RemoveChild(game_clear_message_display[1]);
    message_drawn = -1;
}

void SystemManager::ShowUI(Util::Renderer& renderer){
    if (!ui_drawn){
        renderer.AddChild(UI_format);
        renderer.AddChild(coin);
        for (int i=0;i<int(mario_score_display.size());i++){
            renderer.AddChild(mario_score_display[i]);
        }
        for (int i=0;i<int(coin_amount_display.size());i++){
            renderer.AddChild(coin_amount_display[i]);
        }
        for (int i=0;i<int(level_display.size());i++){
            renderer.AddChild(level_display[i]);
        }
        for (int i=0;i<int(remain_time_display.size());i++){
            renderer.AddChild(remain_time_display[i]);
        }
        ui_drawn = true;
    }
}

void SystemManager::ClearUI(Util::Renderer& renderer){
    if (ui_drawn){
        renderer.RemoveChild(UI_format);
        renderer.RemoveChild(coin);
        for (int i=0;i<int(mario_score_display.size());i++){
            renderer.RemoveChild(mario_score_display[i]);
        }
        for (int i=0;i<int(coin_amount_display.size());i++){
            renderer.RemoveChild(coin_amount_display[i]);
        }
        for (int i=0;i<int(level_display.size());i++){
            renderer.RemoveChild(level_display[i]);
        }
        for (int i=0;i<int(remain_time_display.size());i++){
            renderer.RemoveChild(remain_time_display[i]);
        }
        ui_drawn = false;
    }
}

void SystemManager::UIUpdate(int new_score, int new_coin, double time, std::string new_level){
    MScoreUpdate(new_score);
    MCoinUpdate(new_coin);
    TimerUpdate(time);
    levelUpdate(new_level);
}

void SystemManager::MScoreUpdate(int new_score){
    mario_score = new_score;
    std::string new_t = std::to_string(new_score);
    int length = 7-new_t.length();
    for (int i=0;i<length;i++){
        new_t = "0" + new_t;
    }

    for (int i=0;i<int(new_t.length());i++){
        std::string t(1,new_t[i]);
        mario_score_display[i]->SetDefaultSprite(ConvertNumberToPath(t));
    }
    mario_score_t = new_t;
}

void SystemManager::MCoinUpdate(int new_coin){
    mario_coins = new_coin;
    std::string new_t = std::to_string(new_coin);
    int length = 2-new_t.length();
    for (int i=0;i<length;i++){
        new_t = "0" + new_t;
    }

    for (int i=0;i<int(new_t.length());i++){
        std::string t(1,new_t[i]);
        coin_amount_display[i]->SetDefaultSprite(ConvertNumberToPath(t));
    }
    mario_coins_t = new_t;
}

void SystemManager::TimerUpdate(double time){
    if (game_time > 0){
        game_time -= time;
        std::string new_t = std::to_string(int(game_time));
        int length = 3-new_t.length();
        for (int i=0;i<length;i++){
            new_t = "0" + new_t;
        }

        for (int i=0;i<int(new_t.length());i++){
            std::string t(1,new_t[i]);
            remain_time_display[i]->SetDefaultSprite(ConvertNumberToPath(t));
        }
        game_time_t = new_t;

        if (game_time <= 0){
            time_up = true;
        }
    }
}

void SystemManager::TopScoreUpdate(int new_top){
    if (new_top > top_score){
        top_score = new_top;
        std::string new_t = std::to_string(new_top);
        int length = 7-new_t.length();
        for (int i=0;i<length;i++){
            new_t = "0" + new_t;
        }

        for (int i=0;i<int(new_t.length());i++){
            std::string t(1,new_t[i]);
            top_score_display[i]->SetDefaultSprite(ConvertNumberToPath(t));
        }
        top_score_t = new_t;
    }
}

void SystemManager::levelUpdate(std::string new_level){
    current_level = new_level;
    std::string first(1,new_level[0]);
    std::string second(1,new_level[2]);
    if (second == "4"){second = "3";}
    
    level_display[0]->SetDefaultSprite(ConvertNumberToPath(first));
    level_display[1]->SetDefaultSprite(ConvertNumberToPath(second));

    level_display2[0]->SetDefaultSprite(ConvertNumberToPath(first));
    level_display2[1]->SetDefaultSprite(ConvertNumberToPath(second));
}

void SystemManager::MLiveUpdate(int new_live){
    mario_lives = new_live;
    if (mario_lives <= 0){
        mario_lives = 0;
        game_over = true;
        mario_lives_display->SetDefaultSprite(ConvertNumberToPath("3"));
    }
    else{
        std::string t = std::to_string(mario_lives);
        mario_lives_display->SetDefaultSprite(ConvertNumberToPath(t));
    }
}

void SystemManager::UIPositionUpdate(glm::vec2 CameraPosition){
    UI_format->SetPosition(glm::vec2{CameraPosition.x, 288});

    glm::vec2 s_pos = {CameraPosition.x - 324,276};
    for (int i=0;i<int(mario_score_display.size());i++){
        mario_score_display[i]->SetPosition(s_pos);
        s_pos.x += 24;
    }
    s_pos.x += 48;
    coin->SetPosition(s_pos);

    s_pos.x += 48;
    coin_amount_display[0]->SetPosition(s_pos);
    s_pos.x += 24;
    coin_amount_display[1]->SetPosition(s_pos);

    s_pos.x += 120;
    level_display[0]->SetPosition(s_pos);
    s_pos.x += 48;
    level_display[1]->SetPosition(s_pos);

    s_pos.x += 120;
    for (int i=0;i<3;i++){
        remain_time_display[i]->SetPosition(s_pos);
        s_pos.x += 24;
    }
}

void SystemManager::HideTimer(){
    for (int i=0;i<int(remain_time_display.size());i++){
        remain_time_display[i]->SetVisible(false);
    }
}

void SystemManager::ShowTimer(){
    for (int i=0;i<int(remain_time_display.size());i++){
        remain_time_display[i]->SetVisible(true);
    }
}

double SystemManager::GetTime(){
    return game_time;
}

int SystemManager::GetMScore(){
    return mario_score;
}

int SystemManager::GetMCoin(){
    return mario_coins;
}

int SystemManager::GetMLive(){
    return mario_lives;
}

bool SystemManager::IsTimeUp(){
    return time_up;
}

std::string SystemManager::ConvertNumberToPath(std::string number){
    if (number == "0"){return RESOURCE_DIR"/Sprites/Number/n0.png";}
    else if (number == "1"){return RESOURCE_DIR"/Sprites/Number/n1.png";}
    else if (number == "2"){return RESOURCE_DIR"/Sprites/Number/n2.png";}
    else if (number == "3"){return RESOURCE_DIR"/Sprites/Number/n3.png";}
    else if (number == "4"){return RESOURCE_DIR"/Sprites/Number/n4.png";}
    else if (number == "5"){return RESOURCE_DIR"/Sprites/Number/n5.png";}
    else if (number == "6"){return RESOURCE_DIR"/Sprites/Number/n6.png";}
    else if (number == "7"){return RESOURCE_DIR"/Sprites/Number/n7.png";}
    else if (number == "8"){return RESOURCE_DIR"/Sprites/Number/n8.png";}
    else if (number == "9"){return RESOURCE_DIR"/Sprites/Number/n9.png";}
    else{
        LOG_ERROR("NOT A NUMBER");
        return RESOURCE_DIR"/Sprites/Number/n0.png";
    }
}
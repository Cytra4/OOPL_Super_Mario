#ifndef SYSTEMMANAGER_HPP
#define SYSTEMMANAGER_HPP

#include "Characters/Mario.hpp"
#include "GameUtils/AnimationObject.hpp"
#include "Util/Renderer.hpp"

//This manager will manage system related variables, 
//ex: mario's score, coins, current level...
//Also manages the related texts and screens
//ex: UI, game over and times up screens

class SystemManager{
private:
    std::shared_ptr<Util::SFX> sys_SE;

    int top_score = 0;
    std::string top_score_t = "0000000";
    int mario_score;
    std::string mario_score_t;
    int mario_coins;
    std::string mario_coins_t;
    int mario_lives;
    std::string mario_lives_t;
    double game_time;
    std::string game_time_t;
    std::string current_level;

    bool game_over = false;
    bool time_up = false;

    bool title_drawn = false;
    bool loading_drawn = false;
    bool gameover_drawn = false;
    bool timeup_drawn = false;
    bool ui_drawn = false;
    int message_drawn = -1;

    std::shared_ptr<AnimationObject> title_screen;
    std::shared_ptr<AnimationObject> loading_screen;
    std::shared_ptr<AnimationObject> gameover_screen;
    std::shared_ptr<AnimationObject> timesup_screen;
    std::shared_ptr<AnimationObject> UI_format;
    std::shared_ptr<AnimationObject> coin;
    std::shared_ptr<AnimationObject> mario_lives_display;

    std::vector<std::shared_ptr<AnimationObject>> top_score_display;
    std::vector<std::shared_ptr<AnimationObject>> mario_score_display;
    std::vector<std::shared_ptr<AnimationObject>> remain_time_display;
    std::vector<std::shared_ptr<AnimationObject>> coin_amount_display;
    std::vector<std::shared_ptr<AnimationObject>> level_display; //<- UI
    std::vector<std::shared_ptr<AnimationObject>> level_display2; //<- Loading
    std::vector<std::shared_ptr<AnimationObject>> game_clear_message_display;

public:
    SystemManager();

    void VariableInit();

    void ScreenInit();

    void UIInit();

    void OtherTextInit();

    void ResetMVariables();

    void ResetTimer();

    void PlayBeep();

    void PlayGameOver();

    void ShowTitleScreen(Util::Renderer& renderer);

    void ClearTitleScreen(Util::Renderer& renderer);

    void ShowLoadingScreen(Util::Renderer& renderer);

    void ClearLoadingScreen(Util::Renderer& renderer);

    void ShowGameOverScreen(Util::Renderer& renderer);

    void ClearGameOverScreen(Util::Renderer& renderer);

    void ShowTimeUpScreen(Util::Renderer& renderer);

    void ClearTimeUpScreen(Util::Renderer& renderer);

    void ShowGameClearMessage(Util::Renderer& renderer, int index);
    
    void ClearGameClearMessage(Util::Renderer& renderer);

    void ShowUI(Util::Renderer& renderer);

    void ClearUI(Util::Renderer& renderer);

    //Update variables, include switching pictures
    void UIUpdate(int new_score, int new_coin, double time, std::string new_level);

    void MScoreUpdate(int new_score);

    void MCoinUpdate(int new_coin);

    void TimerUpdate(double time);

    void TopScoreUpdate(int new_top);

    void levelUpdate(std::string new_level);

    void MLiveUpdate(int new_live);

    void UIPositionUpdate(glm::vec2 CameraPosition);

    void HideTimer();

    void ShowTimer();

    double GetTime();

    int GetMScore();

    int GetMCoin();

    int GetMLive();

    bool IsTimeUp();

    std::string ConvertNumberToPath(std::string number);
};

#endif
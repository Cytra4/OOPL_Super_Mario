#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Characters/Mario.hpp"
#include "GameUtils/Background.hpp"
#include "GameUtils/CollisionManager.hpp"
#include "GameUtils/MapManager.hpp"
#include "GameUtils/SystemManager.hpp"
#include "GameUtils/MapDataHolder.hpp"

class App {
public:
    enum class State {
        TITLE_SCREEN,
        START,
        LOADING,
        UPDATE,
        MARIO_DEATH,
        TIME_UP,
        PIPE_ENTER,
        LEVEL_CLEAR,
        CLEAR_WALK_TO_CASTLE,
        ADD_REMAIN_TIME_TO_SCORE,
        LEVEL2_ANIMATION,
        BRIDGE_CLEAR,
        GAME_CLEAR,
        GAME_OVER,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void TitleScreen();

    void Start();

    void Loading(double time);

    void Update();

    void MarioDeath(double time);

    void TimeUp(double time);

    void GameOver(double time);

    void LevelClear(double time);

    void ClearWalkToCastle(double time);

    void AddTimeToScore(double time);

    void PipeEnter(double time, std::string type);

    void Level2Animation(double time);

    void DeleteBridge(double time);

    void GameClear(double time);

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void MarioInitial();

    void CamPosAdjust();

    std::string GetLevel();

    std::shared_ptr<CollisionManager> GetCManager();
    
private:
    State m_CurrentState = State::TITLE_SCREEN;
    Util::Renderer m_Renderer;
    Util::Time m_Time;
    glm::vec2 CameraPosition = {0,0};
    bool camPassedFlag = false;
    bool flagDowned = false;

    bool m_EnterDown = false;
    bool rendered = false;
    bool m_initialed = false;
    bool s_initialed = false;
    bool mario_initial = false;
    bool played1_2 = false;

    std::shared_ptr<Mario> mario;
    
    double m_death_timer = 0.5f;
    double pipe_enter_timer = 0.5f;
    double bridge_delete_timer = 0.1f;
    double load_timer = 4.0f;
    double game_over_timer = 5.0f;
    double game_clear_timer = 9.0f;

    std::string prev_level = "0";
    std::string level = "1_1";
    std::shared_ptr<CollisionManager> CManager;
    std::shared_ptr<MapManager> MManager;
    std::shared_ptr<SystemManager> SManager;
};

//*The classes that should/might be added:
//SystemManager:
//1.Manage Mario's score
//2.Manage Mario's live
//3.Manage the time left in game
//4.Manage the texts

#endif

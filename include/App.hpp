#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"
#include "Characters/Mario.hpp"
#include "GameUtils/Background.hpp"
#include "GameUtils/CollisionManager.hpp"
#include "GameUtils/MapManager.hpp"
#include "GameUtils/MapDataHolder.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        MARIO_DEATH,
        PIPE_ENTER,
        LEVEL_CLEAR,
        CLEAR_WALK_TO_CASTLE,
        GET_NEXT_STAGE,
        LEVEL2_ANIMATION,
        BRIDGE_CLEAR,
        GAME_CLEAR,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void MarioDeath(double time);

    void LevelClear(double time);

    void ClearWalkToCastle(double time);

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
    State m_CurrentState = State::START;
    Util::Renderer m_Renderer;
    Util::Time m_Time;
    glm::vec2 CameraPosition;
    bool camPassedFlag = false;
    bool flagDowned = false;

    bool m_EnterDown = false;
    bool rendered = false;
    bool initialed = false;

    bool mario_initial = false;
    std::shared_ptr<Mario> mario;
    
    double m_death_timer = 0.5f;
    double pipe_enter_timer = 0.5f;
    double bridge_delete_timer = 0.1f;

    std::string prev_level = "0";
    std::string level = "1_1";
    std::shared_ptr<CollisionManager> CManager;
    std::shared_ptr<MapManager> MManager; 
};

//*The classes that should/might be added:
//SystemManager:
//1.Manage Mario's score
//2.Manage Mario's live
//3.Manage the time left in game
//4.Manage the texts

#endif

#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"
#include "Characters/Mario.hpp"
#include "GameUtils/Background.hpp"
#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"

//*Screen scale can be changed in transform.hpp
class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void MarioInitial();

    void CamPosAdjust();

    void FireballUpdate();
    
private:
    State m_CurrentState = State::START;
    Util::Renderer m_Renderer;
    Util::Time m_Time;
    glm::vec2 CameraPosition;
    bool m_EnterDown = false;
    std::shared_ptr<Mario> mario;
    std::shared_ptr<Background> background;
    //Use for storing all the fireballs for update
    std::queue<std::shared_ptr<Fireball>> fireballStore;

    //For testing
    CollisionBox testFloor;
    float barrier;
};

//*The classes that should/might be added:
//MapManager: 
//1.Use to store all the map related stuffs: Blocks, background, barrier...
//2.Drawing the map
//MobManager: 
//1.Use to store all the mob related stuffs: items(mushrooms,fireflowers,stars), enemies, fireballs(that mario throws?), mario?
//2.Handle every mob's Behavior() and PhysicProcess()
//3.Dealing with mob's collision
//SystemManager:
//1.Manage Mario's score
//2.Manage Mario's live
//3.Manage the time left in game
//4.Update the game's phases?
//CollisionEventManager:
//1.Deal with all the collisions

#endif

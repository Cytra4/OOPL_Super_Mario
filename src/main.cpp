#include "Core/Context.hpp"
#include "App.hpp"

int main(int, char**){
    auto context = Core::Context::GetInstance();
    App game;
    while (!context->GetExit()){
        App::State current_state = game.GetCurrentState();
        if (current_state == App::State::TITLE_SCREEN){
            game.TitleScreen();
        }
        else if (current_state == App::State::START){
            game.Start();
        }
        else if (current_state == App::State::LOADING){
            game.Loading(0.02);
        }
        else if (current_state == App::State::UPDATE){
            game.Update();
        }
        else if (current_state == App::State::MARIO_DEATH){
            game.MarioDeath(0.02);
        }
        else if (current_state == App::State::TIME_UP){
            game.TimeUp(0.02);
        }
        else if (current_state == App::State::GAME_OVER){
            game.GameOver(0.02);
        }
        else if (current_state == App::State::LEVEL_CLEAR){
            game.LevelClear(0.02);
        }
        else if (current_state == App::State::CLEAR_WALK_TO_CASTLE){
            game.ClearWalkToCastle(0.02);
        }
        else if (current_state == App::State::ADD_REMAIN_TIME_TO_SCORE){
            game.AddTimeToScore(0.75);
        }
        else if (current_state == App::State::PIPE_ENTER){
            if (game.GetLevel() == "1_2"){
                game.PipeEnter(0.02, game.GetCManager()->GetPipeEnterLevel());
            }
            else{
                game.PipeEnter(0.02, "AAA");
            }
        }
        else if (current_state == App::State::LEVEL2_ANIMATION){
            game.Level2Animation(0.02);
        }
        else if (current_state == App::State::BRIDGE_CLEAR){
            game.DeleteBridge(0.02);
        }
        else if (current_state == App::State::GAME_CLEAR){
            game.GameClear(0.02);
        }
        else{
            game.End();
            context->SetExit(true);
            break;
        }
        context->Update();
    }
    return 0;
}
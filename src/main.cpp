#include "Core/Context.hpp"
#include "App.hpp"

int main(int, char**){
    auto context = Core::Context::GetInstance();
    App game;
    while (!context->GetExit()){
        App::State current_state = game.GetCurrentState();
        if (current_state == App::State::START){
            game.Start();
        }
        else if (current_state == App::State::UPDATE){
            game.Update();
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
#include "App.hpp"

void App::CamPosAdjust(){
    glm::vec2 mario_pos = mario->GetAnimationObject()->GetPosition();
    glm::vec2 map_size = background->GetScaledSize();

    float leftBound = map_size.x/2 * -1;
    float rightBound = map_size.x/2;

    if (mario_pos.x < leftBound + 384){
        CameraPosition.x = leftBound + 384;
    }
    else if (mario_pos.x > rightBound - 384){
        CameraPosition.x = rightBound - 384;
    }
    else{
        CameraPosition.x = mario_pos.x;
    }

    CameraPosition *= -1;
}

void App::FireballUpdate(){
    auto m_fireballs = mario->GetFireballs();
    for (auto fb : m_fireballs){
        fireballStore.push(fb);
        m_Renderer.AddChild(fb->GetAnimationObject());
    }

    //1.Perform all physic process of fireballs
    //2.Removing fireballs that should be removed
    int fireballs_size = fireballStore.size();
    //LOG_DEBUG(fireballs_size);
    for (int i = 0; i < fireballs_size; i++) {
        auto fb = fireballStore.front();
        fireballStore.pop();
        fb->Behavior();
        if (fb->OutOfRange(-CameraPosition) || fb->IsMarkedRemove()) {
            m_Renderer.RemoveChild(fb->GetAnimationObject());
            continue;
        }
        fireballStore.push(fb);
    }
}
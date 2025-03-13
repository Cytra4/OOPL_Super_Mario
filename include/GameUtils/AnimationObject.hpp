#ifndef ANIMATIONOBJECT_HPP
#define ANIMATIONOBJECT_HPP

#include "Util/GameObject.hpp"
#include "pch.hpp"

class AnimationObject : public Util::GameObject{
private:
    std::string defaultSprite;
    std::vector<std::vector<std::string>> animationPaths;
    bool facingRight = true;
    int current_animation = -1;

public:
    AnimationObject(std::string defaultPath, glm::vec2 pos);

    void SetPosition(const glm::vec2& Position);

    glm::vec2 GetPosition();

    void SetDefaultSprite(std::string sprite);

    std::string GetDefaultSprite();

    void AddNewAnimation(std::vector<std::string> animation);

    void SetAnimation(int index, int interval);

    std::vector<std::vector<std::string>>& GetAnimationPaths();

    void SetFacingRight(bool right);

    bool IsFacingRight();

    void SetCurrentAnimation(int i);

    int GetCurrentAnimation();
    
    void SetLooping(bool looping);

    void SetScale(glm::vec2 scale);

    bool IfAnimationEnds();

    bool IsPlaying();

    void PlayAnimation();

    void PauseAnimation();
};

#endif
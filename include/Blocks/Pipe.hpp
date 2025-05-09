#ifndef PIPE_HPP
#define PIPE_HPP

#include <GameUtils/CollisionBox.hpp>
#include <GameUtils/AnimationObject.hpp>

//There will have 3 type of Pipe heights

class Pipe{
private:
    std::shared_ptr<AnimationObject> ani_obj;
    CollisionBox box;
    bool canEnter;
    std::string next_level;
    int height;

public:
    Pipe(bool enter, std::string level, int height, glm::vec2 pos);

    CollisionBox& GetBox();

    std::shared_ptr<AnimationObject> GetAnimationObject();

    bool CanBeEnter();

    int GetHeight();

    std::string GetNextLevel();
};

#endif
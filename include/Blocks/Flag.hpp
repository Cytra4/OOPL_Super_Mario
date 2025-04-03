#ifndef FLAG_HPP
#define FLAG_HPP

#include "GameUtils/CollisionBox.hpp"
#include "GameUtils/AnimationObject.hpp"

//The flag, but more percisely, only the pole of the flag, the flag itself will just be a animation object
//*For the axe in 1-4, I'll probably make a seperate class (But both are similar I guess)

class Flag{
private:
    std::shared_ptr<AnimationObject> ani_obj;
    std::shared_ptr<AnimationObject> flag;
    CollisionBox box;
    bool DropFlagFinish = false;
public:
    Flag(glm::vec2 position, int width, int height);

    CollisionBox& GetBox();

    std::shared_ptr<AnimationObject> GetAnimationObject();

    std::shared_ptr<AnimationObject> GetFlagAniObj();

    void MarkFinish();

    bool isFlagDropDone();
};

#endif
#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Background : public Util::GameObject{
    private:
        std::string imagePath;
    
    public:
        Background(std::string imagePath) : GameObject(std::make_unique<Util::Image>(imagePath), -10){
            this->imagePath = imagePath;
        }

        std::string GetImagePath(){
            return imagePath;
        }

        void SetBackground(std::string new_image){
            imagePath = new_image;
        }

        void DrawBackground() {
            auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
            temp->SetImage(imagePath);
        }
};

#endif
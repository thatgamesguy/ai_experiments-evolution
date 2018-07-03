#ifndef C_ScreenWrapAround_hpp
#define C_ScreenWrapAround_hpp

#include "Component.hpp"

class C_ScreenWrapAround : public Component
{
public:
    C_ScreenWrapAround(Object* owner);
    
    void LateUpdate(float deltaTime) override;
    
    void SetSpriteHalfSize(const sf::Vector2i& spriteSize);
    void SetScreenSize(const sf::Vector2i& screenSize);
    
private:
    sf::Vector2i screenSize;
    sf::Vector2i spriteHalfSize;
};

#endif /* C_ScreenWrapAround.hpp */

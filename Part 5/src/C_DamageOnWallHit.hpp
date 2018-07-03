#ifndef C_DamageOnWallHit_hpp
#define C_DamageOnWallHit_hpp

#include "Component.hpp"
#include "C_Energy.hpp"
#include "C_Sight.hpp"

class C_DamageOnWallHit : public Component
{
public:
    C_DamageOnWallHit(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetWindowSize(const sf::Vector2u& windowSize);
    
private:
    std::shared_ptr<C_Energy> energy;
    std::shared_ptr<C_Sight> sight;
    
    sf::Vector2u windowSize;
};

#endif /* C_DamageOnWallHit_hpp */

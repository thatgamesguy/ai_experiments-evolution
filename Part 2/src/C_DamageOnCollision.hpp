#ifndef C_DamageOnCollision_hpp
#define C_DamageOnCollision_hpp

#include "Component.hpp"
#include "C_Energy.hpp"
#include "C_Sight.hpp"

class C_DamageOnCollision : public Component
{
public:
    C_DamageOnCollision(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
private:
    std::shared_ptr<C_Energy> energy;
    std::shared_ptr<C_Sight> sight;
    float energyReducedOnContact;
};

#endif /* C_DamageOnCollision_hpp */



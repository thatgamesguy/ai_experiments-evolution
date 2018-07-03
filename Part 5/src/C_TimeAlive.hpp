#ifndef C_TimeAlive_hpp
#define C_TimeAlive_hpp

#include "Component.hpp"

class C_TimeAlive : public Component
{
public:
    C_TimeAlive(Object* owner);
    
    void Update(float deltaTime) override;
    
    float Get() const;
    
private:
    float timeAlive;
};

#endif /* C_TimeAlive_hpp */

#include "C_Velocity.hpp"
#include "Object.hpp"

C_Velocity::C_Velocity(Object* owner) : Component(owner), velocity(0.f, 0.f), maxVelocity(80.f, 80.f){}


void C_Velocity::Update(float deltaTime)
{
    owner->transform->AddPosition(velocity * deltaTime);
}

void C_Velocity::Set(const sf::Vector2f& vel)
{
    velocity = vel;
    
    if (fabs(velocity.x) > maxVelocity.x)
    {
        velocity.x = velocity.x > 0.f ? maxVelocity.x : -maxVelocity.x;
    }
    
    if (fabs(velocity.y) > maxVelocity.y)
    {
        velocity.y = velocity.y > 0.f ? maxVelocity.y : -maxVelocity.y;
    }
}

const sf::Vector2f& C_Velocity::Get() const
{
    return velocity;
}

#include "C_DamageOnWallHit.hpp"
#include "Object.hpp"

C_DamageOnWallHit::C_DamageOnWallHit(Object* owner) : Component(owner), windowSize(1920, 1080) {}

void C_DamageOnWallHit::Awake()
{
    energy = owner->GetComponent<C_Energy>();
    sight = owner->GetComponent<C_Sight>();
}

void C_DamageOnWallHit::Update(float deltaTime)
{
    const sf::Vector2f& pos = owner->transform->GetPosition();
    const float radius = sight->GetRadius();
    const float halfRadius = radius * 0.5f;
    
    if(pos.x <= halfRadius || pos.x >= windowSize.x - halfRadius || pos.y <= halfRadius || pos.y >= windowSize.y - halfRadius)
    {
        energy->Set(0.f);
    }
}

void C_DamageOnWallHit::SetWindowSize(const sf::Vector2u& windowSize)
{
    this->windowSize = windowSize;
}

#ifndef C_Energy_hpp
#define C_Energy_hpp

#include "Component.hpp"

class C_Energy : public Component
{
public:
    C_Energy(Object* owner);
    
    void Set(float value);
    float Get() const;
    
private:
    float value;
    
};

#endif /* C_Energy_hpp */

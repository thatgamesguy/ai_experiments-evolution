#ifndef C_NeuralNetwork_hpp
#define C_NeuralNetwork_hpp

#include "Component.hpp"
#include "C_Velocity.hpp"
#include "NeuralNetwork.hpp"
#include "ObjectCollection.hpp"
#include "C_Sight.hpp"

class C_NeuralNetwork : public Component
{
public:
    C_NeuralNetwork(Object* owner);
    
    void Awake() override;
    
    void Update(float deltaTime) override;
    
    void SetWindowSize(const sf::Vector2u& windowSize);
    
    const NeuralNetwork& Get() const;
    void Set(NeuralNetwork& network);
    
private:
    std::vector<float> BuildNetworkInput();
    
    const int neuralNumOfInput = 7;
    const int neuralNumOfHiddenLayers = 1;
    const int neuralNumOfNeuronsInHiddenLayer = 10;
    const int neuralNumOfOutput = 2;
    
    sf::Vector2u windowSize; // We need to know the
    float maxMoveForce;
    std::shared_ptr<C_Velocity> velocity;
    std::shared_ptr<C_Sight> sight;
    NeuralNetwork neuralNetwork;
};


#endif /* C_NeuralNetwork_hpp */

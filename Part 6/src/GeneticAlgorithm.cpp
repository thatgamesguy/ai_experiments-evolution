
#include "GeneticAlgorithm.hpp"


GeneticAlgorithm::GeneticAlgorithm(ObjectCollection& objects, WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window) : objects(objects), workingDir(workingDir), textureAllocator(textureAllocator), window(window), maxPoolSize(40), totalFitnessScore(0.f), mutationChance(0.1f), genNumber(0), addedToGame(0)
{
    const unsigned int numOfUFOsToSpawn = 80;
    
    for (int i = 0; i < numOfUFOsToSpawn; i++)
    {
        SpawnUFO();
    }
}

void GeneticAlgorithm::Update(float deltaTime)
{
    std::vector<std::shared_ptr<Object>>& ufos = objects.GetObjects();
    
    for(auto& o : ufos)
    {
        auto energy = o->GetComponent<C_Energy>();
        
        if(energy != nullptr && energy->Get() <= 0)
        {
            auto neuralNetwork = o->GetComponent<C_NeuralNetwork>();
            
            if(neuralNetwork != nullptr)
            {
                std::shared_ptr<UFOGAData> gaData = std::make_shared<UFOGAData>();
                gaData->energy = energy;
                gaData->neuralNet = neuralNetwork;
                gaData->timeAlive = o->GetComponent<C_TimeAlive>();
                
                AddToPool(gaData);
                
                CalculateTotalFitness();
                
                if(pool.size() > 1)
                {
                    int parentOne = FitnessProportionateSelection();
                    int parentTwo = parentOne;
                    
                    while(parentOne == parentTwo)
                    {
                        parentTwo = FitnessProportionateSelection();
                    }
                    
                    if (parentOne >= 0 && parentTwo >= 0)
                    {
                        // We need to combine the two neural networks and create a new UFO to add to the pool.
                        const NeuralNetwork networkOne = pool[parentOne]->neuralNet->Get();
                        const NeuralNetwork networkTwo = pool[parentTwo]->neuralNet->Get();
                        
                        NeuralNetwork childNetwork = CreateNeuralNetworkFromCrossOver(networkOne, networkTwo);
                        
                        float mutation = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
                        if (mutation < mutationChance)
                        {
                            childNetwork.Mutate();
                        }
                        
                        std::shared_ptr<C_NeuralNetwork> newUFO = SpawnUFO();
                        newUFO->Set(childNetwork); // We've created a new UFO and set its network to be a combination of two retirned UFOs.
                        
                        if(++addedToGame % maxPoolSize == 0)
                        {
                            LogAverageFitness();
                            addedToGame = 0;
                        }
                    }
                }
            }
            
            o->QueueForRemoval();
        }
    }
}

void GeneticAlgorithm::AddToPool(std::shared_ptr<UFOGAData> ufo)
{
    pool.emplace_back(ufo);
    
    std::sort(pool.begin(), pool.end(), PoolSort);
    
    if(pool.size() > maxPoolSize)
    {
        pool.erase(pool.end() - 1);
    }
}

void GeneticAlgorithm::CalculateTotalFitness()
{
    totalFitnessScore = 0;
    
    for (int i = 0; i < pool.size(); i++)
    {
        totalFitnessScore += pool[i]->timeAlive->Get();
    }
}

int GeneticAlgorithm::FitnessProportionateSelection()
{
    float randomSlice = static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / totalFitnessScore));
    
    float fitnessTotal = 0;
    
    for (int i = 0; i < pool.size(); i++)
    {
        fitnessTotal += pool[i]->timeAlive->Get();
        
        if (fitnessTotal >= randomSlice)
        {
            return i;
        }
    }
    
    return -1;
}

NeuralNetwork GeneticAlgorithm::CreateNeuralNetworkFromCrossOver(const NeuralNetwork& networkOne, const NeuralNetwork& networkTwo)
{
    std::vector<float> parentOneWeights = networkOne.GetWeights();
    std::vector<float> parentTwoWeights = networkTwo.GetWeights();
    
    std::vector<int> crossoverPoints = networkOne.GetSplitPoints();
    
    int crossOverIndex = (rand() % static_cast<int>(crossoverPoints.size()));
    int crossOverPoint = crossoverPoints[crossOverIndex];
    
    std::vector<float> newWeights;
    
    for (int i = 0; i < crossOverPoint; i++)
    {
        newWeights.push_back(parentOneWeights[i]);
    }
    
    for (int i = crossOverPoint; i < parentOneWeights.size(); i++)
    {
        newWeights.push_back(parentTwoWeights[i]);
    }
    
    int numOfInput = networkOne.numOfInput;
    int numOfHiddenLayers = networkOne.numOfHiddenLayers;
    int numOfNeuronsInHiddenLayer = networkOne.numOfNeuronsInHiddenLayers;
    int numOfOutput = networkOne.numOfOutput;
    
    NeuralNetwork neuralNet(numOfInput, numOfHiddenLayers, numOfNeuronsInHiddenLayer, numOfOutput);
    neuralNet.SetWeights(newWeights);
    
    return neuralNet;
}

void GeneticAlgorithm::LogAverageFitness()
{
    log.open("ga_log.txt", std::ios_base::app | std::ios_base::out);
    log << std::to_string(++genNumber) << ": " << std::to_string(totalFitnessScore / maxPoolSize) <<  std::endl;
    log.close();
}

std::shared_ptr<C_NeuralNetwork> GeneticAlgorithm::SpawnUFO()
{
    std::shared_ptr<Object> ufo = std::make_shared<Object>(); // Create new object
    
    auto sprite = ufo->AddComponent<C_Sprite>(); // Add sprite component
    sprite->SetTextureAllocator(&textureAllocator); // This provides the means for the sprite component to load images by name.
    
    const std::string spritePreName = "ufo";
    const std::string spritePostName = ".png";
    const std::string ufoCount = std::to_string(1 + (std::rand() % (4 - 1 + 1)));
    sprite->Load(workingDir.Get() + spritePreName + ufoCount + spritePostName); // Builds random name, can be ufo1, ufo2, ufo3, or ufo4.
    sprite->SetPivot(0.5f, 0.5f);
    
    const sf::FloatRect windowRect = window.GetViewSpace();
    const sf::Vector2u windowSize(windowRect.width, windowRect.height);
    const int minX = 50;
    const int minY = 50;
    const int maxX = windowSize.x - minX;
    const int maxY = windowSize.y - minY;
    const int randX = minX + (std::rand() % (maxX - minX + 1));
    const int randY = minY + (std::rand() % (maxY - minY + 1));
    ufo->transform->SetPosition(randX, randY);

    ufo->AddComponent<C_Velocity>();
    auto sight = ufo->AddComponent<C_Sight>();
    sight->SetObjectCollection(&objects);
    auto neuralNet = ufo->AddComponent<C_NeuralNetwork>();
    neuralNet->SetWindowSize(windowSize);
    
    const sf::IntRect spriteRect = sprite->GetTextureRect();
    auto wrapAround = ufo->AddComponent<C_ScreenWrapAround>();
    wrapAround->SetScreenSize({(int)windowRect.width, (int)windowRect.height});
    wrapAround->SetSpriteHalfSize({(int)(spriteRect.width * 0.5f), (int)(spriteRect.height * 0.5f)});
    
    ufo->AddComponent<C_TimeAlive>();
    
    ufo->AddComponent<C_Energy>();
    auto wallHit = ufo->AddComponent<C_DamageOnWallHit>();
    wallHit->SetWindowSize(windowSize);
    ufo->AddComponent<C_DamageOnCollision>();
    
    objects.Add(ufo); // Adds object to game.
    
    return neuralNet;
}

bool GeneticAlgorithm::PoolSort(std::shared_ptr<UFOGAData> a, std::shared_ptr<UFOGAData> b)
{
    return a->timeAlive->Get() > b->timeAlive->Get();
}

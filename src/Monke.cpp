//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>

void Monke::operator()()
{
    std::thread hungerThread(&Monke::startHungerDecrement, this);
    hungerThread.join();

    while(true)
    {
        if(this->hungerLevel <= 69 + 1)
        {
            std::cout << "Monke " << this->id << " is hungary..." << std::endl;

            this->recipe = this->kitchen.getRandomRecipe();

            std::cout << "Monke " << this->id << " is preparing: " << recipe.getName() << std::endl;

            while (true)
            {
                auto step = this->recipe.getNextStep();

                if (step.item == "DONE")
                    break;

                if (rand() % 3 == 0)
                {
                    std::cout << "Monke " << this->id << " is scratching it's butt" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1));
                }

                this->kitchen.useItem(id, step.item);

                std::this_thread::sleep_for(std::chrono::seconds(step.secondsDuration));

                this->kitchen.releaseItem(id, step.item);

            }

            std::cout << "Monke " << this->id << " is eating his " << recipe.getName() << "..." << std::endl;

            auto eatingTime = recipe.getEatingTime();
            auto foodValue = recipe.getValue();

            std::this_thread::sleep_for(std::chrono::seconds(eatingTime - eatingTime/this->eatingSpeed));
            this->hungerLevel = (recipe.getValue()*10 < 100)? (hungerLevel + recipe.getValue()*10) : 100;

            std::cout << "Monke " << this->id << " finished eating! Time to rest." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(foodValue));
        }
    }
}

void Monke::startHungerDecrement()
{
    std::thread decrementThread([this]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(8));
            hungerLevel = (hungerLevel-10 > 0)? hungerLevel - 10 : 0;
            std::cout << "[HUNGER] Monke " << this->id  << " = " << hungerLevel << std::endl;
        }
    });
    decrementThread.detach();
}

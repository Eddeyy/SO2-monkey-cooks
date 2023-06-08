//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>

void Monke::operator()()
{
        while(true)
        {
            auto step = this->recipe.getNextStep();

            if(step.item == "DONE")
                break;

            if(rand() % 3 == 0)
            {
                std::cout << "Monke " << this->id << " is scratching it's butt" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(rand() % 4));
            }

            this->kitchen.useItem(id, step.item);

            std::this_thread::sleep_for(std::chrono::seconds(step.secondsDuration));

            this->kitchen.releaseItem(id, step.item);
        }
}

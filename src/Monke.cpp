//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>

void Monke::operator()()
{
        while(true)
        {
            auto step = this->recipe.getNextStep();

            if(step.description == "DONE")
                break;

            this->kitchen.useItem(id, step.item);

            std::this_thread::sleep_for(std::chrono::seconds(step.secondsDuration));

            this->kitchen.releaseItem(id, step.item);
        }
}

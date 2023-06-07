//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>

void Monke::operator()()
{
    auto step = this->recipe.getNextStep();
    while(step.description != "DONE")
    {
        this->kitchen.useItem(id, step.item);

        std::this_thread::sleep_for(std::chrono::seconds(step.secondsDuration));

        this->kitchen.releaseItem(id, step.item);
    }
}

//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>
#include <MonkeUtility.hpp>

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

                auto items = MonkeUtility::findKeysWithSubstring(kitchen.getAvailabilityMap(), step.item);
                std::string itemName;

                for(auto& item : items)
                {
                    if (!kitchen.getAvailabilityMap().at(item))
                        continue;

                    std::cout << "Monke " << this->id << " found a free "<< step.item <<"!" << std::endl;
                    itemName = item;
                    break;
                }

                if(itemName.empty())
                    itemName = items[MonkeUtility::getRandomIndex(0, items.size()-1)];

                this->kitchen.useItem(id, itemName);

                std::this_thread::sleep_for(std::chrono::seconds(step.secondsDuration));

                this->kitchen.releaseItem(id, itemName);

            }

            /*
             * Symulacja procesu jedzenia.
             * */
            std::cout << "Monke " << this->id << " is eating his " << recipe.getName() << "..." << std::endl;

            auto eatingTime = recipe.getEatingTime();
            auto foodValue = recipe.getValue();

            bool foundTable = false;

            auto seats =  MonkeUtility::findKeysWithSubstring(kitchen.getAvailabilityMap(), "seat");
            std::string seat;
            std::cout << "Monke " << this->id << " is looking for a seat to eat." << std::endl;

            for (auto &table: seats)
            {
                if (!kitchen.getAvailabilityMap().at(table))
                    continue;

                std::cout << "Monke " << this->id << " found a free seat!" << std::endl;
                seat = table;
                foundTable = true;
                break;
            }
            if(seat.empty())
                seat = seats[MonkeUtility::getRandomIndex(0, seats.size()-1)];

            this->kitchen.useItem(id, seat);

            std::this_thread::sleep_for(std::chrono::seconds(eatingTime - eatingTime/this->eatingSpeed));
            this->hungerLevel = (recipe.getValue()*10 < 100)? (hungerLevel + recipe.getValue()*10) : 100;
            this->kitchen.releaseItem(id, seat);

            std::cout << "Monke " << this->id << " finished eating! Time to rest." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(foodValue));
        }
    }
}

void Monke::startHungerDecrement()
{
    std::thread decrementThread([this]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(hungeringTime)); // Co X sekund;
            hungerLevel = (hungerLevel-hungerDepletionAmount > 0)? hungerLevel - hungerDepletionAmount : 0; // Dekrementuj poziom głodu o 10
            std::cout << "[HUNGER] Monke " << this->id  << " = " << hungerLevel << std::endl;
        }
    });
    decrementThread.detach();
}

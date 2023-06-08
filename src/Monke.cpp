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

                claim_item_for_time(step.item, step.secondsDuration);
            }

            /*
             * Symulacja procesu jedzenia.
             * */
            std::cout << "Monke " << this->id << " is eating his " << recipe.getName() << "..." << std::endl;

            auto eatingTime = recipe.getEatingTime();
            auto foodValue = recipe.getValue();

            claim_item_for_time("seat", (eatingTime - eatingTime / this->eatingSpeed), foodValue);

            std::cout << "Monke " << this->id << " finished eating! Time to rest." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(foodValue));
        }
    }
}

void Monke::claim_item_for_time(const std::string &itemName, const uint32_t& duration, const uint32_t& food_value)
{
    auto items = MonkeUtility::findKeysWithSubstring(kitchen.getAvailabilityMap(), itemName);
    std::string itemKeyValue;

    for(auto& item : items)
    {
        if (!kitchen.getAvailabilityMap().at(item))
            continue;

        std::cout << "Monke " << id << " found a free " << itemName << "!" << std::endl;
        itemKeyValue = item;
        break;
    }

    if(itemKeyValue.empty())
        itemKeyValue = items[MonkeUtility::getRandomIndex(0, items.size() - 1)];

    kitchen.useItem(id, itemKeyValue);

    std::this_thread::sleep_for(std::chrono::seconds(duration));
    this->hungerLevel = (food_value*10 < 100)? (hungerLevel + food_value*10) : 100;
    kitchen.releaseItem(id, itemKeyValue);
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

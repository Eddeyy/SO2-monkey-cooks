//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>
#include <MonkeUtility.hpp>

void Monke::operator()()
{
    start_hunger_decrement();

    while(true)
    {
        /*
         * Jeżeli głód spadnie poniżej 70 - szukaj przepisu.
         */
        if(this->hunger_level <= 69 + 1)
        {
            // std::cout << "Monke " << this->id << " is hungary..." << std::endl;
            this->status = "hungry";
            this->recipe = this->kitchen.getRandomRecipe();

            // std::cout << "Monke " << this->id << " is preparing: " << recipe.getName() << std::endl;

            /*
             * Pętla odpowiedzialna za wykonywanie kolejnych kroków.
             */

            if(rand() % 2 == 0)
            {
                bool helped = try_help_another();
                if(!helped)
                {
                    cook();
                }
            }
            else
            {
                cook();
            }
            this->isBeingHelped = false;

            /*
             * Symulacja procesu jedzenia.
             * */
            // std::cout << "Monke " << this->id << " is eating his " << recipe.getName() << "..." << std::endl;

            auto eatingTime = recipe.getEatingTime();
            auto foodValue = recipe.getValue();

            claim_item_for_time("seat", (eatingTime - eatingTime / this->eating_speed), MonkeStatus::eating, foodValue);


            /**
             * Odpoczynek po gotowaniu i zjedzeniu.
             */
            // std::cout << "Monke " << this->id << " finished eating! Time to rest." << std::endl;
            this->status = "resting";
            sleep_for(foodValue);
        }
    }
}

bool Monke::try_help_another()
{
    std::unique_lock<std::mutex> lock(mutex);
    this->status = "is looking to help someone";
    std::cout << "[HELPING] Monke " << this->id << " " << status << std::endl;
    std::shared_ptr<Monke> monke;

    for(auto &m : *allMonkes)
    {
        if(m->id == this->id)
            continue;

        if(!m->isCooking && m->isBeingHelped)
            continue;

        /*
         * jeśli małpa nie jest dalej niż w połowie swojego przepisu...
         */
        if(m->recipe.getSteps().size() < m->recipe.getSteps().size()/2)
            continue;

        monke = m;
        break;
    }

    if(!monke || monke->id == this->id)
    {
        return false;
    }

    this->status = "is helping monke " + std::to_string(monke->id);

    std::cout << "[HELPING] Monke " << this->id << " " << status << monke->id << std::endl;

    monke->isBeingHelped = true;

    for(auto &s : monke->recipe.getSteps())
    {
        s.secondsDuration = s.secondsDuration/2;
    }
    monke->time_left = monke->time_left/2;
    auto newFoodValue = monke->recipe.getValue()*3/2;
    auto newEatingTime = monke->recipe.getEatingTime()*3/2;

    monke->recipe.setValue(newFoodValue);
    monke->recipe.setEatingTime(newEatingTime);

    this->recipe = Recipe({}, newEatingTime, monke->recipe.getName(), newFoodValue);
    cv.wait(lock, [this, &monke] {return !monke->isCooking;});

    return true;
}

void Monke::cook()
{
    this->isCooking = true;
    while (true)
    {
        auto step = this->recipe.getNextStep();

        if (step.item == "DONE")
            break;

        if (rand() % 3 == 0)
        {
            std::cout << "Monke " << this->id << " is scratching it's butt" << std::endl;
            this->status = "scratching butt";
            sleep_for(rand() % 3 + 1);
        }

        claim_item_for_time(step.item, step.secondsDuration, MonkeStatus::cooking);
    }
    this->isCooking = false;
}

void Monke::claim_item_for_time(const std::string &itemName, const int32_t& duration, MonkeStatus status, const uint32_t& food_value)
{
    auto items = MonkeUtility::findKeysWithSubstring(kitchen.getAvailabilityMap(), itemName);
    std::string itemKeyValue;

    for(auto& item : items)
    {
        if (!kitchen.getAvailabilityMap().at(item))
            continue;

        // std::cout << "Monke " << id << " found a free " << itemName << "!" << std::endl;
        itemKeyValue = item;
        break;
    }

    if(itemKeyValue.empty())
        itemKeyValue = items[MonkeUtility::getRandomIndex(0, items.size() - 1)];

    this->status = "using " + itemKeyValue;
    kitchen.useItem(id, itemKeyValue);

    sleep_for(duration);

    this->hunger_level = (food_value * 10 < 100) ? (hunger_level + food_value * 10) : 100;
    kitchen.releaseItem(id, itemKeyValue);
    this->status = "NONE";
}

void Monke::start_hunger_decrement()
{
    std::thread decrementThread([this]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(hungering_time)); // Co X sekund;
            hunger_level = (hunger_level - hunger_depletion_amount > 0) ? hunger_level - hunger_depletion_amount : 0; // Dekrementuj poziom głodu o 10
            // std::cout << "[HUNGER] Monke " << this->id << " = " << hunger_level << std::endl;
        }
    });
    decrementThread.detach();
}

void Monke::sleep_for(const int32_t &seconds)
{
    this->time_left = seconds;
    while(this->time_left > 0)
    {
        // std::cout << "[TIMER] Monke " << this->id << " time left "<< this->status << " = " << this->time_left << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->time_left--;
    }
}

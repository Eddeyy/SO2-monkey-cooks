//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>
#include <MonkeUtility.hpp>

void Monke::operator()()
{
    start_hunger_decrement();

    while (true)
    {
        if (should_search_for_recipe())
        {
            this->status = "hungry";
            this->recipe = this->kitchen.getRandomRecipe();

            should_help_another_monke() ? (try_help_another(), void()) : cook();

            eat_and_rest();
        }
    }
}

bool Monke::should_search_for_recipe()
{
    return this->hunger_level <= 69 + 1;
}

bool Monke::should_help_another_monke()
{
    return rand() % 3 == 0;
}

bool Monke::try_help_another()
{
    this->status = "is looking to help";
    std::shared_ptr<Monke> monke = findAvailableMonkeToHelp();

    if (monke == nullptr)
    {
        return false;
    }

    std::unique_lock<std::mutex> lock(monke->mutex);

    adjustRecipeForHelping(monke);
    waitForMonkeToFinishCooking(monke, lock);

    lock.unlock();

    return true;
}

std::shared_ptr<Monke> Monke::findAvailableMonkeToHelp()
{
    for (auto& m : *allMonkes)
    {
        if (m->id == this->id || !m->isCooking || m->helpingMonke != nullptr || m->recipe.getSteps().size() < m->recipe.getSteps().size() / 2)
            continue;

        m->helpingMonke = this;
        return m;
    }

    return nullptr;
}

void Monke::adjustRecipeForHelping(const std::shared_ptr<Monke>& monke)
{
    for (auto& step : monke->recipe.getSteps())
    {
        step.secondsDuration = step.secondsDuration / 2 + 1;
    }

    auto newFoodValue = (monke->recipe.getValue() * 2) / 3;
    auto newEatingTime = (monke->recipe.getEatingTime() * 2) / 3;

    monke->recipe.setValue(newFoodValue);
    monke->recipe.setEatingTime(newEatingTime);

    this->recipe = Recipe({}, newEatingTime, monke->recipe.getName(), newFoodValue);
}

void Monke::waitForMonkeToFinishCooking(const std::shared_ptr<Monke>& monke, std::unique_lock<std::mutex>& lock)
{
    this->kitchen.helpingMonkes++;
    this->helpedMonkeId = monke->id;
    this->status = "is helping monke " + std::to_string(monke->id);
    cv.wait(lock, [&monke] {
        return !monke->isCooking;
    });
    this->kitchen.helpingMonkes--;
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
            this->status = "scratching butt";
            sleep_for(rand() % 3 + 1);
        }

        claim_item_for_time(step.item, step.secondsDuration, MonkeStatus::cooking);
    }
    this->isCooking = false;

    if (this->helpingMonke)
    {
        this->helpingMonke.load()->cv.notify_one();
        this->helpingMonke = nullptr;
    }
}

void Monke::eat_and_rest()
{
    this->status = "eating";
    auto eatingTime = recipe.getEatingTime();
    auto foodValue = recipe.getValue();
    claim_item_for_time("seat", eatingTime - eatingTime / this->eating_speed, MonkeStatus::eating, foodValue);
    this->recipe.setName("");

    this->status = "resting";
    sleep_for(foodValue);
    this->status = "idle";
}

void Monke::claim_item_for_time(const std::string& itemName, const int32_t& duration, MonkeStatus status, const uint32_t& food_value)
{

    auto items = MonkeUtility::findKeysWithSubstring(kitchen.getAvailabilityMap(), itemName);
    std::string itemKeyValue;

    for (auto& item : items)
    {
        if (!kitchen.getAvailabilityMap().at(item))
            continue;
        itemKeyValue = item;
        break;
    }

    if (itemKeyValue.empty())
        itemKeyValue = items[MonkeUtility::getRandomIndex(0, items.size() - 1)];

    kitchen.useItem(id, itemKeyValue, *this);

    this->timesUsingItem[itemName] = timesUsingItem.find(itemName) != timesUsingItem.end()? timesUsingItem[itemName] + 1 : 1;

    sleep_for(duration);

    this->hunger_level = (hunger_level + food_value * 10 < 100) ? (hunger_level + food_value * 10) : 100;
    kitchen.releaseItem(id, itemKeyValue, *this);
    this->status = "NONE";
}

void Monke::start_hunger_decrement()
{
    std::thread decrementThread([this]() {
        while (true) {
            int how_much_to_hunger = hunger_depletion_amount / 2 + rand() % (int) (hunger_depletion_amount * 1.5);
            std::this_thread::sleep_for(std::chrono::seconds(hungering_time));
            hunger_level = (hunger_level - how_much_to_hunger > 0) ? hunger_level - how_much_to_hunger : 0;
        }
    });
    decrementThread.detach();
}

void Monke::sleep_for(const int32_t& seconds)
{
    this->time_left = seconds;
    while (this->time_left > 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->time_left--;
    }
}

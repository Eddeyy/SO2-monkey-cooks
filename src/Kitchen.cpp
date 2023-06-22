//
// Created by thero on 07.06.2023.
//

#include <Kitchen.hpp>
#include <MonkeUtility.hpp>
#include <Monke.hpp>


Kitchen::Kitchen(
    const std::vector<std::string>& items, 
    const std::vector<Recipe>& recipes
    )
: recipes{recipes}
{
    for(auto item : items)
    {
        availability[item] = true;
        itemTimesUsed[item] = 0;

        auto onlyItemName = item.rfind(' ') != std::string::npos? item.erase(item.rfind(' ')) : item;
        
        itemAmount[item] = itemAmount.find(onlyItemName) != itemAmount.end()? itemAmount[onlyItemName] + 1 : 1;
        itemMonkeId[item] = -1;
    }

}

void Kitchen::useItem(uint32_t monkeId, std::string itemName, Monke& monke) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::unique_lock<std::mutex> lock(this->mutexes[itemName]);
    this->cvs[itemName].wait(lock, [&] {
        monke.setStatus("is waiting for " + itemName);
        return this->availability[itemName]; });
    this->availability[itemName] = false;
    this->itemMonkeId[itemName] = monke.getId();
    this->itemTimesUsed[itemName]++;
    monke.setStatus("is using " + itemName);
}

void Kitchen::releaseItem(uint32_t monkeId, std::string& itemName, Monke& monke) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::lock_guard<std::mutex> lock(this->mutexes[itemName]);
    this->availability[itemName] = true;
    this->itemMonkeId[itemName] = -1;
    this->cvs[itemName].notify_all();
}

Recipe Kitchen::getRandomRecipe()
{
    return this->recipes[MonkeUtility::getRandomIndex(0, this->recipes.size()-1)];
}

const std::map<std::string, bool>& Kitchen::getAvailabilityMap() const
{
    return this->availability;
}

const std::vector<Recipe> &Kitchen::getRecipes() const
{
    return recipes;
}

const std::map<std::string, uint32_t>& Kitchen::getItemTimesUsed() 
{
    return this->itemTimesUsed;
}

const std::map<std::string, uint32_t>& Kitchen::getItemAmount()
{
    return this->itemAmount;
}

const std::map<std::string, int>& Kitchen::getItemMonkeId()
{
    return this->itemMonkeId;
}

const std::map<std::string, uint32_t> Kitchen::getHowManyUsedRightNow()
{
    std::map<std::string, uint32_t> kitchenItems;
    for(const auto& item : itemAmount)
    {
        kitchenItems[item.first] =  std::count_if(availability.begin(), availability.end(), [&item](const std::pair<std::string, uint32_t>& pair)
        {
            return pair.first.find(item.first) != std::string::npos && pair.second != true;
        });
    }
    return kitchenItems;
}

std::vector<std::pair<int, int>> Kitchen::getHelpingMonkes(const std::vector<std::shared_ptr<Monke>>& monkes)
{
    std::vector<std::pair<int, int>> helpingMonkes;

    for(const auto& monke : monkes)
    {
        if(monke.get()->isHelping())
        {
            helpingMonkes.emplace_back(std::pair<int, int>(monke.get()->getId(), monke.get()->getHelpedMonkeId()));
        }
    }

    return helpingMonkes;
}
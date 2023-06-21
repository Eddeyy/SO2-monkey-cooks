//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_KITCHEN_HPP
#define SO2_MONKEY_COOKS_KITCHEN_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <map>
#include <vector>
#include <random>
#include <algorithm>

#include <Recipe.hpp>

class Monke;

class Kitchen {
public:
    Kitchen(const std::vector<std::string>& items, const std::vector<Recipe>& recipes)
    : recipes{recipes}
    {
        // constructor body
        for(auto item : items)
        {
            availability[item] = true;
            itemTimesUsed[item] = 0;

            
            itemAmount[item] = itemAmount.find(item.erase(item.rfind(' '))) != itemAmount.end()? itemAmount[item] + 1 : 1;
            itemMonkeId[item] = -1;
        }

    }


    void useItem(uint32_t monkeId, std::string itemName, Monke& monke);
    void releaseItem(uint32_t monkeId, std::string& itemName, Monke& monke);

    Recipe getRandomRecipe();

    const std::map<std::string, bool>& getAvailabilityMap() const {
        return this->availability;
    }

    const std::vector<Recipe> &getRecipes() const
    {
        return recipes;
    }

    const std::map<std::string, uint32_t>& getItemTimesUsed() 
    {
        return this->itemTimesUsed;
    }

    const std::map<std::string, uint32_t>& getItemAmount()
    {
        return this->itemAmount;
    }

    const std::map<std::string, int>& getItemMonkeId()
    {
        return this->itemMonkeId;
    }

    const std::map<std::string, uint32_t> getHowManyUsedRightNow()
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

private:
    std::map<std::string, bool> availability;
    std::map<std::string, std::mutex> mutexes;
    std::map<std::string, std::condition_variable> cvs;

    //stats
    std::map<std::string, uint32_t> itemTimesUsed;
    std::map<std::string, uint32_t> itemAmount;
    std::map<std::string, int> itemMonkeId;

    std::vector<Recipe> recipes;
};

#endif //SO2_MONKEY_COOKS_KITCHEN_HPP

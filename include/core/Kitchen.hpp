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

#include <Recipe.hpp>

class Kitchen {
public:
    Kitchen(const std::vector<std::string>& items, const std::vector<Recipe>& recipes)
    : recipes{recipes}
    {
        // constructor body
        for(const auto& item : items)
        {
            availability[item] = true;
        }
    }

    void useItem(uint32_t monkeId, std::string itemName);
    void releaseItem(uint32_t monkeId, std::string& itemName);

    Recipe getRandomRecipe();

    const std::map<std::string, bool>& getAvailabilityMap() const {
        return this->availability;
    }

    const std::vector<Recipe> &getRecipes() const
    {
        return recipes;
    }

private:
    std::map<std::string, bool> availability;
    std::map<std::string, std::mutex> mutexes;
    std::map<std::string, std::condition_variable> cvs;
    std::vector<Recipe> recipes;
};

#endif //SO2_MONKEY_COOKS_KITCHEN_HPP

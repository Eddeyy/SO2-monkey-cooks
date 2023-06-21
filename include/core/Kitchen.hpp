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
    Kitchen(const std::vector<std::string>& items, const std::vector<Recipe>& recipes);


    void useItem(uint32_t monkeId, std::string itemName, Monke& monke);
    void releaseItem(uint32_t monkeId, std::string& itemName, Monke& monke);

    Recipe getRandomRecipe();

    const std::map<std::string, bool>& getAvailabilityMap() const;
    const std::vector<Recipe> &getRecipes() const;
    const std::map<std::string, uint32_t>& getItemTimesUsed();
    const std::map<std::string, uint32_t>& getItemAmount();
    const std::map<std::string, int>& getItemMonkeId();
    const std::map<std::string, uint32_t> getHowManyUsedRightNow();

private:

    // Kitchen item mutexes
    std::map<std::string, bool> availability;
    std::map<std::string, std::mutex> mutexes;
    std::map<std::string, std::condition_variable> cvs;

    // Kitchen item stats
    std::map<std::string, uint32_t> itemTimesUsed;
    std::map<std::string, uint32_t> itemAmount;
    std::map<std::string, int> itemMonkeId;

    std::vector<Recipe> recipes;
};

#endif //SO2_MONKEY_COOKS_KITCHEN_HPP

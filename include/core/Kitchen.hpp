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
    const uint32_t getItemTimesUsed(std::string itemName);
    const std::vector<std::pair<std::string, uint32_t>> getItemAmount();
    const uint32_t getItemAmount(std::string);
    const int getItemMonkeId(std::string itemName);
    const std::map<std::string, uint32_t> getHowManyUsedRightNow();

    std::vector<std::pair<int, int>> getHelpingMonkes(const std::vector<std::shared_ptr<Monke>>& monkes);

    uint32_t helpingMonkes = 0;
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

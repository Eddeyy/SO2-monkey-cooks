//
// Created by thero on 07.06.2023.
//

#include <Kitchen.hpp>
#include <MonkeUtility.hpp>
#include <Monke.hpp>


void Kitchen::useItem(uint32_t monkeId, std::string itemName, Monke& monke) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::unique_lock<std::mutex> lock(this->mutexes[itemName]);
    this->cvs[itemName].wait(lock, [&] {
        monke.setStatus("is waiting for " + itemName);
        return this->availability[itemName]; });
    this->availability[itemName] = false;

    monke.setStatus("is using " + itemName);
}

void Kitchen::releaseItem(uint32_t monkeId, std::string& itemName, Monke& monke) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::lock_guard<std::mutex> lock(this->mutexes[itemName]);
    this->availability[itemName] = true;
    this->cvs[itemName].notify_all();
}

Recipe Kitchen::getRandomRecipe()
{
    return this->recipes[MonkeUtility::getRandomIndex(0, this->recipes.size()-1)];
}


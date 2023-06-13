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
        // if(!this->availability[itemName])
        //     std::cout << "Monke " << monkeId << " waiting for " << itemName << "..." << std::endl;
        monke.setStatus("is waiting for " + itemName);
        return this->availability[itemName]; });
    this->availability[itemName] = false;

    monke.setStatus("is using " + itemName);
    std::cout << "Monke " << monkeId << " is using " + itemName << "." << std::endl;
}

void Kitchen::releaseItem(uint32_t monkeId, std::string& itemName, Monke& monke) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::lock_guard<std::mutex> lock(this->mutexes[itemName]);
    this->availability[itemName] = true;
    this->cvs[itemName].notify_all();
    // std::cout << "Monke " << monkeId << " finished using " + itemName << "." << std::endl;
}

Recipe Kitchen::getRandomRecipe()
{
    return this->recipes[MonkeUtility::getRandomIndex(0, this->recipes.size()-1)];
}


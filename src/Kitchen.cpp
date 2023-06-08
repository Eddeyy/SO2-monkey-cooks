//
// Created by thero on 07.06.2023.
//

#include <Kitchen.hpp>
#include <MonkeUtility.hpp>


void Kitchen::useItem(uint32_t monkeId, std::string itemName) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::unique_lock<std::mutex> lock(this->mutexes[itemName]);
    this->cvs[itemName].wait(lock, [&] {
        if(!this->availability[itemName])
            std::cout << "Monke " << monkeId << " waiting for " << itemName << "..." << std::endl;
        return this->availability[itemName]; });
    this->availability[itemName] = false;
    std::cout << "Monke " << monkeId << " is using " + itemName << "." << std::endl;
    lock.unlock();
}

void Kitchen::releaseItem(uint32_t monkeId, std::string& itemName) {
    itemName = MonkeUtility::toLowerCase(itemName);

    std::lock_guard<std::mutex> lock(this->mutexes[itemName]);
    this->availability[itemName] = true;
    this->cvs[itemName].notify_all();
    std::cout << "Monke " << monkeId << " finished using " + itemName << "." << std::endl;
}

Recipe Kitchen::getRandomRecipe()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, this->recipes.size() - 1);

    auto randomIndex = dist(gen);
    return this->recipes[randomIndex];
}


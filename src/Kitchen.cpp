//
// Created by thero on 07.06.2023.
//

#include <Kitchen.hpp>


void Kitchen::useItem(uint32_t monkeId, std::string itemName) {
    std::unique_lock<std::mutex> lock(this->mutexes[itemName]);
    this->cvs[itemName].wait(lock, [&] { return this->availability[itemName]; });
    this->availability[itemName] = false;
    std::cout << "Chef " << monkeId << " is using " + itemName << "." << std::endl;
    lock.unlock();
}

void Kitchen::releaseItem(uint32_t monkeId, const std::string& itemName) {
    std::lock_guard<std::mutex> lock(this->mutexes[itemName]);
    this->availability[itemName] = true;
    this->cvs[itemName].notify_all();
    std::cout << "Chef " << monkeId << " finished using " + itemName << "." << std::endl;
}

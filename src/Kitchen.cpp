//
// Created by thero on 07.06.2023.
//

#include <Kitchen.hpp>

void Kitchen::useOven(uint32_t monkeId)
{
    std::unique_lock<std::mutex> lock(ovenMutex);
    ovenCV.wait(lock, [&] { return ovenAvailable; });
    ovenAvailable = false;
    std::cout << "Chef " << monkeId << " is using the oven." << std::endl;
    lock.unlock();
}

void Kitchen::useMixer(uint32_t monkeId)
{
    std::unique_lock<std::mutex> lock(mixerMutex);
    mixerCV.wait(lock, [&] { return mixerAvailable; });
    mixerAvailable = false;
    std::cout << "Chef " << monkeId << " is using the mixer." << std::endl;
    lock.unlock();
}

void Kitchen::useFridge(uint32_t monkeId)
{
    std::unique_lock<std::mutex> lock(fridgeMutex);
    fridgeCV.wait(lock, [&] { return fridgeAvailable; });
    fridgeAvailable = false;
    std::cout << "Chef " << monkeId << " is using the fridge." << std::endl;
    lock.unlock();
}

void Kitchen::releaseOven()
{
    std::lock_guard<std::mutex> lock(ovenMutex);
    ovenAvailable = true;
    ovenCV.notify_all();
}

void Kitchen::releaseMixer()
{
    std::lock_guard<std::mutex> lock(mixerMutex);
    mixerAvailable = true;
    mixerCV.notify_all();
}

void Kitchen::releaseFridge()
{
    std::lock_guard<std::mutex> lock(fridgeMutex);
    fridgeAvailable = true;
    fridgeCV.notify_all();
}

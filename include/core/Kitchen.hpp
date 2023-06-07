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

class Kitchen {
public:
    Kitchen(const std::vector<std::string>& items)
    {
        // constructor body
        for(const auto& item : items)
        {
            availability[item] = true;
        }
    }

    void useItem(uint32_t monkeId, std::string itemName);
    void releaseItem(uint32_t monkeId, const std::string& itemName);

private:
    std::map<std::string, bool> availability;
    std::map<std::string, std::mutex> mutexes;
    std::map<std::string, std::condition_variable> cvs;
};

#endif //SO2_MONKEY_COOKS_KITCHEN_HPP

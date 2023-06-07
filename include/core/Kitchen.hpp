//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_KITCHEN_HPP
#define SO2_MONKEY_COOKS_KITCHEN_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>

class Kitchen {
public:
    Kitchen() :
    ovenAvailable{true},
    mixerAvailable{true},
    fridgeAvailable{true}
    {
        // constructor body
    }

    void useOven(uint32_t monkeId);
    void useMixer(uint32_t monkeId);
    void useFridge(uint32_t monkeId);

    void releaseOven();
    void releaseMixer();
    void releaseFridge();

private:
    std::mutex ovenMutex, mixerMutex, fridgeMutex;
    std::condition_variable ovenCV, mixerCV, fridgeCV;
    bool ovenAvailable, mixerAvailable, fridgeAvailable;
};

#endif //SO2_MONKEY_COOKS_KITCHEN_HPP

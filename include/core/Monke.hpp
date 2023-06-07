//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_MONKE_HPP
#define SO2_MONKEY_COOKS_MONKE_HPP

#include <iostream>
#include <thread>

#include <Kitchen.hpp>

class Monke {
public:
    Monke(uint32_t id, Kitchen& kitchen, uint32_t ovenOrder, uint32_t mixerOrder, uint32_t fridgeOrder)
    :
    id{id},
    kitchen{kitchen},
    ovenOrder{ovenOrder},
    mixerOrder{mixerOrder},
    fridgeOrder{fridgeOrder}{};

    void operator()();

private:
    uint32_t id;
    Kitchen& kitchen;
    uint32_t ovenOrder, mixerOrder, fridgeOrder;
};
#endif //SO2_MONKEY_COOKS_MONKE_HPP

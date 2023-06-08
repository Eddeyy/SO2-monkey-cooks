//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_MONKE_HPP
#define SO2_MONKEY_COOKS_MONKE_HPP

#include <iostream>
#include <thread>
#include <atomic>

#include <Kitchen.hpp>
#include <Recipe.hpp>
#include <utility>

class Monke {
public:
    Monke(uint32_t id, Kitchen &kitchen)
    :
            id{id},
            kitchen{kitchen} {};

    void operator()();

    void startHungerDecrement();

    void setNewRecipe(Recipe recipe)
    {
        this->recipe = std::move(recipe);
    };
private:
    uint32_t id;
    Kitchen& kitchen;
    Recipe recipe;
    std::atomic<int32_t> hungerLevel = 90;
    uint32_t eatingSpeed = rand() % 7 + 3;
};
#endif //SO2_MONKEY_COOKS_MONKE_HPP

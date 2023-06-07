//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_MONKE_HPP
#define SO2_MONKEY_COOKS_MONKE_HPP

#include <iostream>
#include <thread>

#include <Kitchen.hpp>
#include <Recipe.hpp>
#include <utility>

class Monke {
public:
    Monke(uint32_t id, Kitchen &kitchen, Recipe recipe)
    :
            id{id},
            kitchen{kitchen},
            recipe{std::move(recipe)} {};

    void operator()();

    void setNewRecipe(Recipe recipe)
    {
        this->recipe = std::move(recipe);
    };
private:
    uint32_t id;
    Kitchen& kitchen;
    Recipe recipe;
};
#endif //SO2_MONKEY_COOKS_MONKE_HPP

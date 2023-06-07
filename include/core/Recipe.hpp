//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_RECIPE_HPP
#define SO2_MONKEY_COOKS_RECIPE_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <mutex>

struct RecipeStep {
    const uint32_t secondsDuration;
    const std::string description;
    const std::string item;
};

class Recipe {
public:

    Recipe(std::queue<RecipeStep> steps):
    steps(std::move(steps))
    {

    }

    RecipeStep getNextStep() {

        if(!this ->steps.empty())
        {
            auto step = this->steps.front();
            this->steps.pop();
            return step;
        }
        return RecipeStep{0, "DONE", ""};
    }

private:
    std::queue<RecipeStep> steps;
};


#endif //SO2_MONKEY_COOKS_RECIPE_HPP

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
    uint32_t secondsDuration;
    std::string item;
};

class Recipe {
public:

    Recipe(std::queue<RecipeStep> steps, uint32_t eating_time, std::string name):
    steps(std::move(steps)),
    eating_time(eating_time),
    name{name}
    {

    }

    Recipe(){};

    RecipeStep getNextStep() {

        if(!this ->steps.empty())
        {
            auto step = this->steps.front();
            this->steps.pop();
            return step;
        }
        return RecipeStep{0, "DONE"};
    }

    const int getEatingTime() const {
        return eating_time;
    }

    void setEatingTime(uint32_t eatingTime) {
        eating_time = eatingTime;
    }

    const std::queue<RecipeStep> &getSteps() const {
        return steps;
    }

    void setSteps(const std::queue<RecipeStep> &steps) {
        Recipe::steps = steps;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        Recipe::name = name;
    }

private:
    std::queue<RecipeStep> steps;
    std::string name = "";
    uint32_t eating_time = 0;
};


#endif //SO2_MONKEY_COOKS_RECIPE_HPP

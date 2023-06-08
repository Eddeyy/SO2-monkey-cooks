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

struct RecipeStep
{
    uint32_t secondsDuration;
    std::string item;
};

class Recipe
{
public:

    Recipe(std::vector<RecipeStep> steps, uint32_t eating_time, std::string name) :
            steps(std::move(steps)),
            eating_time(eating_time),
            name{name}
    {};

    Recipe()
    {};

    RecipeStep getNextStep()
    {

        if (!this->steps.empty())
        {
            auto step = this->steps.front();
            this->steps.erase(steps.begin());
            return step;
        }
        return RecipeStep{0, "DONE"};
    }

    const int getEatingTime() const
    {
        return eating_time;
    }

    void setEatingTime(uint32_t eatingTime)
    {
        eating_time = eatingTime;
    }

    const std::vector<RecipeStep> &getSteps() const
    {
        return steps;
    }

    void setSteps(const std::vector<RecipeStep> &steps)
    {
        Recipe::steps = steps;
    }

    const std::string &getName() const
    {
        return name;
    }

    void setName(const std::string &name)
    {
        Recipe::name = name;
    }

private:
    std::vector<RecipeStep> steps;
    std::string name = "";
    uint32_t eating_time = 0;
};


#endif //SO2_MONKEY_COOKS_RECIPE_HPP

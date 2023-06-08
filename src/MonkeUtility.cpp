//
// Created by thero on 08.06.2023.
//
#include <MonkeUtility.hpp>

std::vector<std::string> MonkeUtility::splitString(const std::string &input, char delimiter)
{
    std::vector<std::string> result;
    std::size_t start = 0;
    std::size_t end = input.find(delimiter);

    while (end != std::string::npos)
    {
        result.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }

    result.push_back(input.substr(start));

    return result;
}

std::string MonkeUtility::toLowerCase(const std::string &str)
{
    std::string result = str;

    for (char &c: result)
    {
        c = std::tolower(c);
    }

    return result;
}

std::vector<Recipe> MonkeUtility::loadRecipes(const std::string &path) //TODO: handle errors in deserialization
{

    std::vector<Recipe> recipes;

    std::fstream file;
    file.open(path);

    if (!file.good())
    {
        throw MonkeException(200, "Error opening recipes file");
    }

    std::string line;

    std::string recipeName;
    std::vector<RecipeStep> recipeSteps;
    uint32_t eating_time;
    uint32_t value;

    while (std::getline(file, line))
    {
        if (line == "#")
        {
            recipeName = "";
            recipeSteps.clear();
            continue;
        }

        if (line.find("name:") == 0)
        {
            recipeName = line.substr(5);
            continue;
        }

        if (line.find("steps:") == 0)
        {
            std::vector<std::string> steps = splitString(line.substr(6), ',');

            for (auto &s: steps)
            {
                auto item = s.substr(1, s.rfind(" ") - 1);
                recipeSteps.push_back(RecipeStep{
                        std::stoul(s.substr(s.rfind(' '))),
                        item});
                s = item;
            }
            continue;
        }

        if (line.find("eating time:") == 0)
        {
            eating_time = std::stoul(line.substr(line.rfind(' ')));
            continue;
        }

        if(line.find("value:") == 0)
        {
            value = std::stoul(line.substr(line.rfind(' ')));
            Recipe recipe(recipeSteps, eating_time, recipeName, value);
            recipes.push_back(recipe);
        }

    }

    return recipes;
}

std::vector<std::string> MonkeUtility::loadKitchenItems(const std::string &path)
{

    std::vector<std::string> kitchenItems;

    std::ifstream file;
    file.open(path);
    if (!file.good())
    {
        throw MonkeException(200, "Error opening kitchen items file");
    }

    std::string line;
    while (std::getline(file, line))
    {
        kitchenItems.push_back(toLowerCase(line));
    }

    return kitchenItems;
}

void MonkeUtility::verifyItems(const Kitchen& kitchen)
{
    auto& items = kitchen.getAvailabilityMap();
    for (const auto &recipe: kitchen.getRecipes())
    {
        for(const auto &step : recipe.getSteps())
            if(MonkeUtility::findKeysWithSubstring(items, step.item).empty())
            {
                throw MonkeException(201, "Kitchen items mismatch - no item called " + step.item);
            }
    }
}

int MonkeUtility::getRandomIndex(int from, int to)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(from, to);

    return dist(gen);
}


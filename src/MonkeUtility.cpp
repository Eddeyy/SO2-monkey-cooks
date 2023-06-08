//
// Created by thero on 08.06.2023.
//
#include <MonkeUtility.hpp>

std::vector<std::string> MonkeUtility::splitString(const std::string &input, char delimiter)
{
    std::vector<std::string> result;
    std::size_t start = 0;
    std::size_t end = input.find(delimiter);

    while (end != std::string::npos) {
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

    for (char& c : result) {
        c = std::tolower(c);
    }

    return result;
}

std::vector<Recipe> MonkeUtility::loadRecipes(const std::string& path) //TODO: handle errors in deserialization
{

    std::vector<Recipe> recipes;

    std::fstream file;
    file.open(path);

    if(!file.good())
    {
        std::cout << "Error opening recipes file." << std::endl;
        exit(1);
    }

    std::string line;

    std::string recipeName;
    std::queue<RecipeStep> recipeSteps;
    uint32_t eating_time;

    while(std::getline(file, line))
    {
        if(line == "#")
        {
            recipeName = "";
            for(int i = 0; i < recipeSteps.size(); i++)
            {
                recipeSteps.pop();
            }
            continue;
        }

        if(line.find("name:") == 0)
        {
            recipeName = line.substr(5);
            continue;
        }

        if(line.find("steps:") == 0)
        {
            std::vector<std::string> steps = splitString(line.substr(6), ',');

            for(auto& s : steps)
            {
                auto item = s.substr(1, s.rfind(" ") - 1);
                recipeSteps.push(RecipeStep{
                        std::stoul(s.substr(s.rfind(' '))),
                        item});
                s = item;
            }
            continue;
        }

        if(line.find("eating time:") == 0)
        {
            eating_time = std::stoul(line.substr(line.rfind(' ')));
            Recipe recipe(recipeSteps, eating_time, recipeName);
            recipes.push_back(recipe);
            continue;
        }

    }

    return recipes;
}

std::vector<std::string> MonkeUtility::loadKitchenItems(const std::string& path) {

    std::vector<std::string> kitchenItems;

    std::ifstream file;
    file.open(path);
    if(!file.good())
    {
        std::cout << "Error opening kitchen items file." << std::endl;
        exit(1);
    }

    std::string line;
    while(std::getline(file, line))
    {
        kitchenItems.push_back(toLowerCase(line));
    }

    return kitchenItems;
}

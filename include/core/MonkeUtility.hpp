//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_MONKEUTILITY_HPP
#define SO2_MONKEY_COOKS_MONKEUTILITY_HPP

#include <iostream>
#include <fstream>
#include <queue>
#include "Recipe.hpp"

namespace MonkeUtility {

    std::vector<std::string> splitString(const std::string& input, char delimiter);
    std::string toLowerCase(const std::string& str);
    std::vector<Recipe> loadRecipes(const std::string& path = "./resources/recipes");
    std::vector<std::string> loadKitchenItems(const std::string& path = "./resources/kitchenitems");

}

#endif //SO2_MONKEY_COOKS_MONKEUTILITY_HPP

//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_MONKEUTILITY_HPP
#define SO2_MONKEY_COOKS_MONKEUTILITY_HPP

#include <iostream>
#include <fstream>
#include <queue>
#include <Recipe.hpp>
#include <Kitchen.hpp>

namespace MonkeUtility {

    std::vector<std::string> splitString(const std::string& input, char delimiter);
    std::string toLowerCase(const std::string& str);
    std::vector<Recipe> loadRecipes(const std::string& path = "./resources/recipes");
    std::vector<std::string> loadKitchenItems(const std::string& path = "./resources/kitchenitems");
    void verifyItems(const Kitchen& kitchen);

    template <typename T>
    std::vector<std::string> findKeysWithSubstring(const std::map<std::string, T> &myMap, const std::string &substring)
    {
        std::vector<std::string> keysWithSubstring;

        for (const auto& pair : myMap) {
            auto key = MonkeUtility::toLowerCase(pair.first);
            if (key.find(substring) != std::string::npos) {
                keysWithSubstring.push_back(pair.first);
            }
        }

        return keysWithSubstring;
    }


    class MonkeException : public std::runtime_error {
    private:
        int errorCode;

    public:
        MonkeException(int code, const std::string& what = "Undefined runtime error thrown.") :
                std::runtime_error{what},
                errorCode{code} {}

        int getErrorCode() const {
            return errorCode;
        }
    };
}

#endif //SO2_MONKEY_COOKS_MONKEUTILITY_HPP

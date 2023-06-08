#include <iostream>
#include <main.hpp>

int main() {
try {
    const uint32_t MONKE_NUM = 6;

    srand(time(NULL));

    Kitchen kitchen(MonkeUtility::loadKitchenItems());
    std::vector<std::thread> chefThreads;
    std::vector<Recipe> recipes = MonkeUtility::loadRecipes();

    for (int i = 0; i < MONKE_NUM; i++) {
        chefThreads.emplace_back(Monke(i + 1, kitchen, recipes.front()));
    }

    for (auto &thread: chefThreads) {
        thread.join();
    }

} catch(std::exception& e) {
    std::cout << e.what();
}

    return 0;
}

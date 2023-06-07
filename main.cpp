#include <iostream>
#include <main.hpp>

int main() {
try {
    const uint32_t MONKE_NUM = 5;

    Kitchen kitchen({"Oven", "Cutting board", "Mixer", "Fridge"});
    std::vector<std::thread> chefThreads;

    for (int i = 0; i < MONKE_NUM; i++) {
        chefThreads.emplace_back(Monke(i + 1, kitchen, Recipe(std::queue<RecipeStep>({
            {1, "", "Fridge"},
            {2, "", "Cutting board"},
            {2, "", "Mixer"},
            {3, "", "Oven"},
        }))));
    }

    for (auto &thread: chefThreads) {
        thread.join();
    }

} catch(std::exception& e) {
    std::cout << e.what();
}

    return 0;
}

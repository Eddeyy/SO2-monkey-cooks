#include <iostream>
#include <main.hpp>

int main() {
try {
    const uint32_t MONKE_NUM = 5;

    Kitchen kitchen({"a", "b", "c"});
    std::vector<std::thread> chefThreads;

    for (int i = 0; i < MONKE_NUM; i++) {
        chefThreads.emplace_back(Monke(i + 1, kitchen, Recipe(std::queue<RecipeStep>({
            {1, "", "a"},
            {1, "", "b"},
            {1, "", "c"},
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

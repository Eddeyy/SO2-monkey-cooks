#include <iostream>
#include <main.hpp>

int main() {

    const uint32_t MONKE_NUM = 5;

    Kitchen kitchen;
    std::vector<std::thread> chefThreads;

    for (int i = 0; i < MONKE_NUM; i++)
    {
        chefThreads.emplace_back(Monke(i + 1, kitchen, i % 3, (i + 1) % 3,(i + 2) % 3));
    }

    for (auto& thread : chefThreads)
    {
        thread.join();
    }

    return 0;
}

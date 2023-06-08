#include <iostream>
#include <main.hpp>

int main() {
    try
    {
        const uint32_t MONKE_NUM = 6;

        srand(time(NULL));

        Kitchen kitchen(MonkeUtility::loadKitchenItems(), MonkeUtility::loadRecipes());
        std::vector<std::thread> chefThreads;
        MonkeUtility::verifyItems(kitchen);

        for (int i = 0; i < MONKE_NUM; i++)
        {
            auto chefPtr = std::make_shared<Monke>(i + 1, kitchen);
            chefThreads.emplace_back(std::thread([chefPtr]() { (*chefPtr)(); }));
        }

        for (auto &thread: chefThreads)
        {
            thread.join();
        }

    }
    catch(MonkeUtility::MonkeException& e)
    {
        std::cout << e.what();
        return e.getErrorCode();
    }
    catch(std::exception& e)
    {
        std::cout << e.what();
        return 199;
    }

    return 0;
}

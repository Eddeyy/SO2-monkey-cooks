#include <iostream>
#include <main.hpp>

int main() {

    uint32_t MONKE_NUM = 6;
    uint32_t HUNGER_DEPLETION_TIME = 6;
    int32_t HUNGER_DEPLETION_AMOUNT = 10;

    std::cout << "Provide amount of monkes: ";
    std::cin >> MONKE_NUM;
    std::cout << "\nProvide hunger depletion time (seconds): ";
    std::cin >> HUNGER_DEPLETION_TIME;
    std::cout << "\nProvide hunger depletion value (from 1 to 99): ";
    std::cin >> HUNGER_DEPLETION_AMOUNT;
    try
    {

        srand(time(NULL));

        Kitchen kitchen(MonkeUtility::loadKitchenItems(), MonkeUtility::loadRecipes());
        std::vector<std::thread> chefThreads;
        MonkeUtility::verifyItems(kitchen);

        for (int i = 0; i < MONKE_NUM; i++)
        {
            auto chefPtr = std::make_shared<Monke>(i + 1, kitchen, HUNGER_DEPLETION_TIME, HUNGER_DEPLETION_AMOUNT);
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

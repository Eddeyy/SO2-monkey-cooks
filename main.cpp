#include <iostream>
#include <main.hpp>

std::vector<std::shared_ptr<Monke>> chefs;

int main()
{

    uint32_t MONKE_NUM = 6;
    uint32_t HUNGER_DEPLETION_TIME = 6;
    int32_t HUNGER_DEPLETION_AMOUNT = 10;

    std::cout << "Provide amount of monkes: ";
    std::cin >> MONKE_NUM;
    std::cout << "\nProvide hunger depletion time (seconds): ";
    std::cin >> HUNGER_DEPLETION_TIME;
    std::cout << "\nProvide hunger depletion value (from 1 to 99): ";
    std::cin >> HUNGER_DEPLETION_AMOUNT;

    WINDOW *w = initscr();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(0);
    keypad(w, TRUE);
    nodelay(w, TRUE);
    cbreak();
    refresh();

    Kitchen kitchen = Kitchen(MonkeUtility::loadKitchenItems(), MonkeUtility::loadRecipes());
    std::thread displayThread(display, std::ref(kitchen), std::ref(chefs));

    try
    {

        srand(time(NULL));

        std::vector<std::thread> chefThreads;
        MonkeUtility::verifyItems(kitchen);

        for (int i = 0; i < MONKE_NUM; i++)
        {
            auto chefPtr = std::make_shared<Monke>(i + 1, kitchen, &chefs, HUNGER_DEPLETION_TIME, HUNGER_DEPLETION_AMOUNT);
            chefs.emplace_back(chefPtr);
            chefThreads.emplace_back([chefPtr]()
                                     { (*chefPtr)(); });
        }

        for (auto &thread : chefThreads)
        {
            thread.join();
        }
    }
    catch (MonkeUtility::MonkeException &e)
    {
        std::cout << e.what();
        return e.getErrorCode();
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
        return 199;
    }

    endwin();
    return 0;
}

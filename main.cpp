#include <iostream>
#include <main.hpp>
#include <ncurses.h>
#include <unistd.h>

bool displayOn = true;
std::vector<std::shared_ptr<Monke>> chefs;


void display() {
    noecho();

    while (displayOn)
    {
        clear();
        mvprintw(0,0,"Malpo mlyniarzu, co robisz. Press ESC to exit");
        mvprintw(1, 0,"______________________________________________________________________________________________________________");
        for (int i = 0; i < chefs.size(); i++) {
            Monke *monke = chefs[i].get();
            mvprintw(i+2, 1, "Monke %d", monke->getId());
            mvprintw(i+2, 0, "|");
            mvprintw(i+2, 40, "Hungary level:");
            mvprintw(i+2, 55, "__________");
            mvprintw(i+2, 15, monke->getStatus().c_str());
            for(int j=0; j<monke->getHungerLevel(); j++){
                mvprintw(i+2, 55+(j/10), "#");
            }
            mvprintw(i+2, 70, "Recipe:");
            mvprintw(i+2, 80, monke->getRecipe().getName().c_str());
            mvprintw(i+2, 100, "%d", monke->getTimeLeft());
            mvprintw(i+2, 110, "|");
        }

        mvprintw(7, 0,"______________________________________________________________________________________________________________");
        refresh();
        usleep(10000);
    }

}

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

    initscr();
    cbreak();
    refresh();

    std::thread displayThread(display);

    try
    {

        srand(time(NULL));

        Kitchen kitchen(MonkeUtility::loadKitchenItems(), MonkeUtility::loadRecipes());
        std::vector<std::thread> chefThreads;
        MonkeUtility::verifyItems(kitchen);

        for (int i = 0; i < MONKE_NUM; i++)
        {
            auto chefPtr = std::make_shared<Monke>(i + 1, kitchen, HUNGER_DEPLETION_TIME, HUNGER_DEPLETION_AMOUNT);
            chefs.emplace_back(chefPtr.get());
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

    // displayThread.detach();

    endwin();
    return 0;
}

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
        erase();
        mvprintw(0,0,"Malpo mlyniarzu, co robisz. Press ESC to exit");
        mvprintw(1, 0,"_____________________________________________________________________________________________");
        mvprintw(2, 0,"|____________Monkey_does____________________|_Monkey_hungry_|_______Recipe_______|_Time_left_|");
        int y = 3;
        for (int i = 0; i < chefs.size(); i++) {
            Monke *monke = chefs[i].get();
            mvprintw(y, 1, "Monke %d", monke->getId());
            mvprintw(y, 0, "|");
            mvprintw(y, 46, "__________");
            if(y<12)
                mvprintw(y, 9, monke->getStatus().c_str());
            else
                mvprintw(y, 10, monke->getStatus().c_str());
            for(int j=0; j<monke->getHungerLevel(); j++){
                mvprintw(y, 46+(j/10), "#");
            }
            mvprintw(y, 57, "%d%%",monke->getHungerLevel());
            mvprintw(y, 62, monke->getRecipe().getName().c_str());
            mvprintw(y, 87, "%d", monke->getTimeLeft());
            mvprintw(y, 44, "|");
            mvprintw(y, 60, "|");
            mvprintw(y, 81, "|");
            mvprintw(y, 93, "|");
            y++;
        }

        mvprintw(y, 0,"|____________________________________________________________________________________________|");
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
            auto chefPtr = std::make_shared<Monke>(i + 1, kitchen, &chefs, HUNGER_DEPLETION_TIME, HUNGER_DEPLETION_AMOUNT);
            chefs.emplace_back(chefPtr);
            chefThreads.emplace_back([chefPtr]() { (*chefPtr)(); });
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

    endwin();
    return 0;
}

#include <iostream>
#include <main.hpp>
#include <ncurses.h>
#include <unistd.h>

bool displayOn = true;
std::vector<std::shared_ptr<Monke>> chefs;

void display(Kitchen &kitchen)
{
    noecho();

    bool isMonkeyDesc = false;
    bool isItemDesc = false;
    int monkeClicked = 0;
    std::string itemClicked = "";
    int ch = 0;

    while (displayOn)
    {
        erase();

        ch = getch();

        MEVENT mEvent;
        if (getmouse(&mEvent) == OK)
        {
            if (mEvent.y < chefs.size() + 3 && mEvent.y > 2)
            {
                isMonkeyDesc = true;
                monkeClicked = mEvent.y - 2;
            }
            else
            {
                isMonkeyDesc = false;
                monkeClicked = 0;
                if (mEvent.y < chefs.size() + 5 + kitchen.getItemAmount().size() && mEvent.y > chefs.size() + 4)
                {
                    isItemDesc = true;
                    itemClicked = kitchen.getItemAmount().at(mEvent.y - (chefs.size() + 5)).first;
                }
                else
                {
                    isItemDesc = false;
                    itemClicked = "";
                }
            }
        }

        mvprintw(0, 0, "Malpo mlyniarzu, co robisz. Press ESC to exit");
        mvprintw(1, 0, "_____________________________________________________________________________________________");
        mvprintw(2, 0, "|____________Monkey_does____________________|_Monkey_hungry_|_______Recipe_______|_Time_left_|");
        int y = 3;
        for (int i = 0; i < chefs.size(); i++)
        {
            Monke *monke = chefs[i].get();
            mvprintw(y, 1, "Monke %d", monke->getId());
            mvprintw(y, 0, "|");
            mvprintw(y, 46, "__________");
            if (y < 12)
                mvprintw(y, 9, monke->getStatus().c_str());
            else
                mvprintw(y, 10, monke->getStatus().c_str());
            for (int j = 0; j < monke->getHungerLevel(); j++)
            {
                mvprintw(y, 46 + (j / 10), "#");
            }
            mvprintw(y, 57, "%d%%", monke->getHungerLevel());
            mvprintw(y, 62, monke->getRecipe().getName().c_str());
            mvprintw(y, 87, "%d", monke->getTimeLeft());
            mvprintw(y, 44, "|");
            mvprintw(y, 60, "|");
            mvprintw(y, 81, "|");
            mvprintw(y, 93, "|");
            y++;
        }

        mvprintw(y, 0, "|____________________________________________________________________________________________|");
        auto itemAmount = kitchen.getItemAmount();

        mvprintw(++y, 0, "|___________________________________________|________________________________________________|");
        int yForReturn = y;
        int yUsage = y;
        if (isMonkeyDesc)
        {
            mvprintw(y, 14, "Monkey_%d_usage", monkeClicked);
            for (auto &pair : chefs[monkeClicked - 1]->getTimesUsingItem())
            {
                mvprintw(++y, 1, "Monke %d used %s : %d times", monkeClicked, pair.first.c_str(), pair.second);
                mvprintw(y, 0, "|");
                mvprintw(y, 44, "|");
                mvprintw(y, 93, "|");
            }
        }
        else if (isItemDesc)
        {
            mvprintw(y, 14, "%s_usage", itemClicked.c_str());
            for (int i = 0; i < kitchen.getItemAmount(itemClicked); i++)
            {
                std::string itemFullName = itemClicked + " " + std::to_string(i+1);
                mvprintw(++y, 1, "%s was used %d times", itemFullName.c_str(), kitchen.getItemTimesUsed(itemFullName));
                mvprintw(y, 0, "|");
                mvprintw(y, 44, "|");
                mvprintw(y, 93, "|");
            }
            
        }
        else
        {
            mvprintw(y, 14, "All_kitchen_items");
            for (auto &pair : kitchen.getHowManyUsedRightNow())
            {
                mvprintw(++y, 1, "%ss: %d/%d", pair.first.c_str(), pair.second, itemAmount.at((y - (chefs.size() + 4))).second);
                mvprintw(y, 0, "|");
                mvprintw(y, 44, "|");
                mvprintw(y, 93, "|");
            }
        }
        yUsage = y;

        y = yForReturn;

        mvprintw(y, 65, "Helping_monkes:_%d", kitchen.helpingMonkes);

        for (auto &monkePair : kitchen.getHelpingMonkes(chefs))
        {
            mvprintw(++y, 50, "Monke %d is helping monke %d", monkePair.first, monkePair.second);
            mvprintw(y, 0, "|");
            mvprintw(y, 44, "|");
            mvprintw(y, 93, "|");
        }

        if (yUsage > y)
            y = yUsage;

        mvprintw(++y, 0, "|____________________________________________________________________________________________|");
        refresh();
        usleep(10000);
    }
}

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
    std::thread displayThread(display, std::ref(kitchen));

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

#include <MonkeDisplay.hpp>
#include <Monke.hpp>
#include <ncurses.h>
#include <unistd.h>



void display(Kitchen &kitchen, std::vector<std::shared_ptr<Monke>>& chefs)
{
    noecho();

    bool isMonkeyDesc = false;
    bool isItemDesc = false;
    int monkeClicked = 0;
    std::string itemClicked = "";
    int ch = 0;

    while (true)
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
                std::string itemFullName = kitchen.getItemAmount(itemClicked) > 1? itemClicked + " " + std::to_string(i+1) : itemClicked;
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
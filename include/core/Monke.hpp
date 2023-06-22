//
// Created by thero on 07.06.2023.
//

#ifndef SO2_MONKEY_COOKS_MONKE_HPP
#define SO2_MONKEY_COOKS_MONKE_HPP

#include <iostream>
#include <thread>
#include <atomic>

#include <Kitchen.hpp>
#include <Recipe.hpp>
#include <utility>

enum MonkeStatus {
    resting,
    cooking,
    hungry,
    waiting,
    eating
};

class Monke {
public:
    Monke(
            uint32_t id,
            Kitchen &kitchen,
            std::vector<std::shared_ptr<Monke>>* monkes,
            int32_t hungeringTime = 6,
            int32_t hungerDepletionAmount = 10 )
    :
            id{id},
            kitchen{kitchen},
            allMonkes{monkes},
            hungering_time{hungeringTime},
            hunger_depletion_amount{hungerDepletionAmount}
            {};

    void operator()();


    void setNewRecipe(Recipe recipe)
    {
        this->recipe = std::move(recipe);
    };

    std::string getStatus() {
        return this->status;
    };

    int32_t getHungerLevel() {
        return this->hunger_level;
    };

    Recipe getRecipe() {
        return this->recipe;
    };

    int32_t getTimeLeft() {
        return this->time_left;
    }
    
    int32_t getId() {
        return this->id;
    }

    void setStatus(std::string status)
    {
        this->status = status;
    }

    bool isHelping()
    {
        return this->status.find("helping") != std::string::npos;
    }

    int getHelpedMonkeId()
    {
        return this->helpedMonkeId;
    }

    int getHelpingMonkeId()
    {
        return this->helpingMonke? helpingMonke.load()->getId() : -1;
    }

    const std::map<std::string, uint32_t>& getTimesUsingItem()
    {
        return this->timesUsingItem;
    }

private:
    uint32_t id;
    Kitchen& kitchen;
    Recipe recipe;
    std::atomic<int32_t> hunger_level = 90;
    int32_t eating_speed = rand() % 7 + 3;
    int32_t hungering_time;
    int32_t hunger_depletion_amount;
    std::atomic<int32_t> time_left = 0;
    std::string status = "idle";

    std::map<std::string, uint32_t> timesUsingItem;

    std::mutex mutex;
    std::condition_variable cv;

    std::vector<std::shared_ptr<Monke>>* allMonkes;

    std::atomic<Monke*> helpingMonke = nullptr;
    
    int helpedMonkeId = -1;

    bool isBeingHelped = false;
    bool isCooking = false;

    void cook();
    bool try_help_another();
    void eat_and_rest();
    std::shared_ptr<Monke> findAvailableMonkeToHelp();
    void adjustRecipeForHelping(const std::shared_ptr<Monke>& monke);
    void waitForMonkeToFinishCooking(const std::shared_ptr<Monke>& monke, std::unique_lock<std::mutex>& lock);
    bool should_search_for_recipe();
    bool should_help_another_monke();
    void claim_item_for_time(const std::string &itemName, const int32_t& duration, MonkeStatus status, const uint32_t& food_value = 0);
    void start_hunger_decrement();
    void sleep_for(const int32_t& seconds);
};

#endif //SO2_MONKEY_COOKS_MONKE_HPP

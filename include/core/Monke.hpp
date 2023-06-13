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

private:
    uint32_t id;
    Kitchen& kitchen;
    Recipe recipe;
    std::atomic<int32_t> hunger_level = 90;
    int32_t eating_speed = rand() % 7 + 3;
    int32_t hungering_time;
    int32_t hunger_depletion_amount;
    std::atomic<int32_t> time_left = 0;
    std::string status = "NONE";

    std::mutex mutex;
    std::condition_variable cv;

    std::vector<std::shared_ptr<Monke>>* allMonkes;

    bool isBeingHelped = false;
    bool isCooking = false;

    void cook();
    bool try_help_another();

    void claim_item_for_time(const std::string &itemName, const int32_t& duration, MonkeStatus status, const uint32_t& food_value = 0);
    void start_hunger_decrement();
    void sleep_for(const int32_t& seconds);
};

#endif //SO2_MONKEY_COOKS_MONKE_HPP

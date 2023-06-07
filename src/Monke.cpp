//
// Created by thero on 07.06.2023.
//

#include <Monke.hpp>

void Monke::operator()() {
    this->kitchen.useOven(this->id);

    // Symulowanie korzystania z pieca
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Chef " << this->id << " finished using the oven." << std::endl;

    this->kitchen.releaseOven();
    this->kitchen.useMixer(this->id);

    // Symulowanie korzystania z miksera
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Chef " << this->id << " finished using the mixer." << std::endl;

    this->kitchen.releaseMixer();
    this->kitchen.useFridge(this->id);

    // Symulowanie korzystania z lodówki
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Chef " << this->id << " finished using the fridge." << std::endl;

    this->kitchen.releaseFridge();
}

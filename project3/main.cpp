#include <iostream>

#include "cityMatrix.h"
#include "Menu.h"

int main() {
    cityMatrix city_matrix = cityMatrix();
    int choice;
    std::cout << "Menu:"
                 "\n1. Launch program in interactive mode\n"
                 "2. Launch program in research mode\n"
                 "Your choice:";
    std::cin >> choice;

    while(true){
        switch (choice) {
            case 1:
                Menu::startMenu(city_matrix);
            break;
            case 2:
                Menu::start_research(city_matrix);
            break;
            default:
                std::cout << "Invalid choice.";
            return -2;
        }
    }
}

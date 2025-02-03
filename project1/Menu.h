//
// Created by krisk on 12/10/2024.
//

#ifndef MENU_H
#define MENU_H



class Menu {
public:
    void static startMenu(cityMatrix &city_matrix);
    void static algorithmMenu(cityMatrix &city_matrix);

    void static askForRandomMatrix(cityMatrix &city_matrix);
    void static askForLoadMatrix(cityMatrix &city_matrix);

    void static start_research(cityMatrix &city_matrix);
};



#endif //MENU_H

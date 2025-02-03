//
// Created by krisk on 12/10/2024.
//

#ifndef MENU_H
#define MENU_H
#include "TSP.h"


class Menu {

public:
    void static processFiles( const std::string &directory, cityMatrix &city_matrix, const std::string &outputFilename);

    void static askForFileName(cityMatrix &city_matrix);

    void static askForStopCriterion(cityMatrix &city_matrix);

    static void askForTemperatureFactor(cityMatrix & city_matrix);

    void static startMenu(cityMatrix &city_matrix);
    void static algorithmMenu(cityMatrix &city_matrix);

    void static saveTourToFile(Tour tour, std::string filename);
    void static askForRandomMatrix(cityMatrix &city_matrix);
    void static askForLoadMatrix(cityMatrix &city_matrix);

    static void askForLoadMatrixTXT(cityMatrix &city_matrix);

    void static start_research(cityMatrix &city_matrix);
};



#endif //MENU_H

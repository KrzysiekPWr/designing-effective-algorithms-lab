//
// Created by krisk on 15/10/2024.
//

#ifndef TSP_H
#define TSP_H
#include "cityMatrix.h"

struct Tour {
    std::vector<int> path;
    int totalCost;
};

class TSP {
    static int calculatePathCost(cityMatrix &city_matrix, int* path);

    static void calculatePermutations(cityMatrix &city_matrix, int start, int *path, int *best_path, int &min_cost);

public:
    static int calculatePathCost(cityMatrix &city_matrix, Tour& tour);

    static Tour tabuSearch(cityMatrix &city_matrix, std::string output_filename);

    static Tour simulatedAnnealing(cityMatrix &city_matrix, std::string output_filename);

    static void bruteForceNew(cityMatrix &city_matrix);

    static void bruteForce(cityMatrix &city_matrix);

    static void dynamicProgrammingTSP(cityMatrix &city_matrix);

    static void branchAndBound(cityMatrix &city_matrix);

    static int global_min_cost;
    static int *global_min_path;
};



#endif //TSP_H

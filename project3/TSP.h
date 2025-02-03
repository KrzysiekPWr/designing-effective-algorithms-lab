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

enum class mutationType {
    INVERSION,
    SWAP
};

enum class crossoverType {
    OX,
    PMX
};

class TSP {
    static int calculatePathCost(cityMatrix &city_matrix, int* path);

    static void calculatePermutations(cityMatrix &city_matrix, int start, int *path, int *best_path, int &min_cost);

public:
    static int calculatePathCost(cityMatrix &city_matrix, Tour& tour);

    static Tour geneticAlgorithm(cityMatrix &city_matrix, int population_size, crossoverType crossover_type, mutationType mutation_type, std::string output_filename);
};



#endif //TSP_H

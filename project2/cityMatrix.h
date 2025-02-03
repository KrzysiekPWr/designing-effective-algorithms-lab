//
// Created by krisk on 12/10/2024.
//

#ifndef CITYMATRIX_H
#define CITYMATRIX_H
#include <string>
#include <vector>
#include <chrono>
using namespace std::chrono_literals;
class cityMatrix {
public:
    int ** matrix; // 2D array to store the matrix
    int size; // Size of the matrix
    cityMatrix(int size); // Constructor
    cityMatrix();
    std::chrono::duration<long long> stop_criterion_time = 60s;
    struct Tour {
        std::vector<int> path;
        int totalCost;
    };
    Tour bestTour;
    double starting_temperature = -1;
    mutable double cooling_rate = 0.999;

    void showMatrix(); // Function to display the matrix
    void loadMatrix(const std::string &filename, int number_of_cities, bool load_data); // Function to load the matrix from a file
    void loadMatrixFromTXT(const std::string &filename);

    void clearMatrix() const;

    void initializeMatrix(int size);

    void createRandomMatrix(int num_cities);
};



#endif //CITYMATRIX_H

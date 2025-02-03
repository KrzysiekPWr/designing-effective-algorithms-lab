//
// Created by krisk on 12/10/2024.
//

#ifndef CITYMATRIX_H
#define CITYMATRIX_H
#include <string>


class cityMatrix {
public:
    int ** matrix; // 2D array to store the matrix
    int size; // Size of the matrix
    cityMatrix(int size); // Constructor
    cityMatrix();

    void showMatrix(); // Function to display the matrix
    void loadMatrix(const std::string &filename); // Function to load the matrix from a file
    void clearMatrix() const;

    void initializeMatrix(int size);

    void createRandomMatrix(int num_cities);
};



#endif //CITYMATRIX_H

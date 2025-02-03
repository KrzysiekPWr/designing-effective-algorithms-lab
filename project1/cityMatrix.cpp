//
// Created by krisk on 12/10/2024.
//

#include "cityMatrix.h"

#include <fstream>
#include <iostream>


cityMatrix::cityMatrix(int size) {
    this->size = size;
    this->matrix = new int*[size];
    for(int i = 0; i < size; i++) {
        this->matrix[i] = new int[size];
    }
}

cityMatrix::cityMatrix() {
    this->size = 0;
    this->matrix = nullptr;
}

//-----------------------------------------------------

void cityMatrix::showMatrix() {
    printf("\n\nThe current matrix:\n");
    for(int row = 0; row < this->size; row++) {
        for(int col = 0; col < this->size; col++) {
            printf("%3d ", this->matrix[row][col]);
        }
        std::cout << std::endl;
    }
}

void cityMatrix::loadMatrix(const std::string &filename) {
    if (matrix != nullptr) clearMatrix();  // Clear the matrix if it was already initialized
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(-1);
    }

    file >> size;
    // Initialize the matrix (create empty space for data)
    initializeMatrix(size);

    // Load data into the matrix from the file
    for(int row = 0; row < this->size; row++) {
        for(int col = 0; col < this->size; col++) {
            file >> this->matrix[row][col];
        }
    }
    file.close();
}

void cityMatrix::clearMatrix() const {
    for(int i = 0; i < size; i++) {
        delete[] this->matrix[i];
    }
    delete[] this->matrix;
}

void cityMatrix::initializeMatrix(int size) {
    this->matrix = new int*[size];
    for(int col = 0; col < size; col++) {
        this->matrix[col] = new int[size];
    }
}

void cityMatrix::createRandomMatrix(int num_cities) {
    srand(time(NULL));

    if (matrix != nullptr) clearMatrix();  // Clear the matrix if it was already initialized
    this->size = num_cities;
    initializeMatrix(num_cities);

    for(int row = 0; row < num_cities; row++) {
        for(int col = 0; col < num_cities; col++) {
            if (row == col) {
                this->matrix[row][col] = -1;
            } else {
                this->matrix[row][col] = rand() % (num_cities*10) + 1; // Random number between 1 and num_cities*100
            }
        }
    }
}

//
// Created by krisk on 15/10/2024.

#include "TSP.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

void shell_sort(int size, int *array, int start) {
    int gap = (size - start) / 2;

    while (gap > 0) {
        for (int i = start + gap; i < size; i++) {
            int j = i;
            while (j >= start + gap && array[j] < array[j - gap]) {
                int temp = array[j];
                array[j] = array[j - gap];
                array[j - gap] = temp;
                j -= gap;
            }
        }
        gap /= 2;
    }
}

/**
 * Calculates the cost of the path along the given path
 * @param city_matrix object that contains the matrix of cities
 * @param path array of cities
 * @return cost of the path
 */
int TSP::calculatePathCost(cityMatrix &city_matrix, int *path) {

    int cost = 0;
    //calculate cost of the path
    for (int i = 0; i < city_matrix.size; i++) {
        cost += city_matrix.matrix[path[i]][path[i+1]];
    }
    return cost;
}

void TSP::calculatePermutations(cityMatrix &city_matrix, int start, int *path, int *best_path, int &min_cost) {
    if (start == city_matrix.size) { //if we reach the end city
        //calculate the cost of current path
        int current_cost = calculatePathCost(city_matrix, path);

        //update minimum cost
        if (current_cost < min_cost) {
            min_cost = current_cost;
            for (int i = 0; i < city_matrix.size; i++) {
                best_path[i] = path[i];
            }
        }
        return;
    }

    for (int i = start; i < city_matrix.size; i++) {
        std::swap(path[start], path[i]);
        calculatePermutations(city_matrix, start + 1, path, best_path, min_cost);
        std::swap(path[start], path[i]); // backtrack (restore the original order)
    }
}

bool nextPermutation(int *path, int n) {
    if (n == 1) {
        return false;
    }

    int last_peak = -1;
    for (int i = 2; i < n; i++) { // find the last peak
        if (path[i] > path[i-1]) {
            last_peak = i;
        }
    }

    if (last_peak == -1) { // if there is no peak, return that there are no more permutations
        return false;
    }

    // find an element that lies on the right side of the peak (so on the descending slope
    // that is greater than the element left of the peak and smaller than the peak itself
    int index = last_peak; // index stays the same if no such element is found
    for (int i = last_peak; i < n; i++) {
        if (path[i] > path[last_peak-1] && path[i] < path[index]) {
            index = i;
        }
    }
    std::swap(path[last_peak-1], path[index]);
    shell_sort(n, path, last_peak);
    return true;
}

void TSP::bruteForceNew(cityMatrix &city_matrix) {
    int num_cities = city_matrix.size;
    int path[num_cities + 1];
    int min_cost = INT_MAX;
    int best_path[num_cities];

    // Initialize the path with the first permutation
    for (int i = 0; i < num_cities; i++) {
        path[i] = i;
    }
    path[num_cities] = 0; // Return to the starting city

    // Generate permutations and find minimum cost path iteratively
    do {
        // Calculate the cost of the current path
        int current_cost = calculatePathCost(city_matrix, path);
        if (current_cost < min_cost) {
            min_cost = current_cost;
            for (int i = 0; i < num_cities; i++) {
                best_path[i] = path[i]; // Update the best path
            }
        }

    } while (nextPermutation(path, num_cities));

    // Output the best path and its cost
    std::cout << "Minimum cost: " << min_cost << std::endl;
    std::cout << "Path: ";
    std::cout << "0 ";
    for (int i = 1; i < num_cities; i++) {

        std::cout << "-> " << best_path[i] << " ";
    }
    std::cout << "-> 0"; // add the starting city to the end
    std::cout << std::endl;

}

/**
 * Solves the ATSP problem using brute force method.
 * @param city_matrix object that contains the matrix of cities
 * @return the shortest path + it's cost
 */
void TSP::bruteForce(cityMatrix &city_matrix) {
    int num_cities = city_matrix.size;
    int path[num_cities + 1];
    int min_cost = INT_MAX;
    int best_path[num_cities];

    // Initialize the path with the first permutation
    for (int i = 0; i < num_cities; ++i) {
        path[i] = i;
    }
    path[num_cities] = 0; // Return to the starting city

    // Generate all permutations and calculate the cost
    calculatePermutations(city_matrix, 1, path, best_path, min_cost);

    // Output the best path and its cost
    std::cout << "Minimum cost: " << min_cost << std::endl;
    std::cout << "Path: ";
    for (int i = 0; i < num_cities; i++) {
        std::cout << best_path[i] << " ";
    }
    std::cout << 0; // add the starting city to the end
    std::cout << std::endl;

}

void printPath(cityMatrix city_matrix, int** path_array) {
    int mask = 1, pos = 0; // Start at the first city
    std::cout << "Minimal path: 0 ";

    for (int i = 0; i < city_matrix.size - 1; i++) {
        pos = path_array[pos][mask];
        std::cout << "-> " << pos << " ";
        mask |= (1 << pos); // Mark this city as visited
    }
    std::cout << "-> 0" << std::endl; // Return to start;
}

/**
 * Fuction that searches for all possible solutions for the ATSP problem and returns the minimal cost
 * @param city_matrix - 2D array of cities from cityMatrix object
 * @param mask - mask that represents the visited cities - 0001 - city 0 is visited, 0011 - city 0 and 1 are visited
 * @param city_position - current city position in the path
 * @param path_array - array of cities that represent the paths
 * @param cost_array - 2D array that stores the cost for the current mask and city position
 * @return minimal cost of all paths
 */
int searchAllSolutions(cityMatrix &city_matrix, int mask, int city_position, int **path_array, int **cost_array) {
    if (mask == (1<<city_matrix.size) - 1){ //if all cities are visited
        return city_matrix.matrix[city_position][0]; // return the cost of the starting city
    }

    if (cost_array[city_position][mask] != -1) { //if the cost for the current mask and city position is already calculated
        return cost_array[city_position][mask];
    }


    int min_cost = INT_MAX;
    for (int city_to_visit = 0; city_to_visit < city_matrix.size; city_to_visit++) { //iterate through all cities
        if ((mask & (1<<city_to_visit)) == 0) {
            //if the city is not visited - 1011 & ((1 << 1)=0010) = 0010 but 1011 & ((1 << 2)=0100) = 0000
            int new_mask = (mask | (1<<city_to_visit)); //mark the city as visited (1011 | 0100 = 1111)
            if (city_matrix.matrix[city_position][city_to_visit] == -1) { //if the cost is -1, skip the city
                continue;
            }
            int cost = city_matrix.matrix[city_position][city_to_visit] + searchAllSolutions(city_matrix, new_mask, city_to_visit, path_array, cost_array);

            if (cost < min_cost) {
                min_cost = cost;
                path_array[city_position][mask] = city_to_visit; //save the city that was visited
            }
        }
    }

    return cost_array[city_position][mask] = min_cost; //save the cost for the current mask
}

void TSP::dynamicProgrammingTSP(cityMatrix &city_matrix) {
    int mask = 1;
    int city_position = 0;
    int min_path[city_matrix.size];

    // Initialize the cost and path arrays
    int** cost_array = new int*[city_matrix.size]; //n cities
    int** path_array = new int*[city_matrix.size]; //n cities
    for (int i = 0; i < (city_matrix.size); i++) {
        cost_array[i] = new int[1 << city_matrix.size]; //2^n masks
        path_array[i] = new int[1 << city_matrix.size]; //2^n masks
    }

    for (int row = 0; row < city_matrix.size; row++) { //initialize the cost array
        for (int col = 0; col < 1 << city_matrix.size; col++) {
            cost_array[row][col] = -1;
            path_array[row][col] = -1;
        }
    }
    //-------------------------------------------

    int cost = searchAllSolutions(city_matrix, mask, city_position, path_array, cost_array);
    std::cout << "Minimum cost: " << cost << std::endl;

    std::cout << "Path: ";
    printPath(city_matrix, path_array);

    for (int i = 0; i < city_matrix.size ; i++) {
        delete [] path_array[i];
        delete [] cost_array[i];
    }
    delete [] path_array;
    delete [] cost_array;

}


int reduceMatrix(int** matrix, int size) {
    int total_reduction_cost = 0;
    // Reduce rows
    for (int i = 0; i < size; i++) {
        int min_value = INT_MAX;
        for (int j = 0; j < size; j++) {
            if (matrix[i][j] != -1 && matrix[i][j] < min_value) {
                min_value = matrix[i][j];
            }
        }
    // Subtract the minimal value to create a new reduced matrix
        if (min_value != INT_MAX && min_value > 0) {
            total_reduction_cost += min_value;
            for (size_t j = 0; j < size; j++) {
                if (matrix[i][j] != -1) {
                    matrix[i][j] -= min_value;
                }
            }
        }
    }


    // Reduce columns
    for (int j = 0; j < size; j++) {
        int min_value = INT_MAX;
        for (int i = 0; i < size; i++) {
            if (matrix[i][j] != -1 && matrix[i][j] < min_value) {
                min_value = matrix[i][j];
            }
        }
        // Subtract the minimal value to create a new reduced matrix
        if (min_value != INT_MAX && min_value > 0) {
            total_reduction_cost += min_value;
            for (size_t i = 0; i < size; i++) {
                if (matrix[i][j] != -1) {
                    matrix[i][j] -= min_value;
                }
            }
        }
    }
    return total_reduction_cost;
}

int** allocateMatrix(int size) {
    int** matrix = new int*[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
    }
    return matrix;
}

int** copyMatrix(int** source, int size) {
    int** newMatrix = allocateMatrix(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            newMatrix[i][j] = source[i][j];
        }
    }
    return newMatrix;
}

void freeMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

struct Node {
    int** reducedMatrix;
    int cost;
    int level;
    int i, j; // Edge (from, to)
    std::vector<int> path;
};

struct CompareNode {
    bool operator()(const Node& a, const Node& b) {
        return a.cost > b.cost;
    }
};

int primMST(int** matrix, int size, const std::vector<int>& visitedNodes) {
    std::vector<bool> inMST(size, false);
    std::vector<int> key(size, INT_MAX);
    int totalMSTCost = 0;

    // Start from the first unvisited node
    int startNode = -1;
    for (int i = 0; i < size; i++) {
        if (std::find(visitedNodes.begin(), visitedNodes.end(), i) == visitedNodes.end()) {
            startNode = i;
            break;
        }
    }

    if (startNode == -1) return 0; // If no unvisited nodes, return 0.

    key[startNode] = 0;

    for (int count = 0; count < size; count++) {
        int minKey = INT_MAX;
        int u = -1;

        // Find the node with the minimum key value not in MST
        for (int v = 0; v < size; v++) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) break; // No more nodes to process

        inMST[u] = true;
        totalMSTCost += key[u];

        // Update the key values of adjacent nodes
        for (int v = 0; v < size; v++) {
            if (!inMST[v] && matrix[u][v] != -1 && matrix[u][v] < key[v]) {
                key[v] = matrix[u][v];
            }
        }
    }

    return totalMSTCost;
}

Node createNode(int** parent_matrix, int size, int level, int from_edge, int to_edge, const std::vector<int>& path, int path_cost) {
    // Initialization of the edge
    Node node;
    node.reducedMatrix = copyMatrix(parent_matrix, size);
    node.level = level;
    node.i = from_edge;
    node.j = to_edge;
    node.path = path;
    node.path.push_back(to_edge);

    // Setting -1 to the row and column that correspond to the edge
    for (int i = 0; i < size; i++) {
        node.reducedMatrix[from_edge][i] = -1;
        node.reducedMatrix[i][to_edge] = -1;
    }
    node.reducedMatrix[to_edge][0] = -1;

    // Calculating the node cost - the LOWER BOUND
    int reduction_cost = reduceMatrix(node.reducedMatrix, size);
    int mst_cost = primMST(node.reducedMatrix, size, node.path);
    int effective_lower_bound = std::max(parent_matrix[from_edge][to_edge] + path_cost + reduction_cost, mst_cost);
    node.cost = effective_lower_bound;
    return node;
}

// Function to find the nearest unvisited city
int findNearestCity(int currentCity, cityMatrix &city_matrix, const std::vector<bool> &visited) {
    int size = city_matrix.size;
    int nearestCity = -1;
    int minDistance = INT_MAX;

    for (int i = 0; i < size; i++) { // Iterate through all cities and find the nearest one
        if (!visited[i] && city_matrix.matrix[currentCity][i] != -1 && city_matrix.matrix[currentCity][i] < minDistance) {
            minDistance = city_matrix.matrix[currentCity][i];
            nearestCity = i;
        }
    }

    return nearestCity;
}

// Function to find the upper bound using the nearest neighbour approach
int findUpperBoundNearestNeighbour(cityMatrix &city_matrix) {
    int size = city_matrix.size;
    std::vector<bool> visited(size, false);
    int currentCity = 0;  // Start at the first city
    visited[currentCity] = true;
    int totalCost = 0;

    for (int step = 1; step < size; step++) { // Visit all cities
        int nextCity = findNearestCity(currentCity, city_matrix, visited);
        // if (nextCity == -1) {
        //     std::cerr << "Error: Could not find a valid path." << std::endl;
        //     return INT_MAX;
        // }
        totalCost += city_matrix.matrix[currentCity][nextCity];
        visited[nextCity] = true;
        currentCity = nextCity;
    }

    // Return to the starting city to complete the tour
    totalCost += city_matrix.matrix[currentCity][0];

    return totalCost;
}

void TSP::branchAndBound(cityMatrix &city_matrix) {
    std::priority_queue<Node, std::vector<Node>, CompareNode> node_queue;
    std::vector<int> initial_path = {0};
    int size = city_matrix.size;

    // Initialization of the root node
    Node root;
    root.reducedMatrix = copyMatrix(city_matrix.matrix, size);
    root.level = 0;
    root.i = -1;
    root.j = -1;
    root.path = initial_path;
    root.cost = reduceMatrix(root.reducedMatrix, size);

    node_queue.push(root); // Add the root to the queue
    int best_cost = findUpperBoundNearestNeighbour(city_matrix); // find the UPPER BOUND
    std::vector<int> best_path;

    while (!node_queue.empty()) {   // While there are still nodes to discover
        Node current_node = node_queue.top();
        node_queue.pop();   // Pop the least cost node

        if (current_node.cost >= best_cost) {   // If it's cost is already bigger than UPPER BOUND
            freeMatrix(current_node.reducedMatrix, size); // Do not discover that node further
            continue;
        }

        if (current_node.level == size - 1) { // If we had reached the last city
            int final_cost = current_node.cost;
            if (final_cost < best_cost) { // Update UPPER BOUND if the final cost is smaller
                best_cost = final_cost;
                best_path = current_node.path;
            }
            freeMatrix(current_node.reducedMatrix, size);
            continue;
        }

        // For each path from the current node
        for (int j = 0; j < size; j++) {
            if (current_node.reducedMatrix[current_node.path.back()][j] != -1) { // If it's a possible route
                Node child_node = createNode(current_node.reducedMatrix, size, current_node.level + 1,
                          current_node.path.back(), j, current_node.path, current_node.cost);

                if (child_node.cost < best_cost) { // If the child node is worth exploring
                    node_queue.push(child_node); // Put it on the queue
                } else {
                    freeMatrix(child_node.reducedMatrix, size); // Else get rid of it
                }
            }
        }

        freeMatrix(current_node.reducedMatrix, size); // Free the space after every possible route from the current node
    }

    std::cout << "Minimal cost: " << best_cost << "\n";
    std::cout << "Path: ";
    for (int node : best_path) {
        std::cout << node << " -> ";
    }
    std::cout << best_path[0] << "\n";
}

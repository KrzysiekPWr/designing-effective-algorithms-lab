//
// Created by krisk on 15/10/2024.

#include "TSP.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <random>

/**
 * Calculates the cost of the path along the given path
 * @param city_matrix object that contains the matrix of cities
 * @param path array of cities
 * @return cost of the path
 */

#define NEIGHBOUR_COUNT 1500
#define TABU_TENURE 100
#define PENALTY 1
#define PENALTY_DECREMENT_RATE 1
#define PENALTY_DECREMENT_TIME 5
#define K_T 100
#define STARTING_TEMPERATURE_MULTIPLIER 10

int TSP::calculatePathCost(cityMatrix &city_matrix, Tour &tour) {
    int cost = 0;
    for (int i = 0; i < tour.path.size() -2; ++i) {
        cost += city_matrix.matrix[tour.path[i]][tour.path[i + 1]];
    }
    cost += city_matrix.matrix[tour.path[tour.path.size() - 2]][tour.path[0]]; // cost of returning to the starting city
    return cost;
}

// method to find the greedy tour starting from the given city
Tour findGreedyTour(cityMatrix &city_matrix, int start){
    Tour tour;
    int size = city_matrix.size;
    tour.path.push_back(start);
    tour.totalCost = 0;

    std::vector<bool> visited(size+1, false);
    visited[start] = true;

    int current = start;

    for (int step = 1; step < size; step++) {
        int nearest = -1;
        int minCost = INT_MAX;

        for (int i = 0; i < size; i++) {
            if (!visited[i] && city_matrix.matrix[current][i] < minCost) {
                minCost = city_matrix.matrix[current][i];
                nearest = i;
            }
        }

        if (nearest == -1) {
            // No more cities to visit
            break;
        }

        // add the nearest city to the path
        tour.path.push_back(nearest);
        tour.totalCost += minCost;
        visited[nearest] = true;
        current = nearest;
    }

        // add the cost of the return to the starting city
        tour.path.push_back(start);
        tour.totalCost += city_matrix.matrix[current][start];

    return tour;
}

// method to find the best greedy tour
Tour findBestGreedyTour(cityMatrix &city_matrix) {
    Tour bestTour;
    bestTour.totalCost = INT_MAX;

    for (int start = 0; start < city_matrix.size; ++start) {
        Tour currentTour = findGreedyTour(city_matrix, start);
        if (currentTour.totalCost < bestTour.totalCost) {
            bestTour = currentTour;
        }
    }
    return bestTour;
}

Tour generateRandomTour(cityMatrix& city_matrix) {
    Tour tour;
    tour.path = std::vector<int>(city_matrix.size);
    for (int i = 0; i < city_matrix.size; ++i) {
        tour.path[i] = i;
    }
    std::random_shuffle(tour.path.begin(), tour.path.end());
    tour.totalCost = TSP::calculatePathCost(city_matrix, tour);
    return tour;
}

// Tour generateNeighbor(cityMatrix& city_matrix, Tour& currentTour) {
//     Tour neighbor = currentTour;
//     int i = rand() % currentTour.path.size();
//     int j = rand() % currentTour.path.size();
//     std::swap(neighbor.path[i], neighbor.path[j]);
//     neighbor.totalCost = calculatePathCost(city_matrix, neighbor); // calcualte the cost of the new path
//     return neighbor;
// }

struct Move {
    int from, to;
};

struct DescendingComparator {
    bool operator()(const int &a, const int &b) const {
        return a > b; // Descending order
    }
};

Move generateRandomMove(cityMatrix &city_matrix) {
    //generate a random move excluding the starting and ending city
    int i, j;
    do {
    i = rand() % (city_matrix.size - 1) + 1;
    j = rand() % (city_matrix.size - 1) + 1;
    }while(i == j);
    return Move{i, j};
}

Tour makeMove(cityMatrix& city_matrix, Tour tour, const Move &move) {
    std::swap(tour.path[move.from], tour.path[move.to]);
    tour.totalCost = TSP::calculatePathCost(city_matrix, tour);
    return tour;
}

void generateMoves(cityMatrix &city_matrix, std::map<int, Move, DescendingComparator>& available_moves_map, int** tabu_list, Tour& current_tour) {
    //generate the neighbourhood
    for(int i = 1; i < current_tour.path.size()-1; i++) {
        for(int j = i+1; j < current_tour.path.size()-1; j++) {
            Move new_move = {i, j};
            int penalty = tabu_list[new_move.to][new_move.from];
            Tour tour_after_move = makeMove( city_matrix,current_tour, new_move);
            available_moves_map.insert(std::pair<int, Move>(current_tour.totalCost - tour_after_move.totalCost - penalty, new_move));
        }
    }
}

void viewTabuList(cityMatrix city_matrix, int** tabu_list) {
    for (int i = 0; i < city_matrix.size; i++) {
        for (int j = 0; j < city_matrix.size; j++) {
            std::cout << tabu_list[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void decrementTabuTenure(cityMatrix& city_matrix, int** tabu_list) {
    for (int i = 0; i < city_matrix.size; i++) {
        for (int j = i; j < city_matrix.size; j++) {
            if (tabu_list[i][j] > 0) {
                tabu_list[i][j]--;
            }
        }
    }
}

void decrementPenalties(cityMatrix& city_matrix, int** tabu_list) {
    for (int i = 0; i < city_matrix.size; i++) {
        for (int j = 0; j < city_matrix.size; j++) {
            if (tabu_list[i][j] > 0) {
                tabu_list[i][j] -= PENALTY_DECREMENT_RATE;
            }
        }
    }
}
// ==============================================TABU SEARCH ==================================================

Tour TSP::tabuSearch(cityMatrix& city_matrix, std::string output_filename) {
    // Step 1: Use the greedy procedure to find the initial best solution
    Tour current_tour = findBestGreedyTour(city_matrix); // Initialize current solution x
    Tour best_tour = current_tour;                       // Initialize best solution x*
    auto last_logged_time = 0.0; // Last time the cost was logged

    // Tabu list (tracks tabu moves and their tenure)
    int** tabu_list = new int* [city_matrix.size];

    for (int i = 0; i < city_matrix.size; i++) {
        tabu_list[i] = new int [city_matrix.size];
    }
    // Initialize the tabu list
    for (int i = 0; i < city_matrix.size; i++) {
        for (int j = 0; j < city_matrix.size; j++) {
            tabu_list[i][j] = 0;
        }
    }

    // Prepare the output file
    std::ofstream output_file(output_filename);
    output_file << "TS\n" << city_matrix.size-1 << "\n";
    output_file << 0.0 << "; " << best_tour.totalCost << "\n";


    auto start = std::chrono::steady_clock::now();
    auto stop = start + city_matrix.stop_criterion_time;

    // Main loop of the Tabu Search algorithm
    int loop_iterator = 0;
    while (std::chrono::steady_clock::now() < stop) {

        std::map<int, Move, DescendingComparator> available_moves_map; // Map the difference in cost to the neighbor (key = cost difference, value = neighbor)
        // Generate neighbors using the move operation
        for (int i = 0; i < city_matrix.size*3; i++) {
            // Generate a neighbor and add it to the list
            Move move = generateRandomMove(city_matrix);
            int penalty = tabu_list[move.to][move.from];
            Tour current_tour_after_move = makeMove( city_matrix,current_tour, move);
            available_moves_map.insert(std::pair<int, Move>(current_tour.totalCost - current_tour_after_move.totalCost - penalty, move));
        }

        //Generate the neighbourhood
        generateMoves(city_matrix, available_moves_map, tabu_list, current_tour);

        //Select the best move from the list of neighbors
        //Search for the first non-tabu move
        Move best_move = {-1, -1};
        Move current_move = {-1, -1};
        for (auto & it : available_moves_map) {
            current_move = it.second;
            Tour tour_after_move = makeMove(city_matrix, current_tour, current_move);

            if (tabu_list[current_move.from][current_move.to] == 0) {
                current_tour = tour_after_move;
                best_move = current_move;
                break;
            }
            //if the move is tabu, check if it is better than the best move
            else if (tour_after_move.totalCost < best_tour.totalCost) {
                current_tour = tour_after_move;
                best_tour = tour_after_move;
                best_move = current_move;
                break;
            }
        }
        // If all moves are tabu, select the one with the smallest cost difference (first in the map)
        if (best_move.from == -1) {
            best_move = available_moves_map.begin()->second;
            current_tour = makeMove(city_matrix, current_tour, current_move);
        }

        // std::cout << "\n";
        // std::cout << "Checking move: " << best_move.from << " -> " << best_move.to << std::endl;
        // std::cout << "Cost: " << current_tour.totalCost << std::endl;

        //check if the new solution is better than the best solution
        if (current_tour.totalCost < best_tour.totalCost) {
            best_tour = current_tour;
        }
        else {
            tabu_list[best_move.to][best_move.from] += PENALTY;
        }

        // Step 4: Update the tabu list
        decrementTabuTenure(city_matrix, tabu_list);

        if (loop_iterator % PENALTY_DECREMENT_TIME == 0) {
            decrementPenalties(city_matrix, tabu_list);
        }

        auto current_time = std::chrono::steady_clock::now();
        double time_passed = std::chrono::duration<double>(current_time - start).count(); // Elapsed time in seconds

        // Log every 0.5 seconds
        if (time_passed - last_logged_time >= 0.5) {
            output_file << time_passed << "; " << best_tour.totalCost << "\n";
            last_logged_time = time_passed; // Update the last logged time
        }

        //refresh the tabu tenure for the selected best current_move
        tabu_list[best_move.from][best_move.to] = TABU_TENURE;
    }


    // Return the best solution found
    std::cout << "Greedy approach cost: " << findBestGreedyTour(city_matrix).totalCost << std::endl;
    std::cout << "Tabu search: Minimal cost: " << best_tour.totalCost << std::endl;
    std::cout << "Path: ";
    for (int city : best_tour.path) {
        std::cout << city << " -> ";
    }

    //delete the tabu list
    for (int i = 0; i < city_matrix.size; i++) {
        delete [] tabu_list[i];
    }
    delete [] tabu_list;

    output_file.close();
    return best_tour;
}


// ============================================== SIMULATED ANNEALING ==================================================

double calculateProbability(Tour& best_tour, Tour& new_tour, double& T) {
    double exponent = (new_tour.totalCost - best_tour.totalCost)/T;
    return 1/(1+exp(exponent));
}

double calculateDeltaEFromSamples(cityMatrix& city_matrix, int numSamples = 1000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, city_matrix.size - 1);

    double totalDifference = 0.0;
    int sampleCount = 0;

    for (int i = 0; i < numSamples; ++i) {
        int cityA = dist(gen);
        int cityB = dist(gen);

        // Ensure distinct cities are chosen
        if (cityA != cityB) {
            totalDifference += std::abs(city_matrix.matrix[cityA][cityB] - city_matrix.matrix[cityB][cityA]);
            ++sampleCount;
        }
    }

    return (sampleCount > 0) ? (totalDifference / sampleCount) : 0.0;
}

Tour TSP::simulatedAnnealing(cityMatrix& city_matrix, std::string output_filename) {
    double avg_cost_dif = calculateDeltaEFromSamples(city_matrix, 1000);
    city_matrix.starting_temperature =  -avg_cost_dif / log(0.8) * STARTING_TEMPERATURE_MULTIPLIER;
    double T = city_matrix.starting_temperature;
    auto last_logged_time = 0.0;

    int moves_accepted = 0;
    int moves_rejected = 0;
    int moves_that_worsen = 0;
    int moves_that_improve = 0;
    int end_moves_improved = 0;
    //Use the greedy procedure to find the initial best solution
    Tour current_tour = findBestGreedyTour(city_matrix); // Initialize current solution x
    Tour best_tour = current_tour;

    std::ofstream output_file(output_filename);
    output_file << "SA\n" << city_matrix.size-1 << "\n";
    output_file << 0.0 << "; " << best_tour.totalCost << "\n";

    auto start = std::chrono::steady_clock::now();
    auto stop = start + city_matrix.stop_criterion_time;


    while (std::chrono::steady_clock::now() < stop) {
        for (int i = 0; i < city_matrix.size * 10; ++i) {
            //generate a random move
            Move new_move = generateRandomMove(city_matrix);
            Tour tour_after_move = makeMove(city_matrix, current_tour, new_move);

            //Check if the new tour cost is better
            if(tour_after_move.totalCost < current_tour.totalCost) {
                current_tour = tour_after_move;
                moves_accepted++;
                moves_that_improve++;
            }
            else {
                double probability = calculateProbability(current_tour, tour_after_move, T);
                double random_val = (double)rand()/RAND_MAX;

                // Choose this path with a probability
                if (probability >= random_val) {
                    current_tour = tour_after_move;
                    moves_accepted++;
                    moves_that_worsen++;
                }
                else moves_rejected++;
            }
            // std::cout << "\n";
            // std::cout << "Checking move: " << new_move.from << " -> " << new_move.to << std::endl;
            // std::cout << "Cost: " << current_tour.totalCost << std::endl;
        }
        if (current_tour.totalCost < best_tour.totalCost) {
            best_tour = current_tour;
            end_moves_improved++;
        }
        // std::cout<< "==============================================\n";
        // std::cout << "Current temperature: " << T << std::endl;
        // std::cout << "Current cost: " << current_tour.totalCost << std::endl;
        // std::cout<< "==============================================\n";

        auto current_time = std::chrono::steady_clock::now();
        double time_passed = std::chrono::duration<double>(current_time - start).count(); // Elapsed time in seconds

        // Log every 0.5 seconds
        if (time_passed - last_logged_time >= 0.5) {
            output_file << time_passed << "; " << best_tour.totalCost << "\n";
            last_logged_time = time_passed; // Update the last logged time
        }

        T *= city_matrix.cooling_rate;
    }

    // Return the best solution found
    std::cout << "Greedy approach cost: " << findBestGreedyTour(city_matrix).totalCost << std::endl;
    std::cout << "SA search: Minimal cost: " << best_tour.totalCost << std::endl;
    std::cout << "Path: ";
    for (int city : best_tour.path) {
        std::cout << city << " -> ";
    }
    std::cout << "\nEnd temperature: "<< T << std::endl;
    std::cout << "End exp(-1/Tk): "<< exp(-1/T) << std::endl;

    std::cout << "Moves accepted: " << moves_accepted << std::endl;
    std::cout << "Moves rejected: " << moves_rejected << std::endl;
    std::cout << "Moves that worsen: " << moves_that_worsen << std::endl;
    std::cout << "Moves that improve: " << moves_that_improve << std::endl;
    std::cout << "End moves improved: " << end_moves_improved << std::endl;
    std::cout << "Good moves ratio " << (double)moves_that_improve/(double)(moves_that_improve+moves_that_worsen) << std::endl;

    output_file.close();

    return best_tour;

}
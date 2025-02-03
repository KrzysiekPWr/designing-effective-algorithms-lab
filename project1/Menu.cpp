//
// Created by krisk on 12.10.2024.
//
#include <iostream>
#include <chrono>

#include "cityMatrix.h"
#include "TSP.h"
#include "Menu.h"

#include <fstream>
#include <map>

using namespace std;
void Menu::startMenu(cityMatrix &city_matrix) {
    while(true) {
        cout << """\n\nWelcome to the program!\n"
                "Choose one of the following options:\n"
                "1. Create a random graph\n"
                "2. Load a graph from a file\n"
                "0. Exit\n"
                "Your choice: """;
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                askForRandomMatrix(city_matrix);
                break;
            case 2:
                askForLoadMatrix(city_matrix);
                break;
            case 0:
                exit(0);
            default:
                printf("Invalid choice. Try again.");
                exit(-2);
        }
        algorithmMenu(city_matrix);
    }
}

void Menu::algorithmMenu(cityMatrix &city_matrix){
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> time_span{};

    while(true){
        cout << "\n\nChoose one of the following options:\n"
                "1. Brute force\n"
                "2. Dynamic programming\n"
                "3. Branch and bound\n"
                "9. View the graph\n"
                "0. Return to graph generation menu\n"
                "Your choice: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1:
                // start = std::chrono::high_resolution_clock::now();
                // TSP::bruteForce(city_matrix);
                // end = std::chrono::high_resolution_clock::now();
                // time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                // cout << "Time: " << time_span.count() << "s\n";

            // printf("\n\n\n");
                start = std::chrono::high_resolution_clock::now();
                TSP::bruteForceNew(city_matrix);
                end = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                cout << "Time: " << time_span.count() << "s\n";
                break;
            case 2:
                start = std::chrono::high_resolution_clock::now();
                TSP::dynamicProgrammingTSP(city_matrix);
                end = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                cout << "Time: " << time_span.count() << "s\n";
                break;
            case 3:
                start = std::chrono::high_resolution_clock::now();
                TSP::branchAndBound(city_matrix);
                end = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                cout << "Time: " << time_span.count() << "s\n";
                break;
            case 9:
                city_matrix.showMatrix();
                break;
            case 0:
                printf("\n");
                return;
            default:
                printf("Invalid choice. Try again.");
                algorithmMenu(city_matrix);
        }
    }
}

void Menu::askForRandomMatrix(cityMatrix &city_matrix) {
    int num_cities;
    while(true) {
        printf("\nEnter the number of nodes: ");
        cin >> num_cities;

        if (num_cities < 0) {
            printf("Invalid value(s). Try again.");
        } else {
            break;
        }
    }
    city_matrix.createRandomMatrix(num_cities);
}

void Menu::askForLoadMatrix(cityMatrix &city_matrix) {
    string filename;
    printf("Enter the path to the file: ");
    cin >> filename;
    // filename = "C:\\test_small.txt";
    city_matrix.loadMatrix(filename);
}


void calculateMaxN(cityMatrix &city_matrix) {
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> time_span{};
    std::map<std::string, int> max_n_list;
    int max_n = 0;
    // Brute force
    // do {
    //     max_n++;
    //     city_matrix.createRandomMatrix(max_n);
    //     start = std::chrono::high_resolution_clock::now();
    //     TSP::bruteForceNew(city_matrix);
    //     end = std::chrono::high_resolution_clock::now();
    //     time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    //     cout << "Brute force for " << max_n << " nodes finished in " << time_span.count() << "s\n";
    // } while (time_span.count() < 120.0);
    // max_n_list["bruteForce"] = max_n-1;

    // Dynamic programming
    // max_n = 0;
    // do {
    //     max_n++;
    //     city_matrix.createRandomMatrix(max_n);
    //     start = std::chrono::high_resolution_clock::now();
    //     TSP::dynamicProgrammingTSP(city_matrix);
    //     end = std::chrono::high_resolution_clock::now();
    //     time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    //     cout << "Dynamic Programming for " << max_n << " nodes finished in " << time_span.count() << "s\n";
    // } while (time_span.count() < 120.0);
    // max_n_list["dynamicProgramming"] = max_n-1;

    // Branch and bound
    max_n = 0;
    do {
        max_n++;
        city_matrix.createRandomMatrix(max_n);
        start = std::chrono::high_resolution_clock::now();
        TSP::branchAndBound(city_matrix);
        end = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        cout << "Branch & Bound for " << max_n << " nodes finished in " << time_span.count() << "s\n";
    } while (time_span.count() < 120.0);
    max_n_list["branchAndBound"] = max_n-1;

    cout << "\n\nMax N values:\n";
    for(auto const& [key, val] : max_n_list) {
        cout << key << ": " << val << " nodes\n";
    }
}

#define REPETITIONS 100
void research_all_algorithms(cityMatrix &city_matrix) {
    std::ofstream outfile("research_results.csv");
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file for writing.\n";
        return;
    }

    // Write CSV headers
    outfile << "Nodes,Brute Force Avg Time (s),Dynamic Programming Avg Time (s),Branch and Bound Avg Time (s)\n";

    // calculateMaxN(city_matrix);
    cout << "\n\nStarting research...\n";
    int N_array[7] = {5, 10, 12, 14, 18, 20, 22};

    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> time_span{};
    double times_list[3]; //Brute force[0], Dynamic programming[1], Branch and bound[2]

    for(int n : N_array) {
        times_list[0] = 0;
        times_list[1] = 0;
        times_list[2] = 0;
        for (int i = 0; i < REPETITIONS; i++) {
            if (n <= 12) {
                city_matrix.createRandomMatrix(n);
                start = std::chrono::high_resolution_clock::now();
                TSP::bruteForceNew(city_matrix);
                end = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                times_list[0] += time_span.count();
            }

            if(n <= 20) {
                start = std::chrono::high_resolution_clock::now();
                TSP::dynamicProgrammingTSP(city_matrix);
                end = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                times_list[1] += time_span.count();
            }

                start = std::chrono::high_resolution_clock::now();
                TSP::branchAndBound(city_matrix);
                end = std::chrono::high_resolution_clock::now();
                time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                times_list[2] += time_span.count();
        }
        double avg_time_brute_force = times_list[0]/(double)REPETITIONS;
        double avg_time_dynamic_programming = times_list[1]/(double)REPETITIONS;
        double avg_time_branch_and_bound = times_list[2]/(double)REPETITIONS;

        cout << "Results for " << n << " nodes:\n";
        cout << "Brute force: " << avg_time_brute_force << "s\n";
        cout << "Dynamic programming: " << avg_time_dynamic_programming << "s\n";
        cout << "Branch and bound: " << avg_time_branch_and_bound << "s\n";

        outfile << n << "," << avg_time_brute_force << "," << avg_time_dynamic_programming << "," << avg_time_branch_and_bound << "\n";
    }
    // Close the file
    outfile.close();
    cout << "Results have been written to research_results.csv\n";
    exit(0);
}

#define REPETITIONS_SMALLER 50
void Menu::start_research(cityMatrix &city_matrix) {
    // Open a CSV file for output
    // research_all_algorithms(city_matrix);
    int N_array_bruteforce[7] = {5, 6, 7, 8, 9, 10, 11};
    int N_array_dynamic_programming[7] = {5, 10, 12, 15, 18, 20, 22};
    int N_array_branch_and_bound[7] = {5, 10, 15, 20, 25, 28, 30};

    std::ofstream outfile("research_results2.csv");
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file for writing.\n";
        return;
    }

    // Write CSV headers
    outfile << "5, 6, 7, 8, 9, 10, 11\n";

    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> time_span{};
    double times_list[3] = {0}; //Brute force[0], Dynamic programming[1], Branch and bound[2]

    // Do research for brute force
    for(int n : N_array_bruteforce) {
        times_list[0] = 0;
        for (int i = 0; i < REPETITIONS_SMALLER; i++) {
            city_matrix.createRandomMatrix(n);
            start = std::chrono::high_resolution_clock::now();
            TSP::bruteForceNew(city_matrix);
            end = std::chrono::high_resolution_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            times_list[0] += time_span.count();
        }
        double avg_time_brute_force = times_list[0]/(double)REPETITIONS_SMALLER;
        cout << "Results for Brute force:\n";
        cout << "Brute force: " << avg_time_brute_force << "s\n";
        outfile << avg_time_brute_force << ",";
    }

    // Do research for dynamic programming
    outfile << "\n5, 10, 12, 15, 18, 20, 22\n";
    for(int n : N_array_dynamic_programming) {
        times_list[1] = 0;
        for (int i = 0; i < REPETITIONS_SMALLER; i++) {
            city_matrix.createRandomMatrix(n);
            start = std::chrono::high_resolution_clock::now();
            TSP::dynamicProgrammingTSP(city_matrix);
            end = std::chrono::high_resolution_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            times_list[1] += time_span.count();
        }
        double avg_time_dynamic_programming = times_list[1]/(double)REPETITIONS_SMALLER;
        cout << "Results for Dynamic Programming:\n";
        cout << "Dynamic programming: " << avg_time_dynamic_programming << "s\n";
        outfile << avg_time_dynamic_programming << ",";
    }

    // Do research for branch and bound
    outfile << "\n5, 10, 15, 20, 25, 28, 30\n";
    for(int n : N_array_branch_and_bound) {
        times_list[2] = 0;
        for (int i = 0; i < REPETITIONS_SMALLER; i++) {
            city_matrix.createRandomMatrix(n);
            start = std::chrono::high_resolution_clock::now();
            TSP::branchAndBound(city_matrix);
            end = std::chrono::high_resolution_clock::now();
            time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            times_list[2] += time_span.count();
        }
        double avg_time_branch_and_bound = times_list[2]/(double)REPETITIONS_SMALLER;
        cout << "Results for Branch & Bound:\n";
        cout << "Branch & Bound: " << avg_time_branch_and_bound << "s\n";
        outfile << avg_time_branch_and_bound << ",";
    }
    // Close the file
    outfile.close();
    cout << "Results have been written to research_results2.csv\n";
    exit(0);
}
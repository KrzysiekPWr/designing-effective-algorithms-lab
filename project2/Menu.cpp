//
// Created by krisk on 12.10.2024.
//
#include <iostream>
#include <chrono>

#include "cityMatrix.h"
#include "TSP.h"
#include "Menu.h"
#include <filesystem>
#include <fstream>
#include <map>

using namespace std;

void Menu::startMenu(cityMatrix &city_matrix) {
    while(true) {
        cout << """\n\nWelcome to the program!\n"
                "Choose one of the following options:\n"
                "1. Load a graph from a tsp.xml file\n"
                "2. Load solutions from a file\n"
                "3. Load all saved solutions\n"
                "0. Exit\n"
                "Your choice: """;
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                askForLoadMatrix(city_matrix);
                break;
            case 2:
                askForFileName(city_matrix);
                break;
            case 3:
                processFiles("solutions", city_matrix, "results.txt");
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

    while(true){
        cout << "\n\nChoose one of the following options:\n"
                "1. Tabu Search\n"
                "2. Simulated Annealing\n"
                "3. Enter stop criterion\n"
                "4. Enter temperature factor\n"
                "9. View the graph\n"
                "0. Return to graph generation menu\n"
                "Your choice: ";
        int choice;
        std::cin >> choice;
        Tour best_tour_SA;
        Tour best_tour_TS;
        switch (choice) {
            case 1:
                best_tour_TS = TSP::tabuSearch(city_matrix, "TS_test.txt");
                saveTourToFile(best_tour_TS, "TS");
                break;
            case 2:
                best_tour_SA = TSP::simulatedAnnealing(city_matrix, "SA_test.txt");
                saveTourToFile(best_tour_SA, "SA");
                break;
            case 3:
                askForStopCriterion(city_matrix);
                break;
            case 4:
                askForTemperatureFactor(city_matrix);
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

std::string getCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    // Convert to time_t
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Convert time_t to tm structure for local time
    std::tm* local_time = std::localtime(&time);

    // Use ostringstream to format time as a string
    std::ostringstream oss;
    oss << std::put_time(local_time, "%d_%H_%M_%S"); // Customize format as needed

    return oss.str();
}

void Menu::saveTourToFile(Tour tour, string filename) {
    string current_time = getCurrentTimeAsString();

    int number_of_vertices = tour.path.size()-1;
    filename = "solutions/" + filename + std::to_string(number_of_vertices) + "_" + current_time + ".txt";
    std::ofstream outputFile(filename); // Open the file for writing

    if (!outputFile) {
        std::cerr << "Error: Unable to open file " << filename << " for writing.\n";
        return;
    }

    outputFile << number_of_vertices << "\n"; // Write the number of cities in the tour
    for (int city : tour.path) {
        outputFile << city << '\n'; // Write each city index on a new line
    }

    outputFile.close(); // Close the file
    std::cout << "Tour saved to " << filename << " successfully.\n";
}
void loadCorrectMatrix(cityMatrix& city_matrix, int num_cities) {
    if (num_cities == 56 || num_cities == 54) {
        num_cities = 55;
    }
    else if (num_cities == 171 || num_cities == 169) {
        num_cities = 170;
    }
    else if (num_cities == 359 || num_cities == 357) {
        num_cities = 358;
    }

    switch (num_cities) {
        case 55:
            city_matrix.loadMatrix("C:\\ftv55.xml", num_cities, false);
            break;
        case 170:
            city_matrix.loadMatrix("C:\\ftv170.xml", num_cities, true);
            break;
        case 358:
            city_matrix.loadMatrix("C:\\rbg358.xml", num_cities, true);
            break;
    }

}

Tour readSolutionFromFile(cityMatrix& city_matrix, string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    int num_cities = 0;
    Tour read_tour;

    std::string line;
    // Read the first line to get the number of vertices
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> num_cities;
    } else {
        throw std::runtime_error("Error: File is empty or corrupted.");
    }
    loadCorrectMatrix(city_matrix, num_cities);

    // Read the subsequent lines to get the solution path
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int vertex;
        while (iss >> vertex) {
            read_tour.path.push_back(vertex);
        }
    }
    file.close();

    read_tour.totalCost = TSP::calculatePathCost(city_matrix, read_tour);

    std::cout << "Read tour from file " << filename << " successfully.\n";
    std::cout << "Number of vertices: " << num_cities << std::endl;
    std::cout << "Tour cost: " << read_tour.totalCost << std::endl;
    std::cout << "Tour path: ";
    for (int vertex : read_tour.path) {
        std::cout << vertex << " ";
    }
    return read_tour;
}

namespace fs = std::filesystem;
void Menu::processFiles(const std::string& directory, cityMatrix& city_matrix, const std::string& outputFilename) {
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open output file " + outputFilename);
    }

    for (auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().string();
            try {
                Tour read_tour = readSolutionFromFile(city_matrix, filename);
                outputFile << filename << ": " << read_tour.totalCost << "\n";
                std::cout << "Processed " << filename << " with total cost: " << read_tour.totalCost << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error processing file " << filename << ": " << e.what() << "\n";
            }
        }
    }

    outputFile.close();
    std::cout << "Results saved to " << outputFilename << "\n";
}

void Menu::askForFileName(cityMatrix& city_matrix) {
    std::string input_filename;
    std::cout << "Enter the output filename to load the results from: ";
    std::cin >> input_filename;

    readSolutionFromFile(city_matrix, input_filename);
}

void Menu::askForStopCriterion(cityMatrix &city_matrix) {
    int stop_criterion;
    while(true) {
        printf("\nEnter the stop criterion in seconds (default is 60s):\n");
        cin >> stop_criterion;

        if (stop_criterion < 0) {
            printf("Invalid value(s). Try again.");
        } else {
            city_matrix.stop_criterion_time = stop_criterion * 1s;
            break;
        }
    }
}

void Menu::askForTemperatureFactor(cityMatrix &city_matrix) {
    double temperature_factor;
    while(true) {
        printf("\nEnter the temperature factor (default is 0.999):\n");
        cin >> temperature_factor;

        if (temperature_factor < 0 || temperature_factor > 1) {
            printf("Invalid value(s). Try again.");
        } else {
            city_matrix.cooling_rate = temperature_factor;
            break;
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
    string answer = "-1";
    string filename;
    int num_cities;
    while (true) {
        cout << "\n\n"
             << "Choose one of the following files:\n"
             << "1. ftv55.xml\n"
             << "2. ftv170.xml\n"
             << "3. rbg358.xml\n"
             << "Your choice: ";
        cin >> answer;

        try {
            int int_answer = std::stoi(answer);

            switch (int_answer) {
                case 1:
                    filename = "C:\\ftv55.xml";
                    num_cities = 55;
                break;
                case 2:
                    filename = "C:\\ftv170.xml";
                    num_cities = 170;
                break;
                case 3:
                    filename = "C:\\rbg358.xml";
                    num_cities = 358;
                break;
                default:
                    cout << "Invalid choice. Try again." << endl;
                continue;
            }
            break;

        } catch (std::invalid_argument&) {
            cout << "Invalid input. Please enter a number." << endl;
        } catch (std::out_of_range&) {
            cout << "Number is out of range. Try again." << endl;
        }
    }
    city_matrix.loadMatrix(filename, num_cities, false);
}

void Menu::askForLoadMatrixTXT(cityMatrix &city_matrix) {
    string filename;
    printf("Enter the path to the file: ");
    cin >> filename;
    // filename = "C:\\test_small.txt";
    city_matrix.loadMatrixFromTXT(filename);
}

void Menu::start_research(cityMatrix &city_matrix) {
    std::map<int, std::string> files = {
            {55, "C:\\ftv55.xml"},
            {170, "C:\\ftv170.xml"},
            {358, "C:\\rbg358.xml"}
    };

    for (auto const& [num_cities, filename] : files) {
        city_matrix.loadMatrix(filename, num_cities, false);
        if (num_cities == 55) {
            city_matrix.stop_criterion_time = 120s;
        }
        else if (num_cities == 170) {
            city_matrix.stop_criterion_time = 240s;
        }
        else {
            city_matrix.stop_criterion_time = 360s;
        }


        for (int i = 0; i < 5; i++) {
            std::string output_filename = "TS" + std::to_string(num_cities) + "_results_" + std::to_string(i) + ".txt";
            Tour best_tour_TS = TSP::tabuSearch(city_matrix, output_filename);
            saveTourToFile(best_tour_TS, "TS");
        }

        std::vector <double> cooling_rates = {0.9999, 0.9995, 0.999};
        for (double cooling_rate : cooling_rates) {
            city_matrix.cooling_rate = cooling_rate;
            for (int i = 0; i < 3; i++) {
                std::string output_filename = "SA" + std::to_string(num_cities) + "_results_" + to_string(cooling_rate) + std::to_string(i) + ".txt";
                Tour best_tour_SA = TSP::simulatedAnnealing(city_matrix, output_filename);
                saveTourToFile(best_tour_SA, "SA");
            }
        }
    }
}
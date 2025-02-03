import glob
import matplotlib.pyplot as plt
import matplotlib.colors as mplcolors
import random

def read_data_from_file(filename, from_id, to_id):
    time = []
    values = []
    average_error = []

    with open(filename, 'r') as file:
        lines = file.readlines()

        # Skip the first two lines and last two lines
        for line in lines[from_id : to_id]:
            print(line)
            t, v, e = line.strip().split(';')
            # print(t,v,e)
            time.append(int(t))
            values.append(int(v))
            average_error.append(float(e))

    return time, values, average_error

def read_data(filename, from_id, to_id):
    values = []
    with open(filename, 'r') as file:
        lines = file.readlines()

        for line in lines[from_id : to_id]:
            v = line.strip().split('; ')
            for element in v:
                values.append(float(element))
    return values
        
def plot_values(time, values):
    plt.figure(figsize=(10, 6))
    plt.plot(time, values, marker='o')
    plt.xlabel('Czas (ms)')
    plt.ylabel('Koszt drogi')
    plt.title('Wykres najlepszego kosztu drogi w danej populacji w funkcji czasu')
    plt.grid(True)
    plt.show()

def plot_error(time, average_error):
    plt.figure(figsize=(10, 6))
    plt.plot(time, average_error, marker='o')
    plt.xlabel('Czas (ms)')
    plt.ylabel('Średni błąd względny populacji')
    plt.title('Wykres średniego błędu względnego populacji w funkcji czasu')
    plt.grid(True)
    plt.show()

def plot_all_values(best_results_filenames_map, log_scale=False):
    colors = mplcolors.CSS4_COLORS
    my_colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan', 'teal', 'magenta']
    for key, filename in best_results_filenames_map.items():
        time, values, average_error = read_data_from_file(filename, 3, -3)
        if log_scale:
            plt.yscale('log')
        plt.plot(time, average_error, marker='o', label=key, color=colors.get(my_colors.pop(random.randint(0, len(my_colors) - 1))))

    plt.xlabel('Czas (ms)')
    plt.ylabel('Średni błąd względny populacji')
    plt.title('Wykres najlepszego kosztu drogi w danej populacji w funkcji czasu')
    plt.legend()
    plt.grid(True)
    plt.show()

def save_results1(results_map):
    results_file = open("RESULTS1.txt", "w")

    for cross_type in ["OX", "PMX"]:
        for mutation_type in ["SWAP", "INV"]:
            for i in range(1, 11):
                results_file.write(cross_type + " " + mutation_type + " ; ") 
                for pop_size in [500, 1000, 2000]:
                    key = cross_type + "_" + mutation_type + "_" + str(pop_size) + "_" + str(i)
                    results_values = results_map.get(key)
                    results_file.write(str(int(results_values[0])) + ";" + str(results_values[1]) + " ; ")
                results_file.write("\n")
            results_file.write("\n")
    
    results_file.close()

def save_results2(results_map):
    results_file = open("RESULTS2.txt", "w")

    for cross_type in ["OX", "PMX"]:
        for mutation_type in ["SWAP", "INV"]:
            for i in range(1, 11):
                results_file.write(cross_type + " " + mutation_type + " ; ") 
                for mut_rate in ["0.010000", "0.050000", "0.100000"]:
                    key = cross_type + "_" + mutation_type + "_" + mut_rate + "_" + str(i)
                    results_values = results_map.get(key)
                    results_file.write(str(int(results_values[0])) + ";" + str(results_values[1]) + " ; ")
                results_file.write("\n")
            results_file.write("\n")

    results_file.close()

def save_results3(results_map):
    results_file = open("RESULTS3.txt", "w")
    for cross_type in ["OX", "PMX"]:
        for mutation_type in ["SWAP", "INV"]:
            for i in range(1, 11):
                results_file.write(cross_type + " " + mutation_type + " ; ") 
                for cross_rate in ["0.500000", "0.700000", "0.900000"]:
                    key = cross_type + "_" + mutation_type + "_" + cross_rate + "_" + str(i)
                    results_values = results_map.get(key)
                    # print(key, results_values)
                    results_file.write(str(int(results_values[0])) + ";" + str(results_values[1]) + " ; ")
                results_file.write("\n")
            results_file.write("\n")

    results_file.close()

def plot_best_ga_tabu():
    filename1 = "cmake-build-debug/second-research-files/GA_OX_SWAP_0.010000_7.txt"
    filename2 = "cmake-build-debug/tabu_results.txt"

    for filename in [filename1, filename2]:
        time, values, average_error = read_data_from_file(filename, 3, -3)
        plt.plot(time, average_error, marker='o')
        plt.title('Wykres najlepszej instancji rozwiązania algorytmu dla algorytmu genetycznego i tabu search')
        plt.xlabel('Czas (ms)')
        plt.ylabel('Błąd względny')
        plt.legend(['Algorytm genetyczny', 'Tabu search'])
        plt.grid(True)
        plt.yscale('log')
    plt.show()


if __name__ == "__main__":
    best_map = {}
    results_map = {}
    best_results_filenames_map = {}

    path1 = "cmake-build-debug/first-research-files"
    path2 = "cmake-build-debug/second-research-files"
    path3 = "cmake-build-debug/third-research-files"
    files = glob.glob(path3 + '/*.txt')
    # print(files)
    for filename in files:
        file_type = filename.strip().split("_")
        file_key = file_type[1] + "_" + file_type[2] + "_" + file_type[3]
        file_key_results = file_key + "_" + file_type[4].split(".")[0]

        best_values = best_map.get(file_key)
        current_values = read_data(filename, -2, -1)
        results_map.update({file_key_results: current_values})
        # Search for the best value in each of the filetype
        if best_values != None and current_values[0] < best_values[0]:
            best_map.update({file_key: current_values})
            best_results_filenames_map.update({file_key: filename})
        elif best_values == None:
            best_map.update({file_key: current_values})
            best_results_filenames_map.update({file_key: filename})

    
    # plot_all_values(best_results_filenames_map, log_scale=False)
    plot_best_ga_tabu()
    # plot_all_values(best_results_filenames_map, log_scale=True)
    # save_results1(results_map)
    # save_results2(results_map)
    # save_results3(results_map)


    

    # for key in results_map:
    #     print(key, results_map.get(key))

    # for key in best_map:
    #     print(key, best_map.get(key))
    
    # print("\n")

    # for key in best_results_filenames_map:
    #     print(key, best_results_filenames_map.get(key))

    

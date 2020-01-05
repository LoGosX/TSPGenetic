#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "optimal_path.h"

#include "GeneticAlgorithm.h"
#include "TSPOperators.h"
#include "2opt.h"


int lastIndexOf(std::string p, char c) {
    int i = -1;
    for(int j = p.size() - 1; j >= 0; j--)
        if(p[j] == c){
            return j;
        }
    return -1;
}

void writeResults(std::string path, const TSPChrom& best, const TSPEvaluator& eval, GASettings settings) {
    path += ".results";
    path = "../datasets/" + path;
    std::ofstream file(path, std::ios::app);
    if(file.good()) {
        std::cout << "Saving results to: " << path << "\n";
        file << "\nGenerations: " << settings.n_epochs
            << "\nPopulation: " << settings.pop_size
            << "\nMutation probability: " << settings.mutation_probability
            << "\nCrossover probability: " << settings.crossover_probability
            << "\nElitism percent: " << settings.elitism_percent
            << "\nLocal search rate: " << settings.local_search_rate << std::endl;

        file << "Distance: " << eval.pathDist(best) << std::endl;
        file << "\nPath: 1 ";
        for(int i : best.path)
            file << i + 1 << ' ';
        file << '\n';
        file.close();
    }else
        std::cout << "Could not open " << path << " to save results." << std::endl;
}

std::vector<std::pair<int,int>> load_data(std::string path) {
    std::fstream file(path);
    if(file.good()) {
        int n;
        file >> n;
        std::vector<std::pair<int, int>> cities(n);
        for (int i = 0, a, b, c; i < n; i++)
        {
            file >> a >> b >> c;
            cities[i].first = b;
            cities[i].second = c;
        }
        return cities;
    }else
    {
        std::cerr << "Could not open " << path << " and load data\n";
        exit(1);
    }    
}

std::string getFileName(std::string path)
{
    while(path.front() == '.' || path.front() == '/')
        path = path.substr(1, std::string::npos);
    int i = path.find('.');
    path = path.substr(0, i);
    auto j = path.find('/');
    while(j != std::string::npos)
    {
        path = path.substr(j+1, std::string::npos);
        j = path.find('/');
    }
    return path;
}

void tsp(GASettings settings)
{    
    auto cities = load_data(settings.instance_file_path);
    TSPChromCreator creator(cities.size());
    TSPEvaluator evaluator(cities);

    GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator, TSPLocalSearch>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator, TSPLocalSearch(cities));
    ga.setSettings(settings);
    ga.initPopulation();
    std::string prefix = getFileName(settings.instance_file_path);
    ga.setFilePrefix(prefix);
    ga.run();
    auto best = ga.getBestChromosomeEver();
    std::cout << evaluator.pathDist(best) << std::endl;
    /*
    std::cout << "1 ";
    for(int i : best.path)
        std::cout << i + 1 << ' ';
    std::cout << std::endl;
    */
    writeResults(prefix, best, evaluator, settings);
}

int main(int argc, const char * argv[])
{
    //bool result = validate2opt();
    //if(!result){
    //    std::cout << "2opt does not work!\n";
    //    return 0;
    //}else
    //{
    //    std::cout << "2opt works!\n";
    //}
    int pop_size = 100;
    int epochs = 500;
    float cross_chnc = 0.01;
    float mut_chnc = 0.01;
    float elitism_percent = 0.1;
    int local_search_rate = 100;
    bool save = false;
    std::string file = "default";
    bool help = false;
    for(int i = 1; i < argc; i++)
    {
        if(std::string(argv[i]) == "-h")
        {
            help = true;
            break;
        }
    }
    if(help || argc != 9)
    {
        std::cerr << "\nHow to run:\n"
                << "./OKGen <path_to_dataset> <number_of_generations> <population_size> <crossover_chance (0,1)> <mutation_chance (0,1)> <elitism_percent (0,1)> <local_search_rate> <save results? 0 - no, 1 - yes>\n";
            return 0;
    }

    if(argc >= 7){
        file = std::string(argv[1]);
        epochs = std::atoi(argv[2]);
        pop_size = std::atoi(argv[3]);
        cross_chnc = std::atof(argv[4]);
        mut_chnc = std::atof(argv[5]);
        elitism_percent = std::atof(argv[6]);
        local_search_rate = std::atoi(argv[7]);
        save = std::atoi(argv[8]);
    }
    GASettings settings;
    settings.n_epochs = epochs;
    settings.pop_size = pop_size;
    settings.crossover_probability = cross_chnc;
    settings.mutation_probability = mut_chnc;
    settings.elitism_percent = elitism_percent;
    settings.local_search_rate = local_search_rate;
    settings.save_results = save;
    settings.instance_file_path = file;
    tsp(settings);
    return 0;
}
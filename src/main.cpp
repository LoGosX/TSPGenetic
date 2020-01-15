#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "optimal_path.h"

#include "GeneticAlgorithm.h"
#include "TSPOperators.h"
#include "2opt.h"
#include "greedy.h"


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

        file << "Distance: " << eval.pathDist(best) << "\nPath: 1 ";
        for(int i : best.path)
            file << i + 1 << ' ';
        file << '\n';
        file.close();
    }else
        std::cout << "Could not open " << path << " to save results." << std::endl;
}

std::vector<std::pair<int,int>> load_data(std::string path) {
    return {
        {37, 18},
        {96, 30},
        {73, 53},
        {60, 43},
        {15, 29},
        {15, 61},
        {5 ,14 },       
        {87, 29},
        {60, 37},
        {71, 46},
        {2 ,79 },        
        {97, 20},
        {84, 51},
        {21, 59},
        {18, 4},
    };
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
    std::cout << evaluator.pathDist(ga.getBestChromosomeEver()) << '\n';
    std::cout << "Greedy: " << greedy(cities) << '\n';
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
    int epochs = 1000;
    float cross_chnc = 0.01;
    float mut_chnc = 0.01;
    float elitism_percent = 0.01;
    int local_search_rate = -5;
    bool save = false;
    GASettings settings;
    settings.n_epochs = epochs;
    settings.pop_size = 15;
    settings.crossover_probability = cross_chnc;
    settings.mutation_probability = mut_chnc;
    settings.elitism_percent = elitism_percent;
    settings.local_search_rate = local_search_rate;
    settings.save_results = save;
    tsp(settings);
    return 0;
}
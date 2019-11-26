#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include "optimal_path.h"
#include "preProcess.h"

#include "GeneticAlgorithm.h"
#include "ExampleOperators.h"
#include "TSPOperators.h"
#include "2opt.h"

struct GridSearcher
{
    float curr;
    float dx;
    int steps;
    int step{0};
    GridSearcher(float low, float high, int steps) : steps(steps), curr(low), dx((high - low) / steps) {}
    float next()
    {
        float v = curr;
        curr += dx;
        step++;
        return v;
    }
};

void test_tsp()
{
    auto data = randomPath(11);
    float best_path = minimumPathLength(data);

    TSPChromCreator creator(data.size());
    TSPEvaluator evaluator(data);
    
    GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator, TSPLocalSearch>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator, TSPLocalSearch(data));
    ga.setCrossoverProb(0.1255);
    ga.setMutationProb(0.001);
    ga.initPopulation(1000);
    ga.run(500);
    auto best = ga.getBestChromosomeEver();
    std::cout << "Found by GA: " << evaluator.pathDist(best) << std::endl;
    std::cout << "Optimal: " << best_path << std::endl;
    std::cout << "0 ";
    for(int x : ga.getBestChromosomeEver().path)
        std::cout << x << ' ';
}

void tsp_grid_search()
{
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int>> cities(n);
    for (int i = 0, a, b, c; i < n; i++)
    {
        std::cin >> a >> b >> c;
        cities[i].first = b;
        cities[i].second = c;
    }

    float mut_low = 0.0001;
    float mut_high = 0.25;
    int mut_res = 10;
    GridSearcher mut(mut_low, mut_high, mut_res);

    float cross_low = 0.0001;
    float cross_high = 0.5;
    int cross_res = 10;

    float pop_low = 100;
    float pop_high = 5000;
    int pop_res = 10;


    float best_mu, best_cross, best_eval = 1000000;
    int best_pop;
    std::vector<int> best_path;
    for (; mut.step < mut.steps && false;)
    {
        float mut_chnc = mut.next();
        GridSearcher cros(cross_low, cross_high, cross_res);
        for (; cros.step < cros.steps;)
        {
            float cross_chnc = cros.next();
            
            GridSearcher pop(pop_low, pop_high, pop_res);
            for (; pop.step < pop.steps;)
            {
                int pop_size = (pop.next() + .5F);

                TSPChromCreator creator(n);
                TSPEvaluator evaluator(cities);
                GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator, TSPLocalSearch>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator, TSPLocalSearch(cities));
                ga.setCrossoverProb(cross_chnc);
                ga.setMutationProb(mut_chnc);
                ga.initPopulation(pop_size);
                ga.run(500);
                auto b = ga.getBestChromosomeEver();
                float val = evaluator.pathDist(b);
                std::cout << "Mut: " << mut_chnc << " Cross: " << cross_chnc << " Pop: " << pop_size << " Path length: " << val << std::endl;
                if (val < best_eval)
                {
                    best_eval = val;
                    best_pop = pop_size;
                    best_mu = mut_chnc;
                    best_cross = cross_chnc;
                    best_path = b.path;
                }
            }
        }
    }
    std::cout << "Best path: " << best_eval << "\nMut: " << best_mu << "\nCross: " << best_cross << "\nPop: " << best_pop << std::endl;
}

void writeResults(std::string path, const TSPChrom& best, const TSPEvaluator& eval) {
    path = path.substr(0, path.find('.')) + ".results";
    std::ofstream file(path, ios::app);
    if(file.good()) {
        file << eval.pathDist(best) << "\n1 ";
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

void tsp(std::string path, int epochs, int pop_size, float cross_chnc, float mut_chnc, float elitism_percent, bool save)
{    
    auto cities = load_data(path);
    TSPChromCreator creator(cities.size());
    TSPEvaluator evaluator(cities);

    GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator, TSPLocalSearch>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator, TSPLocalSearch(cities));
    ga.setCrossoverProb(cross_chnc);
    ga.setMutationProb(mut_chnc);
    ga.setElitismPercent(elitism_percent);
    ga.initPopulation(pop_size);
    ga.setFilePrefix(path.substr(path.find('/')+1, path.find('.')));
    ga.run(epochs, save);
    auto best = ga.getBestChromosomeEver();
    std::cout << evaluator.pathDist(best) << std::endl << "1 ";
    for(int i : best.path)
        std::cout << i + 1 << ' ';
    std::cout << std::endl;
    
    writeResults(path, best, evaluator);
}

int main(int argc, const char * argv[])
{
    srand(time(0));
    int pop_size = 100;
    int epochs = 500;
    float cross_chnc = 0.01;
    float mut_chnc = 0.01;
    float elitism_percent = 0.1;
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
    if(help || argc < 7)
    {
        std::cerr << "\nHow to run:\n"
                << "./OKGen <path_to_dataset> <number_of_generations> <population_size> <crossover_chance (0,1)> <mutation_chance (0,1)> <elitism_percent (0,1)> <save results? 0 - no, 1 - yes>\n";
            return 0;
    }

    if(argc >= 7){
        file = std::string(argv[1]);
        epochs = std::atoi(argv[2]);
        pop_size = std::atoi(argv[3]);
        cross_chnc = std::atof(argv[4]);
        mut_chnc = std::atof(argv[5]);
        elitism_percent = std::atof(argv[6]);
        save = std::atoi(argv[7]);
    }
    tsp(file, epochs, pop_size, cross_chnc, mut_chnc, elitism_percent, save);
    return 0;
}
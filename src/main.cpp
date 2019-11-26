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
    auto data = randomPath(12);
    float best_path = minimumPathLength(data);

    TSPChromCreator creator(data.size());
    TSPEvaluator evaluator(data);
    
    GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator);
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


void tsp()
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


    TSPChromCreator creator(cities.size());
    TSPEvaluator evaluator(cities);

    auto greedy = pathGreedy(cities);
    std::vector<int> initial_chrom_path;
    for(int i = 1; i < greedy.size(); i++)
        initial_chrom_path.push_back(greedy[i] - 1);
    
    std::cout << greedy.front() << " " << greedy.back() << " " << greedy.size() << endl;

    std::vector<TSPChrom> initial_chroms(500);
    for(int i = 0; i < initial_chroms.size() / 10; i++)
        initial_chroms[i].path = initial_chrom_path;
    for(int i = initial_chroms.size() / 10; i < initial_chroms.size(); i++)
        creator(initial_chroms[i]);


    GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator);
    ga.setCrossoverProb(0.1255);
    ga.setMutationProb(0.005);
    ga.initPopulation(initial_chroms);
    ga.run(10000);
    auto best = ga.getBestChromosomeEver();
    std::cout << evaluator.pathDist(best) << std::endl;
}

void example()
{
    //PÓKI CO NIE DZIAŁA
    /*
    //przykład z rozdziału 1.1
    //BinaryChrom<10> liczba jest reprezentowana na 22 bitach (jak w przykładzie w książce)
    GeneticAlgorithm<BinaryChrom<22>, BinaryChromosomeCreator, BinaryCrosser, BinaryMutator, BinaryEvaluator> ga;
    ga.initPopulation(50);
    ga.setCrossoverProb(0.25F);
    ga.setMutationProb(0.1); //to nie jest dokładnie to samo co w książce ale co tam
    ga.run(150);
    BinaryEvaluator ev;
    BinaryChrom chr = ga.getBestChromosomeEver();
    std::cout << "Maksymalna wartość funkcji f(x) = x * sin(10*pi*x) + 1.0 po 150 iteracjach wynosi:\n"
        "f(" << ev.getX(chr) << ") = " << ev(chr) << '\n';
    */
}

int main(int argc, const char *argv)
{
    srand(time(0));
    /*
    int pop_size = std::stoi(argv[1]);
    float cross_chnc = std::stoi(argv[2]);
    float mut_chnc = std::stoi(argv[3]);
    int epochs = std::stoi(argv[4]);
    */
    //tsp();
    //test_tsp();
    /* testing 2opt:
    std::vector<int> path = {0, 1, 2, 3};
    std::vector<std::pair<int,int>> cities = {{0, 0}, {5, 1}, {0, 1}, {5, 0}};

    std::vector<int> res = path2opt(path, cities);
    cout << res[0] << res[1] << res[2] << res[3];
    */
    return 0;
}
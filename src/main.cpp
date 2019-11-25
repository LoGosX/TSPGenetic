#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include "optimal_path.h"

#include "GeneticAlgorithm.h"
#include "ExampleOperators.h"
#include "TSPOperators.h"

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
    std::cout << "Found by GA: " << evaluator.pathDist(ga.getBestChromosomeEver()) << std::endl;
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

    float mut_low = 0.0001;
    float mut_high = 0.1;
    int mut_res = 10;
    GridSearcher mut(mut_low, mut_high, mut_res);

    float cross_low = 0.001;
    float cross_high = 0.25;
    int cross_res = 10;

    float pop_low = 100;
    float pop_high = 1000;
    int pop_res = 10;

    TSPChromCreator creator(n);
    TSPEvaluator evaluator(cities);

    GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator);
    ga.setCrossoverProb(0.1255);
    ga.setMutationProb(0.00006);
    ga.initPopulation(730);
    ga.run(10000);
    std::cout << evaluator.pathDist(ga.getBestChromosomeEver()) << std::endl;

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

                GeneticAlgorithm<TSPChrom, TSPChromCreator, TSPCrosser, TSPMutator, TSPEvaluator>
                    ga(creator, TSPCrosser(), TSPMutator(), evaluator);
                ga.setCrossoverProb(cross_chnc);
                ga.setMutationProb(mut_chnc);
                ga.initPopulation(pop_size);
                ga.run(1000);
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
    //test_tsp();
    tsp();
    return 0;
}
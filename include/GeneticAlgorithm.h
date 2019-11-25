#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
#include <string>

//ZBIGNIEW MICHALEWICZ ALG GEN _ STR DAN = PROG EWOL ~STRONA 241+

//później rozdzielę na .h i .cpp
template<class Chromosome, class ChromosomeCreator, class Crosser, class Mutator, class Evaluator>
class GeneticAlgorithm
{
    /*
    Chromosome - represents one solution from a population
    Crosser - defines cross operation between two chromosomes
    Mutator - defines mutation operation on a chromosome
    Evaluator - evaluates the solutions (larger == better)
    ChromosomeCreator - creates and initializes chromosomes
    */
    Crosser crosser;
    Mutator mutator;
    Evaluator evaluator;
    ChromosomeCreator creator;

    std::vector<Chromosome> population;
    std::vector<float> evaluations;
    std::vector<std::pair<float, int>> cum_dist; //used in parent selection (roulette method)
    Chromosome best_chrom_ever; //best chromosome from all generations
    float best_evaluation { -10000000000.F }; // -INF
    int pop_size;
    float cross_prob;
    float mutation_prob;
    float curr_avg_fitness;
    float total_population_fitness;
    float population_variance;
    float population_std_devation;
    float pop_highest_eval;
    float pop_lowest_eval;

    std::ofstream stats_file;
    bool file_good {false};

    void measureStatictics(bool measure_fitness=false)
    {
        if(!measure_fitness)
            measurePopulationFitness();
        population_variance = 0;
        for(int i = 0; i < pop_size; i++)
            population_variance += (evaluations[i] - curr_avg_fitness) * (evaluations[i] - curr_avg_fitness);
        population_variance /= pop_size;
        population_std_devation = sqrtf(population_variance);
    }

    void measurePopulationFitness()
    {
        total_population_fitness = 0;
        pop_lowest_eval = 10000000000;
        pop_highest_eval = -1;
        for(int i = 0; i < pop_size; i++)
        {
            evaluations[i] = evaluator(population[i]);
            total_population_fitness += evaluations[i];
            if(evaluations[i] > pop_highest_eval)
                pop_highest_eval = evaluations[i];
            if(evaluations[i] < pop_lowest_eval)
                pop_lowest_eval = evaluations[i];
            if(evaluations[i] > best_evaluation)
            {
                best_chrom_ever = population[i];
                best_evaluation = evaluations[i];
            }
        }
        curr_avg_fitness = total_population_fitness / pop_size;
    }

    void fillCumDist()
    {
        measurePopulationFitness();
        for(int i = 0; i < pop_size; i++)
            cum_dist[i] = {evaluations[i] / total_population_fitness, i};
        std::sort(begin(cum_dist), end(cum_dist));
        for(int i = 1; i < pop_size; i++)
            cum_dist[i].first += cum_dist[i-1].first;
    }

    int getChromosomeIndexFromDist(float rand_val)
    {
        auto it = std::upper_bound(begin(cum_dist), end(cum_dist), std::pair<float,int>(randomFloat(), -1));
        if(it == end(cum_dist))
            return cum_dist.back().second;
        return it->second;
    }

    void chooseNextPopulation(bool roulette = true)
    {
        if(roulette){
            fillCumDist();
            std::vector<Chromosome> next_pop(pop_size);
            for(auto& chrom : next_pop)
                chrom = population[getChromosomeIndexFromDist(randomFloat())];
            population = next_pop;
        }else //elitism
        {
            float best_percent = 0.1f;
            measurePopulationFitness();
            std::vector<std::pair<float,int>> x;
            for(int i = 0; i < pop_size; i++)
                x.emplace_back(evaluations[i], i);
            std::sort(begin(x), end(x));
        }
    }

    void crossover()
    {
        for(int i = 0; i < pop_size; i++)
        {
            if(randomFloat() < cross_prob)
            {
                int chrom_a = randomInt(0, pop_size);
                int chrom_b = randomInt(0, pop_size);
                crosser(population[chrom_a], population[chrom_b]);
            }
        }
    }

    void mutate()
    {
        for(auto& chrom : population)
            if(randomFloat() < mutation_prob)
                mutator(chrom);
    }

    void dumpStats()
    {
        stats_file << "AvgFitness=" << curr_avg_fitness << " StdDev=" << population_std_devation << " GenBestFitness=" << pop_highest_eval << " GenLowestFitness=" << pop_lowest_eval << " BestFitness=" << best_evaluation << '\n';
    }

public:

    void setOutFile(std::string path) {
        file_good = true;
        stats_file.open(std::string("stats_pop=") + std::to_string(pop_size) + std::string("_mut=") + std::to_string(mutation_prob) + std::string("_cross=") + std::to_string(cross_prob) + std::string(".txt"), std::ios::trunc);
    }

    GeneticAlgorithm() = default;
    
    void setMutationProb(float prob) { mutation_prob = prob; }
    void setCrossoverProb(float prob) { cross_prob = prob; }

    GeneticAlgorithm(ChromosomeCreator creator, Crosser cross, Mutator mut, Evaluator ev) :
        creator(creator), crosser(cross), mutator(mut), evaluator(ev) {}


    void initPopulation(int size) {
        pop_size = size;
        population = std::vector<Chromosome>(pop_size, creator());
        for(auto& chrom : population)
            creator(chrom); //random initialize chromosome
        evaluations.resize(pop_size);
        cum_dist.resize(pop_size);
    }
    void initPopulation(const std::vector<Chromosome>& pop) {
        pop_size = pop.size();
        population = pop;
        evaluations.resize(pop_size);
        cum_dist.resize(pop_size);
    }
    std::vector<Chromosome> getPopulation() const { return population; }

    Chromosome getBestChromosomeEver() const { return best_chrom_ever; }

    void run(int epochs, float epsi = 0.000003f)
    {
        if(file_good)
        {
            measureStatictics();
            dumpStats();
        }
        for(int i = 0; i < epochs /* && poprawa wieksza niz epsilon (np przez ostatnie 5 iteracji) */; i++)
        {
            
            chooseNextPopulation();
            crossover();
            mutate();
            if(file_good)
            {
                measureStatictics(false);
                dumpStats();
            }
        }
        if(file_good)
            stats_file.close();
    }
};
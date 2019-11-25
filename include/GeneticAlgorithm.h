#pragma once
#include <vector>
#include <algorithm>
#include <utility>

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

    void fillCumDist()
    {
        float total = 0;
        for(int i = 0; i < pop_size; i++)
        {
            evaluations[i] = evaluator(population[i]);
            total += evaluations[i];
            if(evaluations[i] > best_evaluation)
            {
                best_chrom_ever = population[i];
                best_evaluation = evaluations[i];
            }
        }
        for(int i = 0; i < pop_size; i++)
            cum_dist[i] = {evaluations[i] / total, i};
        std::sort(begin(cum_dist), end(cum_dist));
        for(int i = 1; i < pop_size; i++)
            cum_dist[i].first += cum_dist[i-1].first;
        curr_avg_fitness = total / pop_size;
    }

    int getChromosomeIndexFromDist(float rand_val)
    {
        auto it = std::upper_bound(begin(cum_dist), end(cum_dist), std::pair<float,int>(randomFloat(), -1));
        if(it == end(cum_dist))
            return cum_dist.back().second;
        return it->second;
    }

    void chooseNextPopulation()
    {
        fillCumDist();
        std::vector<Chromosome> next_pop(pop_size);
        for(auto& chrom : next_pop)
            chrom = population[getChromosomeIndexFromDist(randomFloat())];
        population = next_pop;
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

public:

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
        for(int i = 0; i < epochs /* && poprawa wieksza niz epsilon (np przez ostatnie 5 iteracji) */; i++)
        {
            chooseNextPopulation();
            crossover();
            mutate();
        }
    }
};
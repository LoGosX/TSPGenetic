#pragma once
#include <vector>
#include <algorithm>
#include <execution>
#include <utility>
#include <fstream>
#include <string>
#include "GASettings.h"

//ZBIGNIEW MICHALEWICZ ALG GEN _ STR DAN = PROG EWOL ~STRONA 241+

//później rozdzielę na .h i .cpp
template <typename Chromosome, typename ChromosomeCreator, typename Crosser, typename Mutator, typename Evaluator, typename LocalSearch>
class GeneticAlgorithm
{
    /*
    Chromosome - represents one solution from a population
    Crosser - defines cross operation between two chromosomes
    Mutator - defines mutation operation on a chromosome
    Evaluator - evaluates the solutions (larger == better)
    ChromosomeCreator - creates and initializes chromosomes
    LocalSearch - imporves current solution
    */
    Crosser crosser;
    Mutator mutator;
    Evaluator evaluator;
    ChromosomeCreator creator;
    LocalSearch local;

    std::vector<Chromosome> population;
    std::vector<float> evaluations;
    std::vector<std::pair<float, int>> cum_dist; //used in parent selection (roulette method)
    Chromosome best_chrom_ever;                  //best chromosome from all generations
    float best_evaluation{-10000000000.F};       // -INF
    float curr_avg_fitness;
    float total_population_fitness;
    float population_variance;
    float population_std_devation;
    float pop_highest_eval;
    float pop_lowest_eval;
    
    std::ofstream stats_file;
    bool file_good{false};

    void measureStatictics(bool measure_fitness = false);

    void measurePopulationFitness();

    void fillCumDist();

    int getChromosomeIndexFromDist(float rand_val);

    void chooseNextPopulation();

    void crossover();

    void mutate();

    void dumpStats();

    std::string file_prefix = "stats";
    void openFile();

    void dumpBestChrom();

    GASettings settings;

public:
    void setMutationProb(float prob);
    void setCrossoverProb(float prob);
    void setElitismPercent(float p);
    void setLocalSearchRate(int rate);
    void setSettings(GASettings settings);

    GeneticAlgorithm(ChromosomeCreator creator, Crosser cross, Mutator mut, Evaluator ev, LocalSearch loc);
    void initPopulation();
    void initPopulation(const std::vector<Chromosome> &pop);
    std::vector<Chromosome> getPopulation() const;

    Chromosome getBestChromosomeEver() const;

    void setFilePrefix(std::string prefix);

    void run();
};


/////////////////////////////////////DEFINITIONS//////////////////////////////////////////////////
/////////////////////////////////////DEFINITIONS//////////////////////////////////////////////////
/////////////////////////////////////DEFINITIONS//////////////////////////////////////////////////
/////////////////////////////////////DEFINITIONS//////////////////////////////////////////////////

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::run()
{
    if (settings.save_results)
    {
        openFile();
        if (file_good)
        {
            measureStatictics();
            dumpStats();
        }
    }
    for (int i = 0; i < settings.n_epochs; i++)
    {
        if((i + 1) % 10 == 0) {
            std::cout << i + 1 << '/' << settings.n_epochs << std::endl;
        }
        chooseNextPopulation();
        crossover();
        mutate();
        if ((i + 1) % std::abs(settings.local_search_rate) == 0)
        {
            if (settings.local_search_rate < 0){
                std::for_each(
                    std::execution::par_unseq,
                    population.begin(),
                    population.end(),
                    [&](auto&& chrom){ while(local(chrom)); }
                    );
                }
            else{
                std::for_each(
                    std::execution::par_unseq,
                    population.begin(),
                    population.end(),
                    local
                    );
                }
        }
        if (file_good)
        {
            measureStatictics(false);
            dumpStats();
        }
    }
    while (local(best_chrom_ever))
        ; //full 2opt
    if (file_good)
        stats_file.close();
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::measureStatictics(bool measure_fitness)
{
    if (!measure_fitness)
        measurePopulationFitness();
    population_variance = 0;
    for (int i = 0; i < settings.pop_size; i++)
        population_variance += (evaluations[i] - curr_avg_fitness) * (evaluations[i] - curr_avg_fitness);
    population_variance /= settings.pop_size;
    population_std_devation = sqrtf(population_variance);
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::measurePopulationFitness()
{
    total_population_fitness = 0;
    pop_lowest_eval = 10000000000;
    pop_highest_eval = -1;
    for (int i = 0; i < settings.pop_size; i++)
    {
        evaluations[i] = evaluator(population[i]);
        total_population_fitness += evaluations[i];
        if (evaluations[i] > pop_highest_eval)
            pop_highest_eval = evaluations[i];
        if (evaluations[i] < pop_lowest_eval)
            pop_lowest_eval = evaluations[i];
        if (evaluations[i] > best_evaluation)
        {
            best_chrom_ever = population[i];
            best_evaluation = evaluations[i];
        }
    }
    curr_avg_fitness = total_population_fitness / settings.pop_size;
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::fillCumDist()
{
    measurePopulationFitness();
    for (int i = 0; i < settings.pop_size; i++)
        cum_dist[i] = {evaluations[i] / total_population_fitness, i};
    std::sort(begin(cum_dist), end(cum_dist));
    for (int i = 1; i < settings.pop_size; i++)
        cum_dist[i].first += cum_dist[i - 1].first;
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
int GeneticAlgorithm<A, B, C, D, E, F>::getChromosomeIndexFromDist(float rand_val)
{
    auto it = std::upper_bound(begin(cum_dist), end(cum_dist), std::pair<float, int>(randomFloat(), -1));
    if (it == end(cum_dist))
        return cum_dist.back().second;
    return it->second;
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::chooseNextPopulation()
{
    fillCumDist();
    std::vector<A> next_pop(settings.pop_size);
    for (auto &chrom : next_pop)
        chrom = population[getChromosomeIndexFromDist(randomFloat())];

    int t = settings.pop_size * settings.elitism_percent;
    for (int i = 0; i < t; i++)
        next_pop[i] = population[cum_dist[settings.pop_size - i - 1].second];
    population = next_pop;
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::crossover()
{
    for (int i = settings.pop_size * settings.elitism_percent; i < settings.pop_size; i++)
    {
        if (randomFloat() < settings.crossover_probability)
        {
            int chrom_a = randomInt(0, settings.pop_size);
            int chrom_b = randomInt(0, settings.pop_size);
            crosser(population[chrom_a], population[chrom_b]);
        }
    }
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::mutate()
{
    /*
    for (int i = settings.pop_size * settings.elitism_percent; i < settings.pop_size; i++)
        if (randomFloat() < settings.mutation_probability)
            mutator(population[i]);
    */
    std::for_each(
        std::execution::par_unseq,
        population.begin() + settings.pop_size * settings.elitism_percent,
        population.end(),
        mutator
    );
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::dumpStats()
{
    stats_file
        << "AvgFitness=" << curr_avg_fitness
        << " StdDev=" << population_std_devation
        << " GenBestFitness=" << pop_highest_eval
        << " GenLowestFitness=" << pop_lowest_eval
        << " BestFitness=" << best_evaluation << '\n';
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::openFile()
{
    std::string path =
        std::string("../graphs/") + file_prefix +
        std::string("_epochs=") + std::to_string(settings.n_epochs) +
        std::string("_pop=") + std::to_string(settings.pop_size) +
        std::string("_cross=") + std::to_string(settings.crossover_probability) +
        std::string("_mut=") + std::to_string(settings.mutation_probability) +
        std::string("_elitism_percent=") + std::to_string(settings.elitism_percent) +
        std::string("local_search_rate=") + std::to_string(settings.local_search_rate) +
        std::string(".txt");
    stats_file.open(path, std::ios::trunc | std::fstream::in | std::fstream::out);
    file_good = stats_file.good();
    if (!file_good)
        std::cerr << "Could not open " << path << '\n';
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::dumpBestChrom()
{
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::initPopulation()
{
    population = std::vector<A>(settings.pop_size, creator());
    for (auto &chrom : population)
        creator(chrom); //random initialize chromosome
    evaluations.resize(settings.pop_size);
    cum_dist.resize(settings.pop_size);
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::initPopulation(const std::vector<A> &pop)
{
    settings.pop_size = pop.size();
    population = pop;
    evaluations.resize(settings.pop_size);
    cum_dist.resize(settings.pop_size);
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
std::vector<A> GeneticAlgorithm<A, B, C, D, E, F>::getPopulation() const { return population; }

template <typename A, typename B, typename C, typename D, typename E, typename F>
A GeneticAlgorithm<A, B, C, D, E, F>::getBestChromosomeEver() const { return best_chrom_ever; }

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::setFilePrefix(std::string prefix)
{
    file_prefix = prefix;
}

template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::setMutationProb(float prob) { settings.mutation_probability = prob; }
template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::setCrossoverProb(float prob) { settings.crossover_probability = prob; }
template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::setElitismPercent(float p) { settings.elitism_percent = p; }
template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::setLocalSearchRate(int rate) { settings.local_search_rate = rate; }
template <typename A, typename B, typename C, typename D, typename E, typename F>
void GeneticAlgorithm<A, B, C, D, E, F>::setSettings(GASettings s) { settings = s; }

template <class Chromosome, class ChromosomeCreator, class Crosser, class Mutator, class Evaluator, class LocalSearch>
GeneticAlgorithm<Chromosome, ChromosomeCreator, Crosser, Mutator, Evaluator, LocalSearch>::
    GeneticAlgorithm(ChromosomeCreator creator, Crosser cross, Mutator mut, Evaluator ev, LocalSearch loc) 
        : creator(creator), crosser(cross), mutator(mut), evaluator(ev), local(loc) {}
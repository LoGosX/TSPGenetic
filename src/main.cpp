#include <iostream>
#include <vector>



//ZBIGNIEW MICHALEWICZ ALG GEN _ STR DAN = PROG EWOL ~STRONA 241+

template<class Unit, class Crosser, class Mutator, class Evaluator>
class GeneticAlgorithm
{
    std::vector<Unit> population;
    std::vector<std::pair<float, int>> cum_dist; //dystrybuanta (wartość dystrybuanty, indeks jednostki w wektorze) 
    Unit best_unit_ever; //best unit from all generations
    int size;
    float cross_prob;
    float mutation_prob;

    std::vector<Unit> selectToNextGen() //using cumulative distribution 
    { 
        std::vector<Unit> next_pop;
    }

public:

    GeneticAlgorithm(int size, float corss_prob, float mutation_prob) 
        : size(size), cross_prob(cross_prob), mutation_prob(mutation_prob)
        {
        }

    void setPopulation(const std::vector<Unit>& pop)
    {
        population = pop;
    }

    void run(int epochs, float epsi = 0.000003f)
    {
        for(int i = 0; i < epochs /* && poprawa wieksza niz epsilon (np przez ostatnie 5 iteracji) */; i++)
        {
            selectToNextGen(); //evaluate and select
            //crossover
            //muatate
            //repeat

            //size * cross_prob
            // for(int i = 0; i < size; i++)
            //  if(rand_val() < cross_prob)
            //      Crosser()(random_unit_1, random_unit_2)
            // mutate
            // for(unit in population)
            //  if(rand_val() < mut_prob)
            //      Mutator()(unit)

            BaseRepr a,b;
            Crosser()(a,b);
            Mutator()(a);
            Mutator()(b);
        }
    }
};

// przykład "klas które pełnią rolę operatorów"
struct BaseRepr
{
    float v;
};

struct Mutator
{
    void operator()(BaseRepr& base_repr)
    {
        base_repr.v = -base_repr.v;
    }
};

struct Crosser
{
    void operator()(BaseRepr& a, BaseRepr& b)
    {
        std::swap(a.v, b.v);
    }
};

struct Evaluator
{
    float operator()(const BaseRepr& a)
    {
        return a.v;
    }
};

int main()
{
    std::cout << "Hello World!\n";
    GeneticAlgorithm<BaseRepr, Crosser, Mutator, Evaluator> ga;
    return 0;
}
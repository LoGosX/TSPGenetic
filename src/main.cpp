#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "GeneticAlgorithm.h"
#include "ExampleOperators.h"


int main()
{
    srand(time(0));

    //przykład z rozdziału 1.1
    //BinaryChrom<10> liczba jest reprezentowana na 22 bitach (jak w przykładzie w książce)
    GeneticAlgorithm<BinaryChrom<22>, BinaryCrosser, BinaryMutator, BinaryEvaluator> ga;
    ga.initPopulation(50);
    ga.setCrossoverProb(0.25F);
    ga.setMutationProb(0.1); //to nie jest dokładnie to samo co w książce ale co tam
    ga.run(150);
    BinaryEvaluator ev;
    BinaryChrom chr = ga.getBestChromosomeEver();
    std::cout << "Maksymalna wartość funkcji f(x) = x * sin(10*pi*x) + 1.0 po 150 iteracjach wynosi:\n"
        "f(" << ev.getX(chr) << ") = " << ev(chr) << '\n';
    return 0;
}
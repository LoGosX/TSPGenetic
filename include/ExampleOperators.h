#pragma once
#include <array>
#include "math_helpers.h"

//minimalizacja prostej funkcji, rozdział 1.1 z książki jak dobrze pamiętam

template<int N>
struct BinaryChrom
{
    std::array<int, N> digits;
    BinaryChrom() // chromosome should self-initialize
    {
        for(int& digit : digits)
            digit = (randomFloat() > 0.5 ? 0 : 1);
    }
};

struct BinaryEvaluator
{
    //optymalizujemy funkcję f(x) = x * sin(10 * pi * x) + 1 na przedziale [-1, 2]
    template<int N>
    float getX(const BinaryChrom<N>& chrom)
    {
        float tmp = 1;
        float val = 0;
        for(int i = N - 1; i >= 0; i--)
        {
            val += tmp * chrom.digits[i];
            tmp *= 2;
        }
        float left = -1; //lewa strona przedziału [-1, 2]
        float right = 2; //prawa strona przedziału [-1, 2]
        float x = (val / tmp) * (right - left) + left; //x reprezentowany przez dany Chromosom
        return x;
    }

    template<int N>
    float operator()(const BinaryChrom<N>& chrom)
    {
        float x = getX(chrom);
        return x * sinf(10 * 3.14 * x) + 1.0;
    }
};

struct BinaryMutator 
{
    // do przedyskutowania
    //(1) czy mutation_prob dotyczy szansy na mutację osobnika
    //(2) czy mutation_prob dotyczy szansy na mutację indywidualnego bitu w osobniku
    //(1) - zawsze będzie mutowana taka sami liczba bitów w osobnikach
    //(2) - średnio taka sama, ale nie zawsze
    //tutaj jest opcja (1)
    template<int N>
    void operator()(BinaryChrom<N>& chrom)
    {
        int rand_index = randomInt(0, N);
        chrom.digits[rand_index] = (chrom.digits[rand_index] + 1) % 2; // 0 -> 1, 1 -> 0 
    }
};

struct BinaryCrosser
{
    template<int N>
    void operator()(BinaryChrom<N>& a, BinaryChrom<N>& b)
    {
        int split = randomInt(0, N+1);
        for(int i = split; i < N; i++)
            std::swap(a.digits[i], b.digits[i]);
    }
};
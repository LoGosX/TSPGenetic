#pragma once
#include <string>

struct GASettings {
    int n_epochs;
    int pop_size;
    float crossover_probability;
    float mutation_probability;
    float elitism_percent;
    int local_search_rate;
    bool save_results {false};
    std::string instance_file_path;
};
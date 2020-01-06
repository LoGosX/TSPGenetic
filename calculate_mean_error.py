import os
import numpy as np
from itertools import chain
from matplotlib import pyplot as plt

DATASETS_RELATIVE_DIR = './datasets/'

BAR_WIDTH = 1
BARS_SEPARATION_DISTANCE = 5

def load_results(instance_name):
    file_name = instance_name + '.results'
    file_path = DATASETS_RELATIVE_DIR + file_name
    with open(file_path, 'r') as file:
        distances = []
        for line in file:
            if line.startswith('Distance'):
                distances.append(float(line.split(' ')[-1]))
    return np.array([distances])

def load_benchmark_distance(instance_name):
    file_name = instance_name + '.txt'
    file_path = DATASETS_RELATIVE_DIR + file_name
    with open(file_path, 'r') as file:
        for line in file:
            pass
        return float(line)

# tsp instances a
benchmark_instances = [
    'berlin52',
    'ch130',
    'ch150',
]

results = [load_results(instance) for instance in benchmark_instances]
results = np.array([np.mean(results) for results in results])
benchmark_distances = np.array([load_benchmark_distance(instance) for instance in benchmark_instances])
N_RESULTS = results.shape[0]
bars_x = []
bars_heights = []
for i in range(N_RESULTS):
    x = (i - 1) * BARS_SEPARATION_DISTANCE + 2 * i * BAR_WIDTH
    bars_x.extend([x, x + BAR_WIDTH])
    bars_heights.extend([benchmark_distances[i], results[i]])
labels = list(chain.from_iterable((instance, '') for instance in benchmark_instances))
colors = 'gr' * N_RESULTS
plt.barh(bars_x, bars_heights, tick_label=labels, color=colors)
plt.xlabel('path length')
plt.ylabel('instance')
plt.show()
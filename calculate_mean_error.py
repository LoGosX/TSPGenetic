import os
import numpy as np
from itertools import chain
from matplotlib import pyplot as plt

DATASETS_RELATIVE_DIR = './datasets/'

BAR_WIDTH = 20
BARS_SEPARATION_DISTANCE = 5

# tsp instances taken from http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsp/index.html
# should be at least 10 instances
benchmark_instances = [
    'a280',
    'st70',
    'berlin52',
    'bier127',
    'ch130',
    'ch150',
    'd198',
    'd493',
    'eil51',
    'eil76',
    'eil101',
]


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


results = np.array([np.min(load_results(instance)) for instance in benchmark_instances])
benchmark_distances = np.array([load_benchmark_distance(instance) for instance in benchmark_instances])
results = results / benchmark_distances * 100
N_RESULTS = results.shape[0]
bars_x = []
bars_heights = []
for i in range(N_RESULTS):
    x = (i - 1) * BARS_SEPARATION_DISTANCE + 2 * i * BAR_WIDTH
    bars_x.extend([x, x + BAR_WIDTH])
    bars_heights.extend([100, results[i]])
tick_labels = list(chain.from_iterable((instance, '') for instance in benchmark_instances))
labels = [f"{x:.2f}%" for x in results]
print(labels)
colors = 'by' * N_RESULTS

fig, ax = plt.subplots()
barh_plot = plt.barh(bars_x, bars_heights, tick_label=tick_labels, color=colors, height=BAR_WIDTH)


def autolabel(rects):
    for idx, rect in enumerate(barh_plot):
        width = rect.get_width()
        if idx % 2 == 1:
            ax.text(1 * width, rect.get_y(), labels[idx // 2], ha='left', va='bottom', rotation=0)

    ax.set_xticklabels([])


autolabel(barh_plot)

plt.xlabel('Długość drogi jako % drogi uznanej za optymalną')
plt.ylabel('Instancje')
plt.title("Względni błąd naszych wyników jako % wyniku optymalnego.")
plt.xlim(0, 120)
plt.show()

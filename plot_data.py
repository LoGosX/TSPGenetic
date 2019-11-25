import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict
import sys

def load_data(filename):
    data = defaultdict(list)
    lines = 0
    with open(filename, "r") as f:
        for line in f:
            lines += 1
            fields = line.split(' ')
            for field in fields:
                key,val = field.split("=")
                data[key].append(float(val))
    return data

# Data for plotting
filename = sys.argv[1]
data = load_data(filename)
fig, ax = plt.subplots()
for key, val in data.items():
    ax.plot(val, label=key)

plt.legend()
fig.savefig(filename[:-3] + ".png")
plt.show()
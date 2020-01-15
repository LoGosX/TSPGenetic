import matplotlib.pyplot as plt
import numpy as np
import sys

def load_data(filename):
    dx = []
    dy = []
    try:
        with open(filename, "r") as f:
            _ = f.readline()
            for line in f:
                data = line.split()
                if len(data) == 3:
                    index, x, y = line.split()
                    dx.append(float(x))
                    dy.append(float(y))
    except IOError:
        print("Nie można znaleźć pliku", filename)
        sys.exit(-1)
    return dx,dy


filename = sys.argv[1]
path = list(map(int, sys.argv[2:]))
if min(path) == 1:
    path = [p - 1 for p in path]
x,y = load_data(filename)
X = [x[p] for p in path]
Y = [y[p] for p in path]

plt.plot(X, Y, 'o-', linewidth=.5, markersize=3)
plt.show()
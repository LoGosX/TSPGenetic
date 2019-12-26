import matplotlib.pyplot as plt
import numpy as np

def load_data(filename):
    dx = []
    dy = []
    with open(filename, "r") as f:
        _ = f.readline()
        for line in f:
            index, x, y = line.split()
            dx.append(float(x))
            dy.append(float(y))
    return dx,dy

def load_path(filename):
    filename = filename.split('.')[0] + '.results'
    result = None
    with open(filename, 'r') as f:
        for line in f:
            if line.startswith('Path:'):
                result = line.split(':')[1]
    return list(map(int,result.strip().split()))

# Data for plotting
filename = input("Path to dataset: ")
path = load_path(filename)
x,y = load_data(filename)
X = [x[p-1] for p in path]
Y = [y[p-1] for p in path]
plt.scatter(X, Y)
plt.plot(X, Y)
plt.legend(X, Y)
plt.show()
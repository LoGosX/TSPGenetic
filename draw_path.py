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

# Data for plotting
filename = input("Path to dataset: ")
path = list(map(int,input("Path: ").split()))
x,y = load_data(filename)
X = [x[p-1] for p in path]
Y = [y[p-1] for p in path]
plt.scatter(X, Y)
plt.plot(X, Y)
plt.legend(X, Y)
plt.show()
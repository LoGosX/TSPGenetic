import numpy as np
import matplotlib
from matplotlib import pyplot as plt
matplotlib.use("pgf")
matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    'font.family': 'serif',
    'text.usetex': True,
    'pgf.rcfonts': False,
})


def plot_path(X, Y, path):
    plt.scatter(X, Y)
    plt.plot(X[path], Y[path])
    plt.axis('off')
    plt.show()

GENERATE = False

np.random.seed(42)
N_POINTS = 15
X = (np.random.rand(N_POINTS) * 101).astype(int)
Y = (np.random.rand(N_POINTS) * 101).astype(int)

if GENERATE:
    tour = np.random.permutation(N_POINTS)
    plot_path(X, Y, tour)
    plt.savefig('instancja_poczatkowa.pgf')

    with open("punkty.txt", "r+") as f:
        f.truncate()
        for x,y in zip(X, Y):
            f.write(f"{x} {y}\n")
        f.write(' '.join(str(p) for p in tour))
else:
    dane = [
[np.array([ 3, 5, 13, 11, 7, 1, 12, 9, 2, 8, 10, 4, 6, 14, 0, 3]), 444.006],
[np.array([ 7, 11, 1, 12, 2, 9, 3, 8, 13, 5, 10, 4, 6, 14, 0, 7]), 317.746],
[np.array([ 14, 6, 4, 10, 5, 13, 3, 9, 2, 12, 1, 11, 7, 8, 0, 14]), 315.92],
    ]
    print(dane[0][1])
    plot_path(X, Y, dane[0][0])
    print(dane[1][1])
    plt.savefig('instancja_przed_2opt.pgf')
    plot_path(X, Y, dane[1][0])
    plt.savefig('instancja_po_2opt.pgf')
    print(dane[-1][1])
    plot_path(X, Y, dane[-1][0])
    plt.savefig('instancja_koncowa.pgf')
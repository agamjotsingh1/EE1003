import matplotlib.pyplot as plt
from scipy.stats import binom 
from math import sqrt, pi
import numpy as np
import ctypes

# dll linking
dll = ctypes.CDLL('./sim.so')

# describing the argument and return types of the function 'binomPmfPlot' and 'freeSingleMem' in the dll
dll.binomPmfPlot.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]
dll.binomPmfPlot.restype = ctypes.POINTER(ctypes.c_float)

dll.freeSingleMem.argtypes = [ctypes.POINTER(ctypes.c_float)]
dll.freeSingleMem.restype = None

''' GAUSSIAN '''
# returns the theoritical binomial pmf distribution
def pmf_binomial(n, p):
    # defining the list of x values 
    x = np.arange(0, n + 1)

    # list of pmf values 
    y = binom.pmf(x, n, p)

    return x, y

def gaussian(n, p, c):
    mean = n * p
    variance = n * p * (1 - p)

    iters = 100000 # no of iterations
    simPmf = dll.binomPmfPlot(n, iters, p)

    # plotting the plot using plt.scatter
    coords = []
    for pt in zip(range(0, n + 1), simPmf[:(n + 1)]):
        coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

    coords_plot = np.block(coords)

    # plotting the simulation
    plt.stem(coords_plot[0,:], coords_plot[1,:], c, label=f'Sim m = {n}') 

    # freeing the memory of simPmf
    dll.freeSingleMem(simPmf)

    # plotting the gaussian plot
    x_lin = np.linspace(0, n, 1000)
    y_lin = np.exp(-np.power(x_lin - mean, 2)/(2 * variance))/sqrt(2*pi*variance)

    plt.plot(x_lin, y_lin, label=f'Gaussian m = {n}', color=c)

''' PLOTTING 4 GAUSSIAN AND SIMULATED PLOTS'''
p = 0.5

colors = ['#d62728', '#2ca02c', '#ff7f0e', '#1f77b4']
n_vals = [15, 25, 50, 100]

for i in range(4):
    plt.figure()
    gaussian(n_vals[i], p, colors[i])
    plt.legend(loc="best")
    plt.savefig(f"../figs/gauss_{i}.png")
    plt.show()

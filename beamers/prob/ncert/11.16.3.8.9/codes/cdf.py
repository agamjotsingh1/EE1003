import matplotlib.pyplot as plt
from scipy.stats import binom 
from math import sqrt, pi
import numpy as np
import ctypes

''' THEORITICAL '''
# returns the theoritical binomial cdf distribution
def cdf_binomial(n, p):
    # defining the list of x values 
    x = np.arange(0, n + 1)

    # list of cdf values 
    y = binom.cdf(x, n, p)

    return x, y

n = 3 # number of tosses
p = 0.5 # probability of heads

x, y = cdf_binomial(n, p) # get the probability distribution

''' SIMULATION '''
# dll linking
dll = ctypes.CDLL('./sim.so')

# describing the argument and return types of the function 'binomCdfPlot' and 'freeSingleMem' in the dll
dll.binomCdfPlot.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]
dll.binomCdfPlot.restype = ctypes.POINTER(ctypes.c_float)

dll.freeSingleMem.argtypes = [ctypes.POINTER(ctypes.c_float)]
dll.freeSingleMem.restype = None

iters = 100000
simcdf = dll.binomCdfPlot(n, iters, p)

# plotting the plot using plt.scatter
coords = []
for pt in zip(range(0, n + 1), simcdf[:(n + 1)]):
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)

# freeing the memory of simcdf
dll.freeSingleMem(simcdf)

# Show the thoeritical and simulated cdf
markerline, stemlines, _ = plt.stem(coords_plot[0,:], coords_plot[1,:], 'red', label = "Sim") 
plt.setp(markerline, 'markersize', 8)
plt.stem(x, y, label="Theory")
plt.legend(loc="best")
plt.savefig("../figs/cdf.png")
plt.show()

import matplotlib.pyplot as plt
from scipy.stats import binom 
from math import sqrt, pi
import numpy as np

# returns the binomial pmf distribution
def pmf_binomial(n, p):
    # defining the list of x values 
    x = np.arange(0, n + 1)

    # list of pmf values 
    y = binom.pmf(x, n, p)

    return x, y

n = 3 # number of tosses
p = 0.5 # probability of heads

x, y = pmf_binomial(n, p) # get the probability distribution

# Plot the pmf
plt.figure()
plt.stem(x, y, label="PMF $P_{X}(n)$")
plt.legend(loc="upper right")
plt.savefig("../figs/pmf1.png")
plt.show()

n = 15
p = 0.5

mean = n * p
variance = n * p * (1 - p)

# plotting gaussian distribution and pmf for n = 10
x, y = pmf_binomial(n, p)
plt.figure()
plt.stem(x, y)

x_lin = np.linspace(0, n, 1000)
y_lin = np.exp(-np.power(x_lin - mean, 2)/(2 * variance))/sqrt(2*pi*variance)

plt.plot(x_lin, y_lin, color="orange", label="Gaussian distribution m = 15")

plt.legend(loc="upper right")
plt.savefig("../figs/pmf2.png")
plt.show()

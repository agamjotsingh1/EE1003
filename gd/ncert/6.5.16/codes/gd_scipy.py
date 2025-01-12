import numpy as np
from scipy.optimize import minimize

# Define the objective function
def objective_function(x):
    return x[0]**3 + (16 - x[0])**3

# Initial guess for x
x0 = [7]  # Starting guess

# Use scipy.optimize.minimize to minimize the function
result = minimize(objective_function, x0, method='BFGS')

# Print the results
print("Optimization Result:")
print(f"Minimum value of the function: {result.fun}")
print(f"Value of x at the minimum: {result.x[0]}")
print(f"Success: {result.success}")
print(f"Message: {result.message}")

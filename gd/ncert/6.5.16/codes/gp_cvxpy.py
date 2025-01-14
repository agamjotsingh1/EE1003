import cvxpy as cp

# Define the variable
x = cp.Variable(pos=True)

# Define the objective function f0: x^3 + (16 - x)^3
objective = cp.Minimize(3*(x**2) - 48*x + 256)
constraints = []

# Define and solve the problem
problem = cp.Problem(objective, constraints)
problem.solve(gp=True)

# Output the optimal value and the optimal solution
print(f"f_min: {problem.value}")
print(f"x_min: {x.value}")

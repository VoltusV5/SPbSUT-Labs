import numpy as np
import matplotlib.pyplot as plt

H, l, p = 18, 64, 0.001
U = np.arange(50, 15001, 10)  # Диапазон до 15000
L = H + U
k = 1 / (1 - p) ** (8 * L)
E = U / ((L + l) * k)

U_opt = U[np.argmax(E)]
E_max = np.max(E)
print(f"U_opt = {U_opt}, E_max = {E_max:.3f}")

plt.plot(U, E)
plt.xlabel('U (байт)')
plt.ylabel('E')
plt.grid()
plt.savefig('E_vs_U.png')
plt.show()
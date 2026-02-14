import numpy as np
import matplotlib.pyplot as plt

mu = 1.0
a = 0.5
t = np.linspace(0, 10 / mu, 1000)

# ФРВ конца обслуживания для M/D/1
Fe_md1 = np.where(t < 1/mu, 
                  0, 
                  1 - a**(mu * t - 1))

plt.figure(figsize=(9, 5))
plt.plot(t, Fe_md1, label='M/D/1, a = 0.5', color='red', linewidth=2)
plt.title('Функция распределения времени окончания обслуживания (M/D/1)')
plt.xlabel('Время, t  (в единицах 1/μ)')
plt.ylabel('F_e(t)')
plt.grid(True, alpha=0.3)
plt.legend()
plt.xlim(0, 10/mu)
plt.ylim(0, 1.05)
plt.axvline(1/mu, color='black', linestyle=':', label='t = 1/μ')
plt.legend()
plt.show()
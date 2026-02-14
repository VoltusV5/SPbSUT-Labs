import numpy as np
import matplotlib.pyplot as plt

# --- Параметры ---
mu = 1.0          # Нормируем: μ = 1 (время в единицах 1/μ)
a = 0.5           # Загрузка системы (по доске: P₀ = 1 - a → a = 0.5)
# -----------------

# Время от 0 до 10/μ (10 средних времен обслуживания)
t = np.linspace(0, 10 / mu, 1000)

# ФРВ конца обслуживания для M/M/1
Fe = 1 - np.exp(-mu * (1 - a) * t)

# Построение графика
plt.figure(figsize=(9, 5))
plt.plot(t, Fe, label='M/M/1, a = 0.5', color='blue', linewidth=2)
plt.title('Функция распределения времени окончания обслуживания (M/M/1)')
plt.xlabel('Время, t  (в единицах 1/μ)')
plt.ylabel('F_e(t)')
plt.grid(True, alpha=0.3)
plt.legend()
plt.xlim(0, 10/mu)
plt.ylim(0, 1.05)
plt.axhline(1, color='gray', linestyle='--', alpha=0.5)
plt.show()
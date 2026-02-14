import numpy as np
import matplotlib.pyplot as plt
from scipy.special import comb

# Параметры
x1 = 2  # Количество вызовов в обслуживании
x2 = 3  # Среднее время обслуживания в секундах
mu = 1 / x2  # Интенсивность освобождения
t_values = np.linspace(0, 10, 100)  # Время (t) от 0 до 10 секунд

# Формула для вероятности освобождения i вызовов из x
def probability_all_released(i, x, t, mu):
    return comb(x, i) * (1 - np.exp(-mu * t))**i * np.exp(-(x - i) * mu * t)

# Формула для вероятности, что не освободится ни один вызов
def probability_none_released(x, t, mu):
    return np.exp(-x * mu * t)

# Формула для вероятности, что освободится хотя бы один вызов
def probability_at_least_one_released(x, t, mu):
    return 1 - probability_none_released(x, t, mu)

# Рассчитываем вероятности
prob_all_released = [probability_all_released(i, x1, t, mu) for t in t_values for i in [x1]]
prob_none_released = [probability_none_released(x1, t, mu) for t in t_values]
prob_at_least_one_released = [probability_at_least_one_released(x1, t, mu) for t in t_values]

# Построение графиков
plt.figure(figsize=(10, 6))

# График для вероятности освобождения всех вызовов
plt.subplot(3, 1, 1)
plt.plot(t_values, prob_all_released, label="Освободятся все вызовы", color='blue')
plt.title("Вероятность освобождения всех вызовов")
plt.xlabel("Время (сек)")
plt.ylabel("Вероятность")
plt.grid(True)
plt.legend()

# График для вероятности, что не освободится ни один вызов
plt.subplot(3, 1, 2)
plt.plot(t_values, prob_none_released, label="Не освободится ни один вызов", color='red')
plt.title("Вероятность, что не освободится ни один вызов")
plt.xlabel("Время (сек)")
plt.ylabel("Вероятность")
plt.grid(True)
plt.legend()

# График для вероятности, что освободится хотя бы один вызов
plt.subplot(3, 1, 3)
plt.plot(t_values, prob_at_least_one_released, label="Освободится хотя бы один вызов", color='green')
plt.title("Вероятность, что освободится хотя бы один вызов")
plt.xlabel("Время (сек)")
plt.ylabel("Вероятность")
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()

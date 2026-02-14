import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import poisson, expon
import math

x1 = 2
x2 = 3

print("\n2.1")
lambda1 = x1
t1 = x2

p_no_calls = math.exp(-lambda1 * t1)
print(f"Интенсивность потока λ = {lambda1} с⁻¹")
print(f"Время наблюдения t = {t1} с")
print(f"Вероятность отсутствия вызовов: P(0) = {p_no_calls:.6f}")
print(f"Вероятность поступления хотя бы одного вызова: {1-p_no_calls:.6f}")

print("2.2")

lambda2_1 = x1
t2_1 = x2
k_values_1 = np.arange(0, 60)

print(f"\nСлучай 1: λ = {lambda2_1} с⁻¹, t = {t2_1} с")
print("k\tP(k)\t\tМатематическое ожидание: λt =", lambda2_1 * t2_1)

probabilities_1 = []
for k in k_values_1:
    p = poisson.pmf(k, lambda2_1 * t2_1)
    probabilities_1.append(p)
    print(f"{k}\t{p:.6f}")

lambda2_2 = x2
t2_2 = 0.01
k_values_2 = np.arange(0, 10)

print(f"\nСлучай 2: λ = {lambda2_2} с⁻¹, t = {t2_2} с")
print("k\tP(k)\t\tМатематическое ожидание: λt =", lambda2_2 * t2_2)

probabilities_2 = []
for k in k_values_2:
    p = poisson.pmf(k, lambda2_2 * t2_2)
    probabilities_2.append(p)
    print(f"{k}\t{p:.6f}")

plt.figure(figsize=(12, 5))

plt.subplot(1, 2, 1)
plt.plot(k_values_1, probabilities_1, 'bo-', linewidth=2, markersize=6, label='P(k)')
plt.xlabel('Число вызовов k')
plt.ylabel('Вероятность P(k)')
plt.title(f'Распределение Пуассона: λ={lambda2_1} с⁻¹, t={t2_1} с\nλt={lambda2_1*t2_1:.1f}')
plt.grid(True, alpha=0.3)
plt.xticks(k_values_1)

max_prob_1 = max(probabilities_1)
max_k_1 = k_values_1[np.argmax(probabilities_1)]
plt.plot(max_k_1, max_prob_1, 'ro', markersize=8, label=f'Максимум (k={max_k_1})')
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(k_values_2, probabilities_2, 'ro-', linewidth=2, markersize=6, label='P(k)')
plt.xlabel('Число вызовов k')
plt.ylabel('Вероятность P(k)')
plt.title(f'Распределение Пуассона: λ={lambda2_2} с⁻¹, t={t2_2} с\nλt={lambda2_2*t2_2:.3f}')
plt.grid(True, alpha=0.3)
plt.xticks(k_values_2)

max_prob_2 = max(probabilities_2)
max_k_2 = k_values_2[np.argmax(probabilities_2)]
plt.plot(max_k_2, max_prob_2, 'bo', markersize=8, label=f'Максимум (k={max_k_2})')
plt.legend()

plt.tight_layout()
plt.show()

print(f"Для первого случая: максимум при k = {max_k_1}, P({max_k_1}) = {max_prob_1:.6f}")
print(f"Математическое ожидание λt = {lambda2_1 * t2_1:.1f}")
print(f"Для второго случая: максимум при k = {max_k_2}, P({max_k_2}) = {max_prob_2:.6f}")
print(f"Математическое ожидание λt = {lambda2_2 * t2_2:.3f}")

print("2.3")
mu1 = x1
mu2 = x2

t_values = np.linspace(0, 5, 500)

F1 = 1 - np.exp(-mu1 * t_values)
F2 = 1 - np.exp(-mu2 * t_values)

plt.figure(figsize=(10, 6))
plt.plot(t_values, F1, 'b-', linewidth=2, label=f'μ = {mu1} с⁻¹')
plt.plot(t_values, F2, 'r-', linewidth=2, label=f'μ = {mu2} с⁻¹')
plt.xlabel('Время обслуживания, с')
plt.ylabel('Функция распределения F(t)')
plt.title('Функция распределения времени обслуживания (экспоненциальное распределение)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()

print(f"- При μ = {mu1} с⁻¹: среднее время обслуживания = {1/mu1:.2f} с")
print(f"  F(t) = 1 - e^(-{mu1}·t)")
print(f"- При μ = {mu2} с⁻¹: среднее время обслуживания = {1/mu2:.2f} с")
print(f"  F(t) = 1 - e^(-{mu2}·t)")

print("2.4")

k = 20 + x1 + x2
lambda4 = x2
t4_min = x1 / 100
t4_sec = t4_min * 60

lambda_t = lambda4 * t4_sec

p_k = poisson.pmf(k, lambda_t)

print(f"Число вызовов k = {k}")
print(f"Интенсивность потока λ = {lambda4} с⁻¹")
print(f"Время наблюдения t = {t4_min:.4f} мин = {t4_sec:.2f} с")
print(f"Параметр распределения λt = {lambda_t:.6f}")
print(f"Вероятность P({k}) = {p_k:.10e}")
print(f"Вероятность P({k}) = {p_k:.15f}")

print("end")
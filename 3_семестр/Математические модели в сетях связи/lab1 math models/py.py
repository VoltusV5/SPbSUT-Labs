import numpy as np
import matplotlib.pyplot as plt
import math

# -------------------------
# Функция формулы Эрланга с log-sum-exp
# -------------------------
def erlang_loss(a, v):
    """
    Версия формулы Эрланга для больших v и маленьких a с log-sum-exp.
    """
    log_terms = np.array([k * np.log(a) - math.lgamma(k + 1) for k in range(v + 1)])
    
    # Сдвиг по максимальному логарифму для численной стабильности
    max_log = np.max(log_terms)
    log_terms_shifted = log_terms - max_log

    log_num = log_terms_shifted[v]      # числитель
    log_den = np.log(np.sum(np.exp(log_terms_shifted)))  # знаменатель

    return np.exp(log_num - log_den)

# -------------------------
# Параметры эксперимента
# -------------------------
N = np.arange(1, 11)
a0 = np.linspace(0.1, 1.0, 10)
v_list = [10, 50, 200]

# -------------------------
# Пример имитационных данных (AnyLogic)
# -------------------------
p_sim = {
    10: [0.091, 0.016, 0.003, 0.000684, 0.000105, 0.0, 0.0, 0.0, 0.0, 0.0],
    50: [0.0]*10,
    200: [0.0]*10
}

# -------------------------
# Расчёт аналитических значений
# -------------------------
# Для визуализации лучше увеличить a для v>10
a_scaled = {
    10: a0*10,      # увеличим a в 10 раз для v=10
    50: a0*50,      # увеличим a в 50 раз для v=50
    200: a0*200     # увеличим a в 200 раз для v=200
}

p_ana = {v: [erlang_loss(a_scaled[v][i], v) for i in range(len(a0))] for v in v_list}

# -------------------------
# Вывод таблицы 1
# -------------------------
print("Таблица 1: Имитация vs Аналитика")
for i in range(len(N)):
    row = f"N={N[i]}, a0={a0[i]:.1f} | "
    for v in v_list:
        row += f"v={v}: Sim={p_sim[v][i]:.5f}, Ana={p_ana[v][i]:.5f}, Δ={p_sim[v][i]-p_ana[v][i]:+.5f} | "
    print(row)

# -------------------------
# Пример графика для Таблицы 1
# -------------------------
plt.figure(figsize=(10,6))
for v in v_list:
    plt.plot(a0, p_sim[v], 'o-', label=f"Sim v={v}")
    plt.plot(a0, p_ana[v], 'x--', label=f"Ana v={v}")
plt.xlabel("Интенсивность нагрузки a0")
plt.ylabel("Вероятность потерь p")
plt.title("Таблица 1: Сравнение имитации и аналитики")
plt.legend()
plt.grid(True)
plt.show()

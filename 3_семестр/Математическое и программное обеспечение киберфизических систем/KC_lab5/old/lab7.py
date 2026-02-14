import numpy as np
import matplotlib.pyplot as plt

x1 = float(input("Введите скорость x1 для интерфейса (Мбит/с): "))

# ---------- 2.1 ----------
len_10 = 158
t_cod_10 = 0.01
bandwidth_10 = (len_10 * 8) / t_cod_10 / 1000
print(f"2.1 Полоса передачи кодека G.711(10): {bandwidth_10:.1f} кбит/с")

# ---------- 2.2 ----------
len_30 = 318
t_cod_30 = 0.03
bandwidth_30 = (len_30 * 8) / t_cod_30 / 1000
print(f"2.2 Полоса передачи кодека G.711(30): {bandwidth_30:.1f} кбит/с")

# ---------- 2.3 ----------
service_time_10 = (len_10 * 8) / (x1 * 1e6) * 1000   # мс
print(f"2.3 Среднее время занятия пакетом (G.711(10)): {service_time_10:.3f} мс")

# ---------- 2.4 ----------
service_time_30 = (len_30 * 8) / (x1 * 1e6) * 1000   # мс
print(f"2.4 Среднее время занятия пакетом (G.711(30)): {service_time_30:.3f} мс")

# ---------- 2.5 ----------
lambda_call = 1000 / 3600

# G.711(10)
lambda_pac_10 = lambda_call * (180 / t_cod_10)
mu_10 = (x1 * 1e6) / (len_10 * 8)
rho_10 = lambda_pac_10 / mu_10

# G.711(30)
lambda_pac_30 = lambda_call * (180 / t_cod_30)
mu_30 = (x1 * 1e6) / (len_30 * 8)
rho_30 = lambda_pac_30 / mu_30

print("\n2.5 Графики ФРВ окончания обслуживания (M/M/1):")

t_ms = np.linspace(0, 10, 1000)
t_sec = t_ms / 1000

# Первый интерфейс – G.711(10)
if rho_10 >= 1:
    print("Для G.711(10): rho >= 1, система нестабильна!")
else:
    print(f"Для G.711(10): rho = {rho_10:.3f}")
    F_10 = 1 - np.exp(-mu_10 * (1 - rho_10) * t_sec)
    plt.figure(1)
    plt.plot(t_ms, F_10)
    plt.title('ФРВ окончания обслуживания (G.711(10))')
    plt.xlabel('Время, мс')
    plt.ylabel('F(t)')
    plt.grid(True)
    plt.show()

# Второй интерфейс – G.711(30)
if rho_30 >= 1:
    print("Для G.711(30): rho >= 1, система нестабильна!")
else:
    print(f"Для G.711(30): rho = {rho_30:.3f}")
    F_30 = 1 - np.exp(-mu_30 * (1 - rho_30) * t_sec)
    plt.figure(2)
    plt.plot(t_ms, F_30)
    plt.title('ФРВ окончания обслуживания (G.711(30))')
    plt.xlabel('Время, мс')
    plt.ylabel('F(t)')
    plt.grid(True)
    plt.show()
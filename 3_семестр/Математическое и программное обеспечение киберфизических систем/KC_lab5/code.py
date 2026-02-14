import numpy as np
import matplotlib.pyplot as plt

x1 = float(input("Введите скорость интерфейса x1 (Мбит/с): "))
x2_kb = float(input("Введите размер пакета x2 (КБ) для задания 2.6: "))
lambda_26 = float(input("Введите λ (пак/с) для задания 2.6: "))

len_10, t_cod_10 = 158, 0.01
len_30, t_cod_30 = 318, 0.03

bandwidth_10 = (len_10 * 8) / t_cod_10 / 1000
bandwidth_30 = (len_30 * 8) / t_cod_30 / 1000

print(f"\n2.1 Полоса передачи G.711(10): {bandwidth_10:.1f} кбит/с")
print(f"2.2 Полоса передачи G.711(30): {bandwidth_30:.1f} кбит/с")

T_10_ms = (len_10 * 8) / (x1 * 1e6) * 1000
T_30_ms = (len_30 * 8) / (x1 * 1e6) * 1000

print(f"2.3 Tср (G.711(10)): {T_10_ms:.3f} мс")
print(f"2.4 Tср (G.711(30)): {T_30_ms:.3f} мс")

lambda_call = 1000 / 3600
t_talk = 180

lambda_pac_10 = lambda_call * (t_talk / t_cod_10)
lambda_pac_30 = lambda_call * (t_talk / t_cod_30)

mu_10 = (x1 * 1e6) / (len_10 * 8)
mu_30 = (x1 * 1e6) / (len_30 * 8)

rho_10 = lambda_pac_10 / mu_10
rho_30 = lambda_pac_30 / mu_30

print(f"\n2.5 ФРВ окончания обслуживания (M/M/1) на интерфейсе {x1} Мбит/с:")

t_ms = np.linspace(0, 10, 1000)
t_sec = t_ms / 1000

if rho_10 >= 1:
    print(f"   G.711(10): ρ = {rho_10:.3f} ≥ 1 → нестабильно")
else:
    print(f"   G.711(10): ρ = {rho_10:.3f}")
    F_10 = 1 - np.exp(-mu_10 * (1 - rho_10) * t_sec)
    plt.figure(1)
    plt.plot(t_ms, F_10, label=f'G.711(10), ρ={rho_10:.3f}')
    plt.title('ФРВ окончания обслуживания (G.711(10))')
    plt.xlabel('Время, мс')
    plt.ylabel('F(t)')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

if rho_30 >= 1:
    print(f"   G.711(30): ρ = {rho_30:.3f} ≥ 1 → нестабильно")
else:
    print(f"   G.711(30): ρ = {rho_30:.3f}")
    F_30 = 1 - np.exp(-mu_30 * (1 - rho_30) * t_sec)
    plt.figure(2)
    plt.plot(t_ms, F_30, label=f'G.711(30), ρ={rho_30:.3f}')
    plt.title('ФРВ окончания обслуживания (G.711(30))')
    plt.xlabel('Время, мс')
    plt.ylabel('F(t)')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

delta_t = 0.020
C = 10 * 1e6
L_bytes = x2_kb * 1000
L_bits = L_bytes * 8

T_sr_26 = L_bits / C
mu_26 = 1 / T_sr_26
rho_26 = lambda_26 * T_sr_26

print(f"\n--- Задание 2.6 ---")
print(f"Δt = {delta_t*1000:.0f} мс")
print(f"Lпакета = {L_bytes} байт ({x2_kb} КБ)")
print(f"Tср = {T_sr_26*1000:.3f} мс")
print(f"ρ = λ × Tср = {rho_26:.3f}")

if rho_26 >= 1:
    print("Система НЕСТАБИЛЬНА (ρ ≥ 1)")
else:
    print("Система стабильна")
    F_e = 1 - np.exp(-mu_26 * (1 - rho_26) * t_sec)
    plt.figure(3)
    plt.plot(t_ms, F_e, color='red', label=f'ρ={rho_26:.3f}')
    plt.title('ФРВ окончания обслуживания (2.6, M/M/1)')
    plt.xlabel('Время, мс')
    plt.ylabel('F_e(t)')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()
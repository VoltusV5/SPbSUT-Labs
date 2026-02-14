import itertools
from itertools import product
import itertools
import json
import math as mt
import pickle as pk
import time

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy import stats

from statsmodels.graphics.tsaplots import plot_acf, plot_pacf
from statsmodels.stats.diagnostic import acorr_ljungbox, het_arch, normal_ad
from statsmodels.tsa.arima.model import ARIMA
from statsmodels.tsa.stattools import adfuller


# x - значение переменной времени
# lmd - параметр распределения (частота событий)
def exp_rasp(x, lmd):
    return 1 - np.exp(-lmd*x)


# Функция для расчёта накопленного распределения
def RMST_sp(data):
    s = 0
    sp = []
    for i in data:
        s += i
        sp.append(s)
    return sp


# Параметры Гамма-распределения
A1 = 3
B1 = 0.02
# среднее значение гамма-распределения
A1_B1 = A1 * B1

# Загрузка данных
data = pd.read_csv("data.csv", encoding="cp1251")

# Удаление ненужных столбцов
column = data.columns.to_list()
column = [i for i in column if i != 'Time' and i != 'Length' and i != 'No.']
for i in range(4):
    del data[column[i]]


# Столб1 - левая граница каждого интервала: 0, 0.03, 0.06 …
# Столб2 - правая граница: 0.03, 0.06, 0.09 …
distance = 0.03
data["Столб1"] = [i*distance for i in range(0, len(data))]
x = 0
data["Столб2"] = [distance + i*distance for i in range(len(data))]


# Подсчёт количества значений времени, попадающих в каждый интервал
times = data['Time'].values
bins = np.append(data["Столб1"].values, data["Столб2"].values[-1])  # границы
# сколько событий попадает в каждый интервал:
counts, _ = np.histogram(times, bins=bins)

# Обрезаем таблицу, чтобы количество строк совпадало с количеством интервалов.
data = data.iloc[:len(counts)]
data["Количество"] = counts  # добавляем столбец с количеством


sr = data['Time'].mean()  # среднее время
dataH = data["Time"]  # копия временного ряда для Хёрста
#################
data = data.truncate(after=16)  # Обрезка таблицы (для упрощения вычислений)

count = data['Количество'].values
sm = sum(count)
P = [i/sm for i in count]  # вероятность попадания в интервал
# кол-во событий в интервале​ / общее кол-во событий
data["P_i"] = P

lmd = 1/sr
# Экспоненциальное распределение для каждого интервала
# вероятность того, что событие произойдёт именно в этом интервале времени
data["exp.rasp"] = exp_rasp(data['Столб2'], lmd) - \
    exp_rasp(data['Столб1'], lmd)

# Второе гамма-распределение
A2 = 11
B2 = 0.021

data["gamma1"] = stats.gamma.cdf(
    data['Столб2'], a=A1, scale=B1) - stats.gamma.cdf(data['Столб1'], a=A1, scale=B1)
data["gamma2"] = stats.gamma.cdf(
    data['Столб2'], a=A2, scale=B2) - stats.gamma.cdf(data['Столб1'], a=A2, scale=B2)

# Парето распределение
k = 1.2
a = 0.01

# Вероятность того, что событие произойдёт до правой границы интервала -
# - вероятность, что событие произойдёт до левой границы интервала.
data["pareto"] = (1 - (a/data['Столб2'])**k) - (1 - (a/(data['Столб1']))**k)
x1 = a
data['pareto'][0] = 1 - (a/data['Столб2'][0])**k


# Смешанное распределение
K1, K2, K3, K4 = 0.66, 0.015, 0.0011, 0.3239
# print(K1 + K2 + K3 + K4)  # должно быть 1

# Суммарное распределение для каждого интервала
data['sum'] = K1*data['pareto'] + K2*data["exp.rasp"] + \
    K3*data['gamma1'] + K4*data['gamma2']


# --------------------------------------------------------------


'''
RMSE - ошибка на уровне отдельных интервалов.
RMST - ошибка на уровне накопленных вероятностей (кумулятивная ошибка).
'''

# RMSE - оценка ошибки. Корень из среднего квадратичного отклонения
RMSE = sum((((data['P_i'] - data['sum'])**2)/20)**(1/2))
print("RMSE:", RMSE)

# RMST - накопленные вероятности
data["P_C"] = RMST_sp(data['P_i'].values)  # Обращаемся к функции RMST_sp
data["sum_c"] = RMST_sp(data['sum'].values)

# RMST показывает, насколько хорошо модель описывает
# кумулятивное распределение, а не отдельные интервалы
RMST = sum((((data["P_C"] - data['sum_c'])**2)/20))**(1/2)
print("RMST:", RMST)

'''
Параметр Хёрста
'''

dataH = pd.read_csv("logDifference.csv", index_col=0)  # [:1_048_540]

total_rows = len(dataH)
print(f"Всего строк в logDifference: {total_rows}")

# Хотим примерно 20 сегментов
num_segments = 20
segment_size = total_rows // num_segments   # целочисленное деление
print(
    f"Размер каждого сегмента: {segment_size} (сегментов будет ~{num_segments})")

# Разбиваем на 20 сегментов
sp20 = []
k = 0
for i in range(segment_size, total_rows, segment_size):
    sp20.append(dataH.iloc[k:i])
    k = i
print(sp20)

if k < total_rows:
    sp20.append(dataH.iloc[k:])

print(f"Реально создано сегментов: {len(sp20)}")

MO = []
V = []
for segment in sp20:
    if len(segment) > 1:  # защита от слишком маленького остатка
        MO.append(segment["log_dif"].mean())
        V.append(segment["log_dif"].var())

miniTable = pd.DataFrame({
    "N": list(range(1, len(MO) + 1)),
    "M.O": MO,
    "V": V
})

# Средние значения MO и V
srMO = miniTable["M.O"].mean()  # среднее МО
srV = miniTable["V"].mean() * 10  # средняя дисперсия
stdV = np.var(miniTable['V'])**(1/2)  # стандартное отклонение дисперсий
stdMO = np.var(miniTable['M.O'])**(1/2)  # стандартное отклонение МО

# относительное отклонение (%), показывает вариативность блоков
percentMO = stdMO/srMO * 100  # процент отклонения МО
percentV = stdV/srV * 1000  # процент отклонения V

print(f"срМО: {abs(srMO)} || срV: {srV}")
print(f"MO% {abs(percentMO)}%  || V%  {percentV}%")


# Разбиваем временной ряд на сегменты разной длины
sp = [[] for _ in range(6)]
segment_sizes = [4, 8, 16, 32, 64, 128]

for j, segment_size in enumerate(segment_sizes):
    for i in range(0, len(dataH), segment_size):
        segment = dataH[i:i + segment_size]
        sp[j].append(segment)

# Вычисляем дисперсии для каждого сегмента
disp = [[], [], [], [], [], []]
for i in range(len(sp)):
    for j in sp[i]:
        disp[i].append(np.var(j))

# Вычисляем дисперсию дисперсий для каждого размера сегмента
variance_of_variances = []
for i in range(len(disp)):
    var_of_vars = np.var(disp[i])
    variance_of_variances.append(var_of_vars)

# m - размер сегмента
# var - дисперсия дисперсий для этого размера
# log(m) и log(var) - логарифмы для линейной регрессии, чтобы оценить Хёрст.
experiment = pd.DataFrame({
    "m": segment_sizes,
    "var": variance_of_variances,
    "log(m)": np.log(segment_sizes),
    "log(var)": np.log(variance_of_variances)
})
print(experiment)


# График для оценки Хёрста (log(var) против log(m))

# Позволяет визуально оценить тренд, который используется
# для расчёта H (Хёрста) через наклон линии регрессии.
plt.figure(figsize=(10, 6))
plt.scatter(experiment['log(m)'], experiment['log(var)'],
            color='blue', s=50, label='Данные')

'''Линейная регрессия'''
# берём (log(var), log(m)) из таблицы experiment
# slope - наклон прямой (ключевая величина);
# intercept - свободный член;
# r_value² - качество аппроксимации
slope, intercept, r_value, p_value, std_err = stats.linregress(
    experiment['log(m)'], experiment['log(var)'])
trend_line = slope * experiment['log(m)'] + intercept

print(f"Herst:{1 - abs(slope)/2}")

# Трендовая линия
plt.plot(experiment['log(m)'], trend_line, color='red', linewidth=2,
         label=f'Тренд: y = {slope:.3f}x + {intercept:.3f}\nR² = {r_value**2:.4f}')
plt.legend()
plt.grid(True)
plt.show()

# Распределения и аппроксимации
plt.figure(figsize=(9, 5))
plt.bar(data['Столб2'], data['P_i'], alpha=0.7, width=0.005, label="P_i")
plt.plot(data['Столб2'], data['gamma1'], label="gamma1", color="green")
plt.plot(data['Столб2'], data['gamma2'], label="gamma2", color="red")
plt.plot(data['Столб2'], data['exp.rasp'], label="exp", color="yellow")
plt.plot(data['Столб2'], data['pareto'], label="pareto")
plt.legend()
plt.grid(True)
plt.show()

# суммарная модель распределения
plt.figure(figsize=(9, 5))
# Реальная
plt.bar(data['Столб2'], data['P_i'], alpha=0.7, width=0.005, label="P_i")
# аппроксимация эмпирического распределения
plt.plot(data['Столб2'], data['sum'])
plt.show()

# график математического ожидания и дисперсии
plt.figure(figsize=(9, 5))
plt.plot(miniTable["N"], miniTable['M.O'], label="MO")
plt.plot(miniTable['N'], miniTable['V'], label="V", color="red")  # Для хайпа
plt.legend()
plt.show()

'''RS-анализ для Хёрста'''

'''
R/S-анализ — это метод оценки наличия долговременной памяти во временном ряду, 
основанный на анализе того, как нормированный размах накопленных отклонений 
растёт с увеличением длины интервала наблюдений.
'''

# разные размеры блоков
rs_block_sizes = [500, 1000, 2000, 3000]

RS_mean = []
RS_std = []
RS_R = []
RS_RS = []

for block in rs_block_sizes:
    RS_values = []
    mean_values = []
    std_values = []
    R_values = []

    # идём по блокам
    for i in range(0, len(dataH), block):
        segment = dataH[i:i + block]
        if len(segment) < block:  # Отбрасываем последний кусок, если неполный
            continue

        # Подготовка данных
        X = np.array(segment, dtype=float)  # текущий сегмент
        mean_X = X.mean()  # математическое ожидание
        std_X = X.std()  # стандартное отклонение

        # центрирование (Каждое значение смещается относительно среднего)
        Y = X - mean_X
        # накопленное отклонение
        Z = np.cumsum(Y)
        # размах (амплитуда колебаний накопленного процесса)
        R = Z.max() - Z.min()

        # Нормирование R/S
        if std_X != 0:
            RS = R / std_X
        else:
            RS = np.nan

        # Сохранение значений для текущего сегмента
        RS_values.append(RS)
        mean_values.append(mean_X)
        std_values.append(std_X)
        R_values.append(R)

    # средние значения по всем сегментам блока
    avg_mean = np.mean(mean_values)
    avg_std = np.mean(std_values)
    avg_R = np.mean(R_values)
    avg_RS = np.mean(RS_values)

    # Cохранение результата
    RS_mean.append(avg_mean)
    RS_std.append(avg_std)
    RS_R.append(avg_R)
    RS_RS.append(avg_RS)

# таблица
RS_detailed = pd.DataFrame({
    "block": rs_block_sizes,  # размер блока
    "mean": RS_mean,  # среднее МО
    "std": RS_std,  # среднее отклонение
    "R": RS_R,  # средний размах
    "R/S": RS_RS  # главная величина RS-анализа
})

print("\nПодробная таблица RS-анализа:")
print(RS_detailed)


# логарифмическая регрессия (Таблица для вычисления параметра Хёрста)
RS_table = pd.DataFrame({
    "block": rs_block_sizes,  # размер блока
    "RS": RS_RS,  # R/S
    "log(block)": np.log(rs_block_sizes),  # логарифм размера блока
    "log(RS)": np.log(RS_RS)  # логарифм R/S
})

# slope_RS — наклон прямой
# intercept_RS — сдвиг
# r_value_RS² — качество аппроксимации

slope_RS, intercept_RS, r_value_RS, _, _ = stats.linregress(
    RS_table["log(block)"], RS_table["log(RS)"])

H_RS = slope_RS  # параметр Хёрста равен наклону прямой

print("\nТаблица для Хёрста (RS):")
print(RS_table)
print(f"\nH (RS-анализ) = {H_RS}")

# график RS
plt.figure(figsize=(8, 5))
plt.scatter(RS_table["log(block)"], RS_table["log(RS)"], s=60, label="Данные")

# линия тренда
trend_line_RS = slope_RS * RS_table["log(block)"] + intercept_RS
plt.plot(
    RS_table["log(block)"],
    trend_line_RS,
    linewidth=2,
    label=f"Тренд: y = {slope_RS:.3f}x + {intercept_RS:.3f}\nR² = {r_value_RS**2:.4f}"
)

plt.grid(True)
plt.xlabel("log(block)")
plt.ylabel("log(R/S)")
plt.title("RS-анализ")
plt.legend()
plt.show()

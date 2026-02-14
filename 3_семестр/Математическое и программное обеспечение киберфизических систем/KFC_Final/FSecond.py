import math as mt
import time
import json
import pickle as pk
import itertools

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

from statsmodels.tsa.stattools import adfuller
from statsmodels.graphics.tsaplots import plot_acf, plot_pacf
from statsmodels.stats.diagnostic import acorr_ljungbox, het_arch, normal_ad
from statsmodels.tsa.arima.model import ARIMA


d1 = pd.read_csv("data_1.csv", delimiter=',', encoding="cp1251")
d1['Time'] = pd.to_datetime(d1['Time'])
d1 = d1.set_index('Time')

del d1["Source"]
del d1["Destination"]
del d1["Protocol"]
del d1["Info"]
del d1["No."]

d2 = d1.resample('50s').mean()  # агрегация
print(len(d2))

d2.interpolate(inplace=True)  # если NaN

# Полный график агрегированного трафика (как на рисунке 1 в методичке)
plt.figure(figsize=(14, 7))  # большой размер для отчёта
plt.plot(d2.index, d2['Length'], color='blue', linewidth=1)
plt.title('Динамика среднего размера пакетов IoT-трафика (агрегация по 50 секундам)')
plt.xlabel('Время')
plt.ylabel('Средняя длина пакета (байты)')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()

d3 = d2
# print(adfuller(d3))
# Можно проверить стационарность
# временного ряда с помощью теста Дики-Фуллера
k = 0

# Вывод всех окошек для отбора вручную самых интересных
# for i in range(120, len(d2["Length"]), 120):
#     print(k, i)
#     d3 = d2[k:i]
#     plt.figure(figsize=(12, 5))
#     plt.plot(d3.index, d3['Length'])
#     plt.title("Length")
#     plt.xlabel("Time")
#     plt.ylabel("Packet Length")
#     plt.grid(True)
#     plt.show()
#     print(len(d2['Length'])*0.07)
#     k = i

# print("====================================")

# Список окошек
sp = [(0, 120), (120, 240), (240, 360), (360, 480), (480, 600),
      (600, 720), (720, 840), (840, 960), (960, 1080), (1080, 1200)]
'''aicDict = {f"{i} {j}": [] for i, j in sp}

for x, y in sp:
    # plot_acf(d2.Length[x:y]) #– график АКФ
    # plot_acf(d2.Length[x:y].diff().dropna())
    # plt.show()
    # plot_pacf(d2.Length[x:y]) #– график ЧАКФ
    # plot_pacf(d2.Length[x:y].diff().dropna())
    # plt.show()

    # Проверка на стационарность тест Дики-Фуллера
    # Берём разности .diff() временного ряда, чтобы убрать тренд
    # Если p-value < 0.05 → ряд стационарен, можно строить ARIMA.
    # Иначе сегмент пропускается (continue).
    if adfuller(d2.Length[x:y].diff().dropna())[1] < 0.05:
        print("p:", adfuller(d2.Length[x:y].diff().dropna())[1])
        # plot_acf(d2.Length[x:y].diff().dropna())
        # plot_pacf(d2.Length[x:y].diff().dropna())
        # plt.show()
    else:
        print("Не стационарен")
        continue

    # Подбор параметров p,d,q для ARIMA
    p = range(0, 5)  # порядок авторегрессии (AR)
    d = range(0, 3)  # порядок интегрирования (разностей)
    q = range(0, 5)  # порядок скользящей средней (MA)
    pdq = list(itertools.product(p, d, q))  # все комбинации p,d,q
    best_pdq = (0, 0, 0)
    best_aic = np.inf
    for params in pdq:
        # Создание и обучение модели ARIMA
        model_test = ARIMA(d2.iloc[x:y], order=params)
        try:
            result_test = model_test.fit()  # обучение модели
        except:
            continue  # Если при обучении модель падает
        if result_test.aic < best_aic:  # сравнение AIC
            best_pdq = params
            best_aic = result_test.aic
            # print(best_pdq, best_aic)
            aicDict[f"{x} {y}"].append((best_pdq, best_aic))

'''
# ВОТ ЭТО МОЖНО РАСКОМЕНТИТЬ ОДИН РАЗ ЗАПУСТИТЬ
# НАДОЛГО ОНО СОХРАНИТЬСЯ И ВЕСЬ ЦИКЛ СВЕРХУ БОЛЬШЕ НЕ НУЖЕН
# with open("data.json", "w", encoding="UTF-8") as file:
#     json.dump(aicDict, file, indent=4)

print("!====================================!")

# ДЛЯ ЧТЕНИЯ AICDICT чтобы много раз не запускать алгоритм отбора
# где куча циклов наверху
with open("data.json", "r", encoding="UTF-8") as file:
    aicDict = json.load(file)

# print(aicDict)

# Список: ARIMA модель, параметры, сегмент, обученная модель
'''spArima = []
for i in aicDict:
    param = min(aicDict[i], key=lambda x: x[1])[0]  # параметры с мин AIC
    pmodel = ARIMA(d2.iloc[int(i.split()[0]):int(
        i.split()[1])], order=(param[0], param[1], param[2]))  # модель

    stats_mdl = pmodel.fit()  # обучение модели
    # print(int(i.split()[0]),int(i.split()[1]))
    # print(stats_mdl.summary()) # ВЫВОД В консоль таблиц

    # Для записи в файл таблиц!
    # with open("TABELS.txt", "a", encoding="UTF-8") as file:
    #     file.write('\n' + str(i))
    #     file.write(str(stats_mdl.summary()))
    #     file.write("\n")
    #     file.write("+"*100)

    # В spArima сохраняем:
    # объект модели ARIMA (pmodel)
    # её параметры (param)
    # сегмент (i)
    # обученную модель (stats_mdl)
    # Это позволяет потом:
    # делать прогнозы
    # строить графики
    # сохранять результаты в файл
    spArima.append((pmodel, param, i, stats_mdl))
    print("====================================")

print(spArima)

with open("data.pkl", "wb") as file:
    pk.dump(spArima, file)
'''
with open("data.pkl", "rb") as file:
    spArima = pk.load(file)

spisok = ["360 480"]
# Создаем общий график
plt.figure(figsize=(12, 6))

# Создаем словарь цветов для разных участков
colors = ['red', 'blue', 'green']
color_dict = {spisok[i]: colors[i] for i in range(len(spisok))}


mape = []
ser = []
ARIMAs = []
ARIMEmodels = []
# Для каждой модели в spArima
for model, param, indx, fitted_model in spArima:
    if indx in spisok:
        # Данные для прогноза
        data = d2.iloc[int(indx.split()[0]):int(indx.split()[1])]

        # Прогноз на 50 точек
        forecast = fitted_model.predict(start=0, end=len(data["Length"]))

        # График
        plt.figure(figsize=(12, 6))
        x_point = list(range(0, len(data["Length"])))[::5]
        print(x_point)
        print(forecast[::5])
        # Прогноз ARIMA с шагом 5
        plt.plot(x_point, data["Length"][::5],
                 linewidth=2, markersize=8, label=f'ARIMA{param} (шаг={5})')
        perem = forecast[::5]
        plt.plot(x_point, perem[:-1])
        # # Настройки
        # plt.title(f'ARIMA прогноз (каждая {step}-я точка)\nУчасток {indx}')
        # plt.xlabel('Индекс')
        # plt.ylabel('Значение')
        # plt.legend()
        # plt.grid(True, alpha=0.3)
        plt.show()

        forecast = np.array(forecast[::5])[:-1]
        data = np.array(data["Length"][::5])
        ARIMAs.append(f"ARIMA{param}")
        ARIMEmodels.append(fitted_model)
        # MAPE
        # средняя абсолютная ошибка в процентах:
        # показывает, насколько прогноз в среднем отклоняется от реальных данных.
        mape.append(100*np.mean(np.abs((data - forecast)/data)))

        # SER
        # отношение мощности сигнала к мощности ошибки
        # (чем выше, тем лучше прогноз)
        ser.append(10 * np.log10(np.sum(data**2) /
                   np.sum((data - forecast)**2)))
        break

print(mape)
# ГРАФИКИ MAPE и SER
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 6))
ax1.bar(ARIMAs, mape, width=0.1)
ax1.set_title("MAPE")
ax2.bar(ARIMAs, ser, width=0.1)
ax2.set_title("SER")
plt.show()

print(ARIMEmodels)
ARIMA_beautiful = ARIMEmodels[0]

# ВЫВОД формулы


def arima_equation(res):
    """
    Функция которая выводит модель Аримы в виде формулы
    """
    orders = res.model_orders

    # берем p, d, q
    p = orders.get("ar", 0)
    d = orders.get("diff", orders.get("integrated", 0))
    q = orders.get("ma", 0)

    ar = res.arparams
    ma = res.maparams

    parts = []

    # AR terms --- как прошлые разности влияют на текущую
    for i in range(p):
        parts.append(f"{ar[i]:.4f}·Δ^{d}Y_(t-{i+1})")  # Δ^d Y_(t-i)

    # noise term --- шум
    parts.append("ε_t")

    # MA terms --- как прошлые ошибки влияют на текущую
    for i in range(q):
        sign = "+" if ma[i] >= 0 else ""
        parts.append(f"{sign}{ma[i]:.4f}·ε_(t-{i+1})")  # ε_(t-i)

    return f"Δ^{d} Y_t = " + " + ".join(parts)  # формула


print(arima_equation(ARIMA_beautiful))  # вывод формулы


# ОСТАТКИ
def residual_diagnostics(res, lags=10):
    """
    Функция которая считает табличку статистики. 
    lags - это лаги, они нужны чтобы точнее определить автокорреляцию
    """
    resid = res.resid  # остатки модели

    # Ljung–Box тест (автокорреляция)
    # Получаем DataFrame
    lb = acorr_ljungbox(resid, lags=[lags], return_df=True)
    lb_stat = lb['lb_stat'].iloc[0]  # статистика
    # p-значение если < 0.05 → остатки коррелированы
    lb_p = lb['lb_pvalue'].iloc[0]

    # Jarque–Bera (нормальность)
    jb_stat, jb_p = normal_ad(resid)  # более точный Anderson-Darling тест

    # ARCH-LM (гетероскедастичность)
    # проверяет, зависит ли дисперсия остатков от их прошлых значений
    # Если p-значение < 0.05 → гетероскедастичность присутствует
    arch_stat, arch_p, _, _ = het_arch(resid)

    # AIC модели
    # чем меньше AIC, тем лучше модель (Критерий Акаике.)
    aic = res.aic

    # Формируем таблицу
    df = pd.DataFrame({
        "Метрика": [
            "Тест Льюинга–Бокса",
            "Тест Харке–Бера",
            "ARCH-LM тест остатков",
            "AIC"
        ],
        "Статистика": [
            round(lb_stat, 4),
            round(jb_stat, 4),
            round(arch_stat, 4),
            round(aic, 4)
        ],
        "p-значение": [
            round(lb_p, 4),
            round(jb_p, 4),
            round(arch_p, 4),
            None
        ]
    })

    return df

# Histogram + KDE — распределение остатков.
# QQ-plot — проверка нормальности.
# Standardized residuals — остатки по времени.
# ACF of residuals — автокорреляция остатков.


df = residual_diagnostics(ARIMA_beautiful, lags=1)
print(df)


# ГРАФИКИ

ARIMA_beautiful.plot_diagnostics(figsize=(15, 10))
plt.show()

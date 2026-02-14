<div align="center">

# Федеральное агентство связи

**ФЕДЕРАЛЬНОЕ ГОСУДАРСТВЕННОЕ БЮДЖЕТНОЕ  
ОБРАЗОВАТЕЛЬНОЕ УЧРЕЖДЕНИЕ ВЫСШЕГО ОБРАЗОВАНИЯ**

**«САНКТ-ПЕТЕРБУРГСКИЙ ГОСУДАРСТВЕННЫЙ УНИВЕРСИТЕТ  
ТЕЛЕКОММУНИКАЦИЙ ИМ. ПРОФ. М. А. БОНЧ-БРУЕВИЧА» (СПбГУТ)**

</div>

<div align="center">
Факультет информационных технологий и программной инженерии  
Кафедра: Программная инженерия. Разработка программного обеспечения и приложений искусственного интеллекта в киберфизических системах

---

## Практическая работа

по дисциплине **«Математическое и программное обеспечение киберфизических систем»**

## **Тема:** **«Прогнозирование трафика Интернета вещей с помощью параметрических моделей ARIMA»**

</div>

<div align="right"><br>
Выполнил: студент 2-го курса группы ИКПИ-42  

Гарькуша Никита Антонович

**Преподаватель:**
к.т.н., доцент кафедры СС и ПД
Гребенщикова А. А.

</div>
<br><br>
<div align="center">
Санкт-Петербург  
2025  
</div>

<div style="page-break-before: always;"></div>

---

## 1. Исследование трафика Интернета вещей в Wireshark

Анализ трафика проводился на основе реальных дампов трафика, в котором присутствует нагрузка интернета вещей (Internet of Things, IoT). 

Таким образом, перед анализом сетевого трафика было необходимо отфильтровать данные в формате pcap по принципу отбрасывания пакетов, физический адрес которых не относился к устройствам типа IoT. Фильтрация нужного набора пакетов проводилась в программе анализа сетевого трафика Wireshark. Для примера на рисунке 1 представлено распределение трафика.

![alt text](image.png)
**Рисунок 1 – Распределение трафика типа интернета вещей**

Трафик носит периодический и стабильный характер с относительно невысокой средней нагрузкой (в основном ~50–100 пакетов/с).

**Формат времени целенаправленно был переведен в “время между пакетами”.**

Команда для фильтрации интернет вещей.
```
eth.addr == 70:ee:50:18:34:43 or eth.addr == d0:52:a8:00:67:5e or
eth.addr == 44:65:0d:56:cc:d3 or eth.addr == f4:f2:6d:93:51:f1 or
eth.addr == 00:16:6c:ab:6b:88 or eth.addr == 30:8c:fb:2f:e4:b2 or
eth.addr == 00:62:6e:51:27:2e or eth.addr == e8:ab:fa:19:de:4f or
eth.addr == 00:24:e4:11:18:a8 or eth.addr == ec:1a:59:79:f4:89 or
eth.addr == 50:c7:bf:00:56:39 or eth.addr == 74:c6:3b:29:d7:1d or
eth.addr == ec:1a:59:83:28:11 or eth.addr == 18:b4:30:25:be:e4 or
eth.addr == 70:ee:50:03:b8:ac or eth.addr == 00:24:e4:1b:6f:96 or
eth.addr == 74:6a:89:00:2e:25 or eth.addr == 00:24:e4:20:28:c6 or
eth.addr == d0:73:d5:01:83:08 or eth.addr == 18:b7:9e:02:20:44 or
eth.addr == e0:76:d0:33:bb:85 or eth.addr == 70:5a:0f:e4:9b:c0 or
eth.addr == 08:21:ef:3b:fc:e3 or eth.addr == 30:8c:fb:b6:ea:45 or
eth.addr == 14:cc:20:51:33:ea

```

## 2. Исследование трафика Интернета вещей и апроксимация

При начальном анализе дампов трафика были получены графики плотностей распределения интервалов между пакетами (рисунок 2)

![alt text](image-1.png)
**Рисунок 2 – Плотность распределения времени между пакетами трафика интернета вещей**

Как видно на рисунке 2, полученное распределение было
аппроксимировано и на основе этой аппроксимации была выведена
математическая функция вида

$$
f(t) = 0.05 f_{pareto}(t) + 0.95 f_{gamma1}(t), \quad (1)
$$


![alt text](image-2.png)
**Рисунок 3 – все плотности распределения вместе**

Экспоненциальное и ряд других стандартных распределений не были использованы в итоговой модели, поскольку они не обеспечивали требуемой точности аппроксимации экспериментальных данных. В частности, экспоненциальное распределение не отражало форму эмпирического распределения интервалов между пакетами и приводило к увеличению ошибок RMSE и RMST. Взвешенная комбинация гамма-распределения и распределения Парето показала наилучшее соответствие экспериментальным данным и была выбрана в качестве итоговой аппроксимирующей модели.
  

**Оценка аппроксимации:**  
Оценка производилась по RMSE для PDF и CDF.  
Формула RMSE: \[
\mathrm{RMSE} = \sqrt{
\frac{1}{n}
\sum_{i=1}^{n}
\left( y_i - \hat{y}_i \right)^2
}
\] 
Формула RMST: \[
\mathrm{RMST} =
\sqrt{
\frac{1}{n}
\sum_{i=1}^{n}
\left(
F_{\text{emp}}(x_i) - F_{\text{model}}(x_i)
\right)^2
}
\] 
Отличия: RMSE оценивает плотность, RMST – кумулятивную функцию.  
- RMSE: 0.016
- RMST: 0.006

**Выводы:**
Полученные значения RMSE = 0.016 и RMST = 0.006 свидетельствуют о хорошем соответствии аппроксимирующей модели экспериментальным данным. Низкое значение RMSE указывает на малое расхождение между эмпирической и модельной плотностями распределения, а меньшее значение RMST подтверждает высокую точность аппроксимации кумулятивной функции распределения. В целом, аппроксимация интервалов между пакетами трафика интернета вещей взвешенной суммой гамма-распределения и распределения Парето является адекватной и корректно описывает наблюдаемый трафик.

## 3. Самоподобие

Вследствие наличия во временном ряде сильной зависимости значений от предыдущих возникает такое понятие как самоподобие. Таким образом, Интернет-трафик при сглаживании имеет определенную структуру с трендом, на которую стохастически влияют редкие «всплески» пакетов. Такие особенности трафика имеют особое значение и влияние на математические моменты временной последовательности как в локальных масштабах времени, так и на больших размахах.

В традиционных моделях сетевого трафика практически отсутствует свойство долговременной зависимости. Причиной такого явления выступает ярко выраженное сглаживание пульсаций. Именно с приходом концепции единой мультисервисной сети процесс генерации трафика стоит рассматривать с точки зрения степени самоподобия трафика и его долговременной зависимости. Таким образом, теория пуассоновских процессов уходит на второй план, уступая самоподобному процессу, для которого характерно сохранение некоторых статистических особенностей при масштабировании времени.


**Описание методов:**

- Дисперсионный анализ: Для проверки стационарности временной ряд был разбит на 20 блоков, для каждого из которых вычислялись среднее значение и дисперсия. Анализ относительных отклонений средних и дисперсий показал их устойчивость по блокам, что позволяет считать ряд стационарным и использовать его для дальнейшей оценки параметра Хёрста.
- R/S анализ: Для оценки параметра Хёрста использовался R/S-анализ, при котором временной ряд разбивался на участки различной длины. Для каждого участка рассчитывалось отношение размаха накопленных отклонений к среднеквадратичному отклонению.

![alt text](image-3.png)
**Рисунок 4 – График дисперсионного анализа**

![alt text](image-4.png)
**Рисунок 5 – График R/S анализа**

- Параметр Херста по дисперсионному анализу: H=0.58. Как найти: $H = 1 - \frac{k}{2}$. Прямая y = kx + b
- Параметр Херста по R/S анализу: H = 0.43. Как найти: H = k.



**Оценка стационарности:**

срМО = 0.838090681609874% || срV = 11.934021227656151%
MO= 11.288334808011225%  || V =12.679851517402751%
Вывод: ряд стационарный т.к MO<10% и V<40%

**Вывод**
Полученные значения параметра Хёрста, рассчитанные дисперсионным методом ($H_{дисп}=0.59$) и методом R/S-анализа ($H_{RS}=0.44$), указывают на отсутствие выраженной долговременной зависимости во временном ряду.

## 4. Приведение ряда в эквидистантный вид и поиск портфеля моделей

**Обоснование:**  
Так как $H_{дисп} >0.5$, а $H_{rs}<0.5$, то при $\Delta t = 100мс$ , $\Delta t_{агрегации} = 1с$ , у нас $\Delta t = 800мс$, а значит окно будет равно 7 процентам от всех данных.



![alt text](image-5.png)
**Рисунок 6 – Распределение трафика на временных интервалах с периодом агрегации 8 секунд**

Пример статистики модели ARIMA:
![alt text](image-6.png)
**Рисунок 7 – Результаты по модели ARIMA(3,0,3)**


- Портфель моделей: ARIMA(3, 0, 3) , в ходе анализа была выбрана одна модель ARIMA, так как у всех остальных вероятность превышала 0.05 хотя бы в одной строке.

## 5. Оценка прогноза

![alt text](image-7.png)
**Рис. 8. Прогноз значений трафика**

Для оценки точности прогноза используется ряд стандартных показателей.

Средняя абсолютная процентная ошибка (MAPE):

$$
MAPE = \frac{100\%}{L} \sum_{t=1}^{L} \left| \frac{X_t - \hat{X}_t}{X_t} \right|, \quad (5)
$$

где $X_t$ – реальное значение, $\hat{X}_t$ - прогнозное значение, $L$ – интервал прогноза. Если $MAPE<10\%$, то прогноз сделан с высокой точностью, $10\%<MAPE<20\%$ - прогноз хороший, $20\%<MAPE<50\%$ - прогноз удовлетворительный, $MAPE>50\%$ - прогноз плохой.

Отношение сигнала к шуму (SER):

$$
SER = 10 \lg \left( \frac{\sum_{t=1}^{L} X_t^2}{\sum_{t=1}^{L} (X_t - \hat{X}_t)^2} \right). \quad (6)
$$

![alt text](image-8.png)
**Рис. 9. MAPE и SER для ARIMA**  

## 6. Оценка остатков ARIMA

Модели ARIMA (p,d,q) представляют особый класс нестационарных моделей, которые считаются однородными и находятся в статистическом равновесии. Модель ARIMA определяется уравнением (2) и такая модель является обобщённой, т.к. включает в себя в качестве частных случаев следующие модели: авторегрессионные, скользящего среднего, смешанные модели авторегрессии-скользящего среднего и интеграция всех трёх соответственно.

$$
\Delta^d Y_t = \phi_1 \Delta^d Y_{t-1} + \dots + \phi_p \Delta^d Y_{t-p} + \varepsilon_t + \theta_1 \varepsilon_{t-1} + \dots + \theta_q \varepsilon_{t-q} \quad (2)
$$

Эффективная итеративная процедура построения моделей для описания зависимости наблюдаемых временных рядов состоит из трёх этапов:

1. Идентификация. Подразумевает использование уже имеющихся данных о временном ряде, чтобы подобрать соответствующий и оптимальный класс моделей для последующей оценки.
2. Оценка. Подразумевает выявление сопутствующих параметров для построения модели.
3. Диагностика. Подразумевает проверку модели на соответствие имеющимся данным, чтобы выявить несоответствия и определить оптимальные пути улучшения модели.

Для подтверждения адекватности полученной модели необходимо убедиться, что ряд остатков представляет собой случайную компоненту. Таким образом, чтобы оптимально провести анализ остатков сформированной модели используется Q-тест Льюнг–Бокса для проверки гипотезы на наличие автокорреляции в данных. При H0 (нулевая гипотеза об отсутствии корреляции) статистика Q-теста асимптотически имеет распределение Хи-квадрат. Значения p выше 0,05 указывают на принятие нулевой гипотезы о точности модели при уровне значимости 95%.

Наиболее известный тест для проверки гипотезы о нормальном распределении остатков – статистический тест Харке-Бера (Jarque-Bera test). Также необходимо убедиться в наличии условной гетероскедастичности с помощью теста множителей Лагранжа (ARCH LM-тест).


$\Delta^0 Y_t = 0.5438 \Delta^0 Y_{t-1} -0.8346·Δ^0Y_{t-2} + 0.7509·Δ^0Y_{t-3} + \varepsilon_{t} - 0.6811\varepsilon_{t-1} + 0.7721·ε_{t-2} -0.8722·ε_{t-3}$



**Таблица – Оценка остатков модели ARIMA(3,0,3)**

|                       | Статистика           | p-значение |
| --------------------- | -------------------- | ---------- |
| Тест Льюинга-Бокса    |  0.9085              | 0.3405     |
| Тест Харке-Бера       | 23.8983              | 0          |
| ARCH-LM тест остатков | 19.2257              | 0.0375     |
| AIC                   |              7461.2055            |


![alt text](image-9.png)
**Рис. 10. Графики диагностики**


**Вывод**
- Ряд стандартизированных остатков (Standardized residual for "L"): Остатки колеблются вокруг нуля без видимого тренда или систематических отклонений, что указывает на стационарность и хорошее захватывание линейной динамики моделью.
- Ряд стандартизированных остатков (Standardized residual for "L"):
Остатки колеблются вокруг нуля без тренда и систематических отклонений — хорошая стационарность и захват линейной динамики.
- Коррелограмма (Correlogram):
ACF близка к нулю на всех лагах (в пределах доверительного интервала). Нет значимой автокорреляции (тест Льюнга-Бокса: p = 0.3405 > 0.05).
- Гистограмма с оценкой плотности (Histogram plus estimated density): Гистограмма и KDE сильно отклоняются от N(0,1) — асимметрия и/или тяжёлые хвосты. Подтверждает ненормальность (тест Харке-Бера: p ≈ 0).
- Нормальный Q-Q сюжет (Normal Q-Q):
Точки сильно отклоняются от прямой, особенно в хвостах — визуальное подтверждение ненормальности остатков.

Модель хорошо справляется с линейными зависимостями и автокорреляцией — ошибки случайны в этом смысле. Но ошибки не распределены нормально и их дисперсия меняется со временем.Нормальность ошибок не критична для самих прогнозов, но важна, если нужны интервалы доверия или статистические тесты. В целом, модель подходит для базового прогнозирования, но она не идеальна.
## Приложение: Код на Python

**first.py**
```python
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import math as mt
import json
from scipy import stats
from statsmodels.tsa.stattools import adfuller
from statsmodels.graphics.tsaplots import plot_acf
from statsmodels.graphics.tsaplots import plot_pacf
import itertools
from openpyxl import Workbook
from openpyxl.styles import PatternFill, Font, Alignment, Border, Side
from statsmodels.stats.diagnostic import acorr_ljungbox, het_arch, normal_ad
from statsmodels.tsa.arima.model import ARIMA

def exp_rasp(x, lmd):
    return 1 - np.exp(-lmd*x)

def RMST_sp(data):
    s = 0
    sp = []
    for i in data:
        s+=i
        sp.append(s)
    return sp
## ПАРАМЕТРЫ
A1=14
B1=0.008
A1_B1 = A1*B1


data = pd.read_csv("практика 1.csv")

column = data.columns.to_list()
column = [i for i in column if i!='Time' and i!= 'Length' and i!= 'No.']
for i in range(4):
    del data[column[i]]

distance = 0.01
data["Столб1"] = [i*distance for i in range(0, len(data))]
x = 0
data["Столб2"] = [distance + i*distance for i in range(len(data))]



# Создаем матрицу сравнений
times = data['Time'].values
col1 = data['Столб1'].values
col2 = data['Столб2'].values


times = data['Time'].values
bins = np.append(data["Столб1"].values, data["Столб2"].values[-1])  # границы
counts, _ = np.histogram(times, bins=bins)

data = data.iloc[:len(counts)]  # чтобы совпадало
data["Количество"] = counts


data['Количество'] = counts

sr = data['Time'].mean()
dataH = data["Time"]
data = data.truncate(after=20) # Обрезка таблицы

count = data['Количество'].values
sm = sum(count)
P = [i/sm for i in count]
data["P_i"] = P

lmd = 1/sr
# data["exp.rasp"] = exp_rasp(data['Столб2'], lmd) - exp_rasp(data['Столб1'], lmd)




data["gamma1"] = stats.gamma.cdf(data['Столб2'], a=A1, scale=B1) - stats.gamma.cdf(data['Столб1'], a=A1, scale=B1)
k = 1.1
a = 0.001
paretosr = (a*k)/(k)

data["pareto"] = (1 - (a/data['Столб2'])**k) - (1 - (a/(data['Столб1']))**k)
x1 = a
data['pareto'][0] = 1 - (a/data['Столб2'][0])**k 

#print(data)


# Сумма распределений 
# Кэффы для каждого распределения из 100%

K1, K3 = 0.95, 0.05
data['sum'] = K3*data['gamma1'] + K1*data['pareto']

RMSE = sum((((data['P_i'] - data['sum'])**2)/20)**(1/2))
print("RMSE:", RMSE)
data["P_C"] = RMST_sp(data['P_i'].values)
data["sum_c"] = RMST_sp(data['sum'].values)

RMST = sum((((data["P_C"] - data['sum_c'])**2)/20))**(1/2)
print("RMST:", RMST)

##Параметр Хёрста 
## 1) Дисперсионный анализ
## Разбить на 20 блоков, найти для каждого среднее
## Получить табличку по блокам и по средним в каждом и дисперсии и получится новая таблица
## Среднее для каждого столба и стан отклонение. Стан откл/среднее

dataH = dataH[:103080]
sp20 = []
k=0
for i in range(5153, len(dataH), 5154):
    sp20.append(dataH[k:i])
    k = i

miniTable = 0
MO = []
V = []
for i in sp20:
    MO.append(i.mean())
    V.append(i.var())


miniTable = pd.DataFrame({"N" : [i for i in range(1, 21)], "M.O": MO, "V":V})
srMO = miniTable["M.O"].mean()
srV = miniTable["V"].mean()
stdV = np.var(miniTable['V'])**(1/2)
stdMO = np.var(miniTable['M.O'])**(1/2)
percentMO = stdMO/srMO *100
percentV = stdV/srV *100

print(miniTable)
print(f"срМО: {srMO} || срV: {srV}")
print(f"MO% {percentMO}%  || V%  {percentV}%")
print("Вывод: ряд стационарный т.к MO%<10 и V%<40")

#Табличка в 1 4 во 2 8 потом 16 32 64,  128, потом средняя дисперсия , логарифм от m, логарифм от v
sp = [[] for _ in range(6)]
segment_sizes = [4, 8, 16, 32, 64, 128]

for j, segment_size in enumerate(segment_sizes):
    for i in range(0, len(dataH), segment_size):
        segment = dataH[i:i + segment_size]
        sp[j].append(segment)

disp = [[], [], [], [], [], []]
for i in range(len(sp)):
    for j in sp[i]:
        disp[i].append(np.var(j))

variance_of_variances = []
for i in range(len(disp)):
    # Берем все дисперсии для данного размера сегмента и считаем дисперсию от них
    var_of_vars = np.var(disp[i])
    variance_of_variances.append(var_of_vars)

experiment = pd.DataFrame({"m":segment_sizes, "var": variance_of_variances, "log(m)": np.log(segment_sizes), "log(var)": np.log(variance_of_variances)})
print(experiment)

plt.figure(figsize=(10, 6))
plt.scatter(experiment['log(m)'], experiment['log(var)'], color='blue', s=50, label='Данные')

# Линейная регрессия
slope, intercept, r_value, p_value, std_err = stats.linregress(experiment['log(m)'], experiment['log(var)'])
trend_line = slope * experiment['log(m)'] + intercept

print(f"Herst:{1 - abs(slope)/2}")


# ## RS анализ, он более точнее. 





# Трендовая линия
plt.plot(experiment['log(m)'], trend_line, color='red', linewidth=2, 
         label=f'Тренд: y = {slope:.3f}x + {intercept:.3f}\nR² = {r_value**2:.4f}')
plt.legend()
plt.grid(True)
plt.show()

plt.figure(figsize=(9,5))
plt.bar(data['Столб2'],data['P_i'], alpha=0.7, width=0.005, label="P_i")
plt.plot(data['Столб2'], data['gamma1'], label="gamma1", color="green")
plt.plot(data['Столб2'], data['pareto'], label="pareto")
plt.legend()
plt.grid(True)
plt.show()

plt.figure(figsize=(9,5))
plt.bar(data['Столб2'],data['P_i'], alpha=0.7, width=0.005, label="P_i")
plt.plot(data['Столб2'], data['sum'])
plt.xlabel("Время")
plt.ylabel("Плотность распределения")
plt.grid(True)
plt.show()

# plt.figure(figsize=(9,5))
# plt.plot(miniTable["N"], miniTable['M.O'])
# plt.plot(miniTable['N'], miniTable['V'])
# plt.show()



rs_block_sizes = [500, 1000, 2000, 3000]

RS_avg = []
RS_mean = []
RS_std = []
RS_R = []
RS_RS = []

table_rows = []

for block in rs_block_sizes:
    RS_values = []
    mean_values = []
    std_values = []
    R_values = []

    # деление на сегменты
    for i in range(0, len(dataH), block):
        segment = dataH[i:i + block]
        if len(segment) < block:
            continue
        
        X = np.array(segment, dtype=float)
        mean_X = X.mean()
        std_X = X.std()
        
        Y = X - mean_X              # центрирование
        Z = np.cumsum(Y)            # накопленное отклонение
        R = Z.max() - Z.min()       # размах
        
        if std_X != 0:
            RS = R / std_X
        else:
            RS = np.nan
        
        RS_values.append(RS)
        mean_values.append(mean_X)
        std_values.append(std_X)
        R_values.append(R)

    # средние значения по всем сегментам блока
    avg_mean = np.mean(mean_values)
    avg_std = np.mean(std_values)
    avg_R = np.mean(R_values)
    avg_RS = np.mean(RS_values)

    RS_mean.append(avg_mean)
    RS_std.append(avg_std)
    RS_R.append(avg_R)
    RS_RS.append(avg_RS)

# таблица
RS_detailed = pd.DataFrame({
    "block": rs_block_sizes,
    "mean": RS_mean,
    "std": RS_std,
    "R": RS_R,
    "R/S": RS_RS
})

print("\nПодробная таблица RS-анализа:")
print(RS_detailed)


# логарифмическая регрессия
RS_table = pd.DataFrame({
    "block": rs_block_sizes,
    "RS": RS_RS,
    "log(block)": np.log(rs_block_sizes),
    "log(RS)": np.log(RS_RS)
})

slope_RS, intercept_RS, r_value_RS, _, _ = stats.linregress(RS_table["log(block)"], RS_table["log(RS)"])
H_RS = slope_RS

print("\nТаблица для Хёрста (RS):")
print(RS_table)
print(f"\nH (RS-анализ) = {H_RS}")

# график RS
plt.figure(figsize=(8,5))
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

```

**second.py**
```python
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import math as mt
import json
from scipy import stats
from statsmodels.tsa.stattools import adfuller
from statsmodels.graphics.tsaplots import plot_acf
from statsmodels.graphics.tsaplots import plot_pacf
import itertools
from openpyxl import Workbook
from openpyxl.styles import PatternFill, Font, Alignment, Border, Side
from statsmodels.stats.diagnostic import acorr_ljungbox, het_arch, normal_ad
from statsmodels.tsa.arima.model import ARIMA



## Использование параметров хёрста
## ===============================

d1 = pd.read_csv("NIKITA.csv", delimiter=',',encoding="cp1251")
d1['Time'] = pd.to_datetime(d1['Time'])
d1 = d1.set_index('Time')


del d1["Source"]
del d1["Destination"]
del d1["Protocol"]
del d1["Info"]
del d1["No."]


d2 = d1.resample('8s').mean() #агрегация
print(d2)

d2.interpolate(inplace=True) # если NaN
d3=d2[1000:3000]
print(adfuller(d3)) #статистика Дики-Фуллера
k = 0

for i in range(756, len(d2["Length"]), 756):
    
    print(k, i)
    d3=d2[k:i]
    plt.figure(figsize=(12, 5))
    plt.plot(d2.index, d2['Length'])
    plt.title("Length")
    plt.xlabel("Time")
    plt.ylabel("Packet Length")
    plt.grid(True)
    plt.show()
    
    k = i
    break

sp = [(756,1512),(1512,2268), (2268,3024), (3024,3780), (3780,4536), (4536,5292), (5292,6048), (6804,7560), (7560,8316), (9072,9828)]
aicDict = {f"{i} {j}":[] for i, j in sp}
# for x,y in sp:
#     # plot_acf(d2.Length[x:y]) #– график АКФ
#     # plot_acf(d2.Length[x:y].diff().dropna())
#     # plt.show()
#     # plot_pacf(d2.Length[x:y]) #– график ЧАКФ
#     # plot_pacf(d2.Length[x:y].diff().dropna())
#     # plt.show()
#     if adfuller(d2.Length[x:y].diff().dropna())[1] < 0.05:
#         print("p:", adfuller(d2.Length[x:y].diff().dropna())[1])
#         # plot_acf(d2.Length[x:y].diff().dropna())
#         # plot_pacf(d2.Length[x:y].diff().dropna())
#         # plt.show()
#     else: 
#         print("Не стационарен")
#         continue

#     p = range(0,5)
#     d = range(0,3)
#     q = range(0,5)
#     pdq = list(itertools.product(p, d, q))
#     best_pdq = (0,0,0)
#     best_aic = np.inf
#     for params in pdq:
#         model_test = ARIMA(d2.iloc[x:y], order = params)
#         result_test = model_test.fit()
#         if result_test.aic < best_aic:
#             best_pdq = params
#             best_aic = result_test.aic
#             # print(best_pdq, best_aic)
#             aicDict[f"{x} {y}"].append((best_pdq, best_aic))

# with open("nikita.json", "w", encoding="UTF-8") as file:
#     json.dump(aicDict, file, indent=4)


with open("nikita.json", "r", encoding="UTF-8") as file:
    aicDict = json.load(file)

print(aicDict)

spArima = []
for i in aicDict:
    param = min(aicDict[i],key=lambda x: x[1])[0]
    pmodel = ARIMA(d2.iloc[int(i.split()[0]):int(i.split()[1])], order = (param[0], param[1], param[2]))
    
    stats_mdl = pmodel.fit()
    
    print(int(i.split()[0]),int(i.split()[1]))
    print(stats_mdl.summary())
    spArima.append((pmodel, param, i, stats_mdl))
    print("====================================")

# 3024 3780
mape = []
ser = []
ARIMAs = []
Arima = 0
for model, param, indx, fitted_model in spArima:
    if indx == "3024 3780":
        # Данные
        data = d2.iloc[int(indx.split()[0]):int(indx.split()[1])]
        
        # Прогноз на 50 точек
        forecast = fitted_model.predict(start=600, end=len(data["Length"]))
        

        
        # График
        plt.figure(figsize=(12, 6))
        x_point = list(range(600, len(data["Length"])))[::5]
        
        # Прогноз ARIMA с шагом 5
        plt.plot(x_point, data["Length"][600::5], 
                linewidth=2, markersize=8, label=f'ARIMA{param} (шаг={5})')
        plt.plot(x_point, forecast[::5], label="Прогноз")
        # Настройки
        plt.title(f'ARIMA прогноз (каждая {5}-я точка)\nУчасток {indx}')
        plt.xlabel('Индекс')
        plt.ylabel('Значение')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.show()


        forecast = np.array(forecast[::5])
        data = np.array(data["Length"][600::5])
        ARIMAs.append(f"ARIMA{param}")
        Arima = fitted_model
        #MAPE
        mape.append(100*np.mean(np.abs((data - forecast)/data)))

        #SER
        ser.append(10 * np.log10(np.sum(data**2)/ np.sum((data - forecast)**2) ))
        break

print(mape)
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 6))
ax1.bar(ARIMAs, mape, width=0.001)
ax1.set_title("MAPE")
ax2.bar(ARIMAs, ser, width=0.001)
ax2.set_title("SER")
plt.show()



##### ВЫВОД формулы 

def arima_equation(res):
    orders = res.model_orders

    # безопасно берем p, d, q
    p = orders.get("ar", 0)
    d = orders.get("diff", orders.get("integrated", 0))
    q = orders.get("ma", 0)

    ar = res.arparams
    ma = res.maparams

    parts = []

    # AR terms
    for i in range(p):
        parts.append(f"{ar[i]:.4f}·Δ^{d}Y_(t-{i+1})")

    # noise term
    parts.append("ε_t")

    # MA terms
    for i in range(q):
        sign = "+" if ma[i] >= 0 else ""
        parts.append(f"{sign}{ma[i]:.4f}·ε_(t-{i+1})")

    return f"Δ^{d} Y_t = " + " + ".join(parts)

print(arima_equation(Arima))


#### ОСТАТКИ 
def residual_diagnostics(res, lags=10):
    resid = res.resid

    # Ljung–Box
    lb = acorr_ljungbox(resid, lags=[lags], return_df=True)
    lb_stat = lb['lb_stat'].iloc[0]
    lb_p = lb['lb_pvalue'].iloc[0]

    # Jarque–Bera (нормальность)
    jb_stat, jb_p = normal_ad(resid)  # более точный Anderson-Darling тест

    # ARCH-LM (гетероскедастичность)
    arch_stat, arch_p, _, _ = het_arch(resid)

    # AIC модели
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

df = residual_diagnostics(Arima, lags=1)
print(df)

##ГРАФИКИ

Arima.plot_diagnostics(figsize = (15, 10))
plt.show()
```

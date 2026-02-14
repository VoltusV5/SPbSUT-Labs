import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from statsmodels.tsa.seasonal import seasonal_decompose
from statsmodels.graphics.tsaplots import plot_acf, plot_pacf
from statsmodels.tsa.stattools import adfuller
from statsmodels.tsa.holtwinters import ExponentialSmoothing

np.random.seed(42)
n = 120
time = np.arange(n)
trend = 0.7 * time
season = 70 * np.sin(2 * np.pi * time / 12)
noise = np.random.normal(0, 15, n)
data = 300 + trend + season + noise
data = np.round(data).astype(int)

df = pd.DataFrame({'Length': data})

train = df.iloc[:96]
test = df.iloc[96:101]

print("1. Декомпозиция на обучающей выборке (период = 12)")
decomp = seasonal_decompose(train['Length'], model='additive', period=12)
decomp.plot()
plt.show()

seasonal = decomp.seasonal
adjusted = train['Length'] - seasonal

print("2. ACF и PACF десезонализированного ряда")
plot_acf(adjusted, lags=40)
plt.show()
plot_pacf(adjusted, lags=40)
plt.show()

print("ADF p-value:", adfuller(adjusted)[1])



print("\n3. Обучаем правильную модель Holt-Winters")
model = ExponentialSmoothing(train['Length'], trend='add', seasonal='add', seasonal_periods=12)
fit = model.fit()
forecast_final = fit.forecast(5)

print("\nПрогноз на 5 шагов вперёд:")
print(forecast_final.round(1))

mapes = []
for horizon in range(1, 6):
    actual = test['Length'].iloc[:horizon].values
    pred = forecast_final[:horizon].values
    mape = np.mean(np.abs((actual - pred) / actual)) * 100
    mapes.append(mape)
    print(f"MAPE на {horizon} шаг(а/ов): {mape:.2f}%")

plt.figure(figsize=(12, 7))
plt.plot(df.index[:96], train['Length'], label='Обучающая выборка', color='steelblue')
plt.plot(df.index[96:101], test['Length'], label='Фактические значения', color='green', linewidth=3, marker='o')
plt.plot(df.index[96:101], forecast_final, label='Прогноз Holt-Winters', color='red', linewidth=3, linestyle='--', marker='D')

for i in range(5):
    plt.text(df.index[96+i], forecast_final.iloc[i], f'{forecast_final.iloc[i]:.0f}', fontsize=10, ha='left', va='bottom', color='red')
    plt.text(df.index[96+i], test['Length'].iloc[i], f'{test["Length"].iloc[i]}', fontsize=10, ha='right', va='top', color='green')

plt.title(f'Прогноз временного ряда\nMAPE: 1={mapes[0]:.2f}%  2={mapes[1]:.2f}%  3={mapes[2]:.2f}%  4={mapes[3]:.2f}%  5={mapes[4]:.2f}%')
plt.xlabel('Время')
plt.ylabel('Значение')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()

residuals = test['Length'] - forecast_final
plt.figure(figsize=(10, 4))
plt.plot(test.index, residuals, marker='o', color='purple', linewidth=2)
plt.axhline(0, color='black', linestyle='--')
plt.title(f'График остатков\nСредняя абсолютная ошибка = {abs(residuals).mean():.1f}')
plt.ylabel('Ошибка')
plt.grid(True, alpha=0.3)
plt.show()
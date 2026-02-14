import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from statsmodels.tsa.seasonal import seasonal_decompose
from statsmodels.graphics.tsaplots import plot_acf, plot_pacf
from statsmodels.tsa.arima.model import ARIMA
from statsmodels.tsa.statespace.sarimax import SARIMAX
from statsmodels.tsa.stattools import adfuller

np.random.seed(42)
n = 120
time = np.arange(n)
trend = 0.7 * time
season = 70 * np.sin(2 * np.pi * time / 12)
noise = np.random.normal(0, 15, n)
data = 300 + trend + season + noise
data = np.round(data).astype(int)

df = pd.DataFrame({'Length': data}, index=pd.RangeIndex(n))
train = df.iloc[:96]
test  = df.iloc[96:101]

print("1. Декомпозиция (период = 12)")
decomp = seasonal_decompose(train['Length'], model='additive', period=12)
decomp.plot()
plt.tight_layout()
plt.show()

seasonal_component = decomp.seasonal
adjusted = train['Length'] - seasonal_component          # десезонализированный ряд

print("2. ACF и PACF десезонализированного ряда")
plot_acf(adjusted, lags=40)
plt.show()
plot_pacf(adjusted, lags=40)
plt.show()

print("ADF p-value на десезонализированном ряде:", adfuller(adjusted)[1])


model_arima = ARIMA(adjusted, order=(0, 2, 2))
fit_arima = model_arima.fit()

print(fit_arima.summary())

forecast_adjusted = fit_arima.forecast(steps=5)


last_seasonal_cycle = seasonal_component[-12:].values

# Для прогноза берём нужные 5 значений сезонности (по модулю 12)
season_to_add = last_seasonal_cycle[-5:] if len(last_seasonal_cycle) >= 5 else np.tile(last_seasonal_cycle, 2)[-5:]

forecast_final = forecast_adjusted + season_to_add

print("\nПрогноз ARIMA + сезонность на 5 шагов вперёд:")
print(forecast_final.round(1))

mapes = []
for h in range(1, 6):
    actual = test['Length'].iloc[:h].values
    pred   = forecast_final[:h].values
    mape   = np.mean(np.abs((actual - pred) / actual)) * 100
    mapes.append(mape)
    print(f"MAPE на {h} шаг(ов): {mape:.2f}%")


plt.figure(figsize=(12, 7))
plt.plot(df.index[:96], train['Length'], label='Обучающая выборка', color='steelblue')
plt.plot(df.index[96:101], test['Length'], label='Фактические значения', color='green', linewidth=3, marker='o')
plt.plot(df.index[96:101], forecast_final, label='Прогноз ARIMA(0,2,2)+сезонность', color='red', linewidth=3, linestyle='--', marker='D')

for i in range(5):
    plt.text(df.index[96+i], forecast_final.iloc[i], f'{forecast_final.iloc[i]:.0f}', 
             fontsize=10, ha='left', va='bottom', color='red')
    plt.text(df.index[96+i], test['Length'].iloc[i], f'{test["Length"].iloc[i]}', 
             fontsize=10, ha='right', va='top', color='green')

plt.title(f'Прогноз ARIMA + ручная сезонность\n'
          f'MAPE: 1={mapes[0]:.2f}%  2={mapes[1]:.2f}%  3={mapes[2]:.2f}%  4={mapes[3]:.2f}%  5={mapes[4]:.2f}%')
plt.xlabel('Время')
plt.ylabel('Значение')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()


residuals = test['Length'].values - forecast_final.values

plt.figure(figsize=(10, 4))
plt.plot(test.index, residuals, marker='o', color='purple', linewidth=2)
plt.axhline(0, color='black', linestyle='--')
plt.title(f'График остатков (ARIMA)\nСредняя абсолютная ошибка = {np.abs(residuals).mean():.1f}')
plt.ylabel('Ошибка')
plt.grid(True, alpha=0.3)
plt.show()
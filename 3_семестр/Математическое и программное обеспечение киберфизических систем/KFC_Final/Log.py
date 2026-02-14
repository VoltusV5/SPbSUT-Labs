import pandas as pd
import numpy as np

dataH = pd.read_csv("dataTime.csv")
dataH['Time'][0] = 0.00001  # чтобы не было логарифма от нуля

# Вычисление логарифмической разности
data = pd.DataFrame(
    {"log_dif": np.log(dataH['Time']) - np.log(dataH['Time'].shift(-1))})

# Удаление NaN значений, если есть
data = data.dropna()
data.to_csv("logDifference.csv")
print(len(data))

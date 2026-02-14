import pandas as pd
import matplotlib.pyplot as plt

# Таблица данных
data = {
    "a": [0.2, 0.5, 0.8, 0.9, 0.99],
    "T1_I": [1.042, 1.333, 2.875, 4.985, 47.79],
    "T2_I": [1.043, 1.34, 2.794, 5.203, 56.80],
    "T1_II": [1.021, 1.167, 1.842, 3.025, 16.16],
    "T2_II": [1.030, 1.245, 2.279, 4.671, 29.73],
    "T1_III": [1.040, 1.323, 2.737, 5.233, 32.876],
    "T2_III": [1.062, 1.368, 2.098, 2.658, 4.248],
}

df = pd.DataFrame(data)

# Среднее время = (T1 + T2) / 2 для каждого эксперимента
df["Mean_I"] = (df["T1_I"] + df["T2_I"]) / 2
df["Mean_II"] = (df["T1_II"] + df["T2_II"]) / 2
df["Mean_III"] = (df["T1_III"] + df["T2_III"]) / 2

# Построение графика
plt.figure(figsize=(8,6))
plt.plot(df["a"], df["Mean_I"], marker='o', label="Эксперимент I")
plt.plot(df["a"], df["Mean_II"], marker='s', label="Эксперимент II")
plt.plot(df["a"], df["Mean_III"], marker='^', label="Эксперимент III")

plt.xlabel("Интенсивность нагрузки (a)")
plt.ylabel("Среднее время обслуживания (Tср)")
plt.title("Зависимость среднего времени обслуживания от нагрузки")
plt.legend()
plt.grid(True)
plt.show()

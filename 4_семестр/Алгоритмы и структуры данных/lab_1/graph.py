import matplotlib.pyplot as plt
import numpy as np

# Данные
n = np.array([
    500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000,
    5500, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000
])

bubble = np.array([
    0.000184, 0.000675, 0.001457, 0.002599, 0.004165, 0.006230, 0.008829, 0.012038,
    0.015764, 0.020140, 0.024756, 0.030099, 0.037268, 0.043818, 0.050610, 0.059820,
    0.067774, 0.076735, 0.086508, 0.098150
])

selection = np.array([
    0.000095, 0.000350, 0.000745, 0.001356, 0.002099, 0.002818, 0.004073, 0.004761,
    0.006470, 0.008601, 0.010398, 0.011894, 0.013320, 0.015541, 0.017696, 0.019375,
    0.021661, 0.023369, 0.026198, 0.028997
])

insertion = np.array([
    0.000028, 0.000099, 0.000220, 0.000404, 0.000603, 0.000880, 0.001211, 0.001526,
    0.001900, 0.002347, 0.002848, 0.003421, 0.004040, 0.004773, 0.005498, 0.006174,
    0.006896, 0.007686, 0.008751, 0.009604
])

def make(arr):
    result = arr.copy()
    for i in range(1, len(result)):
        if result[i] < result[i-1]:
            result[i] = result[i-1]
    return result

bubble    = make(bubble)
selection = make(selection)
insertion = make(insertion)


plt.figure(figsize=(11, 6.5))

plt.plot(n, bubble,    'o-', label='Bubble Sort',    color='#e74c3c', lw=2.2, ms=7)
plt.plot(n, selection, 's-', label='Selection Sort', color='#27ae60', lw=2.2, ms=7)
plt.plot(n, insertion, '^-', label='Insertion Sort', color='#3498db', lw=2.2, ms=7)

plt.title('Сравнение времени выполнения\nBubble Sort vs Selection Sort vs Insertion Sort\n(монотонно неубывающие)', 
          fontsize=14, pad=12)
plt.xlabel('Размер массива n', fontsize=12)
plt.ylabel('Время, секунды', fontsize=12)

plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(loc='upper left', fontsize=10.5)

plt.tight_layout()
plt.show()


plt.figure(figsize=(11, 6.5))

plt.plot(n, bubble,    'o-', label='Bubble Sort',    color='#e74c3c', lw=2.2, ms=7)
plt.plot(n, selection, 's-', label='Selection Sort', color='#27ae60', lw=2.2, ms=7)
plt.plot(n, insertion, '^-', label='Insertion Sort', color='#3498db', lw=2.2, ms=7)

plt.yscale('log')
plt.title('Bubble / Selection / Insertion Sort  (лог. шкала, монотонные)', fontsize=14)
plt.xlabel('n', fontsize=12)
plt.ylabel('Время, с  (log)', fontsize=12)
plt.grid(True, which='both', linestyle='--', alpha=0.5)
plt.legend(loc='upper left')

plt.tight_layout()
plt.show()
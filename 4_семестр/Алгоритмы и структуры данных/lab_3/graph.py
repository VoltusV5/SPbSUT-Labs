import matplotlib.pyplot as plt
import numpy as np

data = """h Euler RungeKutta AdamsBashforth
1,00E-01 2,17E-06 5,45E-06 2,99E-06
5,00E-02 2,22E-06 9,16E-06 5,02E-06
2,50E-02 5,02E-06 2,05E-05 8,64E-06
1,25E-02 1,24E-05 3,83E-05 1,47E-05
6,25E-03 1,97E-05 8,24E-05 2,41E-05
3,13E-03 4,00E-05 1,59E-04 5,55E-05
1,56E-03 7,88E-05 3,23E-04 1,17E-04
7,81E-04 1,65E-04 6,50E-04 2,08E-04"""

lines = data.strip().split('\n')
h = []
euler = []
rk = []
ab = []

for line in lines[1:]:
    parts = line.replace(',', '.').split()
    h.append(float(parts[0]))
    euler.append(float(parts[1]))
    rk.append(float(parts[2]))
    ab.append(float(parts[3]))

x = np.arange(1, len(h) + 1) 

plt.figure(figsize=(11, 7))

plt.plot(x, euler, '-', label='Euler',           color='blue',  linewidth=2.5)
plt.plot(x, rk,    '-', label='Runge-Kutta',     color='red',   linewidth=2.5)
plt.plot(x, ab,    '-', label='Adams-Bashforth', color='green', linewidth=2.5)

plt.title('Euler & RungeKutta & Adams-Bashforth Methods', fontsize=15, pad=15)
plt.xlabel('Neighborhood of the point', fontsize=12)
plt.ylabel('Time (seconds)', fontsize=12)
plt.grid(True, alpha=0.3)
plt.legend(fontsize=11)
plt.xticks(x)

plt.tight_layout()
plt.savefig('all_three_methods.png', dpi=300, bbox_inches='tight')
plt.show()
plt.figure(figsize=(11, 7))

plt.plot(x, euler, '-', label='Euler',           color='blue',  linewidth=2.5)
plt.plot(x, ab,    '-', label='Adams-Bashforth', color='green', linewidth=2.5)

plt.title('Euler & Adams-Bashforth Methods', fontsize=15, pad=15)
plt.xlabel('Neighborhood of the point', fontsize=12)
plt.ylabel('Time (seconds)', fontsize=12)
plt.grid(True, alpha=0.3)
plt.legend(fontsize=11)
plt.xticks(x)

plt.tight_layout()
plt.savefig('euler_and_adams.png', dpi=300, bbox_inches='tight')
plt.show()

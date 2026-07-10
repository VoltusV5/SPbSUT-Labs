import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('graph_bench.csv')

plt.figure(figsize=(10, 6))
plt.plot(df['Vertices'], df['Dijkstra'], label='Dijkstra O(E log V)', linewidth=4)
plt.plot(df['Vertices'], df['BellmanFord'], label='Bellman-Ford O(V*E)',linewidth=4)
plt.plot(df['Vertices'], df['FloydWarshall'], label='Floyd-Warshall O(V^3)', linewidth=4)

plt.xlim(left=0)
plt.ylim(bottom=0)
plt.title('Graph Algorithms Complexity: Time vs Vertices')
plt.xlabel('Number of Vertices (V)')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.show()
import pandas as pd
import matplotlib.pyplot as plt
import os

# Create directory for assets if it doesn't exist
assets_dir = 'assets'
if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)

# Read the data
data = pd.read_csv('src/rez.txt')

# Create Excel file
with pd.ExcelWriter('mst_algorithms_performance.xlsx', engine='openpyxl') as writer:
    data.to_excel(writer, index=False, sheet_name='Measurements')
    
    # In a real scenario, adding a chart to Excel via openpyxl is possible but complex.
    # For now, we provide the data in Excel as requested.

# Create Plot
plt.figure(figsize=(10, 6))
plt.plot(data['Vertices'], data['Kruskal'], label='Kruskal O(E log E)')
plt.plot(data['Vertices'], data['Prim'], label='Prim O(V^2)')
plt.plot(data['Vertices'], data['Boruvka'], label='Boruvka O(E log V)')

plt.title('MST Algorithms Performance Comparison')
plt.xlabel('Number of Vertices (V)')
plt.ylabel('Time (ns)')
plt.legend()
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.yscale('linear') # Can be 'log' if needed

# Save the plot
plt.savefig(os.path.join(assets_dir, 'mst_performance_graph.png'), dpi=300)
plt.close()

print("Excel file and graph image generated successfully.")

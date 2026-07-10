import networkx as nx
import matplotlib.pyplot as plt
import os

# Create directory for assets if it doesn't exist
assets_dir = 'assets'
if not os.path.exists(assets_dir):
    os.makedirs(assets_dir)

# Define the graph
G = nx.Graph()
edges = [
    (0, 1, 2),
    (0, 3, 6),
    (1, 2, 3),
    (1, 3, 8),
    (1, 4, 5),
    (2, 4, 7),
    (3, 4, 9)
]

for u, v, w in edges:
    G.add_edge(u, v, weight=w)

# Set positions for all nodes
pos = nx.spring_layout(G, seed=42) # Seed for reproducibility

# Draw nodes and edges
plt.figure(figsize=(8, 6))
nx.draw_networkx_nodes(G, pos, node_size=700, node_color='skyblue')
nx.draw_networkx_edges(G, pos, width=2, alpha=0.7, edge_color='black')
nx.draw_networkx_labels(G, pos, font_size=20, font_family='sans-serif')

# Edge weight labels
edge_labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=16)

plt.title("Initial Graph (Section 2.3)", fontsize=16)
plt.axis('off') # Hide axes

# Save the plot
plt.savefig(os.path.join(assets_dir, 'initial_graph.png'), dpi=300, bbox_inches='tight')
plt.close()

print("Initial graph image generated successfully using networkx.")

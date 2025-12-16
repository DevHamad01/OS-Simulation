import tkinter as tk
from tkinter import messagebox
import json
import os
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

def show_view(json_path):
    # Create Toplevel for Graph
    root = tk.Toplevel()
    root.title("Resource Allocation Graph")
    root.geometry("800x600")
    
    if not os.path.exists(json_path):
        messagebox.showerror("Error", "Output file not found.")
        return

    with open(json_path, 'r') as f:
        data = json.load(f)

    is_deadlock = data.get("deadlock", False)
    cycle_nodes = set(data.get("cycle", []))

    # Reconstruct graph from input.json (Need to read input again? 
    # Or just rely on output? Output might not have edges if we didn't pass them back.
    # Actually, RAG output usually just says deadlock or not.
    # To draw the graph, we need the edges. 
    # Let's read input.json as well for the structure.
    base_dir = os.path.dirname(json_path)
    input_path = os.path.join(base_dir, "input.json")
    
    if not os.path.exists(input_path):
        messagebox.showerror("Error", "Input file missing, cannot draw graph.")
        return
        
    with open(input_path, 'r') as f:
        inp = json.load(f)
        
    edges = inp.get("deadlock", {}).get("edges", [])
    
    G = nx.DiGraph()
    for e in edges:
        G.add_edge(e["from"], e["to"])

    # Drawing
    fig = plt.Figure(figsize=(6, 5), dpi=100)
    ax = fig.add_subplot(111)
    
    pos = nx.spring_layout(G)
    
    # Colors
    node_colors = []
    for node in G.nodes():
        if node in cycle_nodes:
            node_colors.append('red') # In deadlock cycle
        elif "P" in node:
            node_colors.append('skyblue') # Process
        else:
            node_colors.append('lightgreen') # Resource

    nx.draw(G, pos, ax=ax, with_labels=True, node_color=node_colors, 
            node_size=2000, arrowsize=20, font_weight='bold')
    
    ax.set_title("Deadlock Detected!" if is_deadlock else "No Deadlock", 
                 color='red' if is_deadlock else 'green')

    canvas = FigureCanvasTkAgg(fig, master=root)
    canvas.draw()
    canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    tk.Button(root, text="Close", command=root.destroy).pack(pady=5)

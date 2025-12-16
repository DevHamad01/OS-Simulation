import tkinter as tk
from tkinter import ttk, messagebox
import json
import os

def show_view(json_path):
    root = tk.Toplevel()
    root.title("Banker's Algorithm Result")
    root.geometry("600x500")

    if not os.path.exists(json_path):
        messagebox.showerror("Error", "Output file not found.")
        return

    with open(json_path, 'r') as f:
        data = json.load(f)

    # Header
    lbl_safe = tk.Label(root, text="System is SAFE" if data.get("safe") else "System is UNSAFE", 
                       fg="green" if data.get("safe") else "red", font=("Helvetica", 16, "bold"))
    lbl_safe.pack(pady=10)

    if data.get("safe"):
        seq = " -> ".join(data.get("safe_sequence", []))
        tk.Label(root, text=f"Safe Sequence: {seq}", font=("Helvetica", 12)).pack(pady=5)

    # Steps Log
    frame_steps = ttk.LabelFrame(root, text="Execution Steps")
    frame_steps.pack(fill="both", expand=True, padx=10, pady=10)

    text_steps = tk.Text(frame_steps, height=15)
    text_steps.pack(side="left", fill="both", expand=True)
    
    scrollbar = ttk.Scrollbar(frame_steps, orient="vertical", command=text_steps.yview)
    scrollbar.pack(side="right", fill="y")
    text_steps.config(yscrollcommand=scrollbar.set)

    steps = data.get("steps", [])
    for s in steps:
        msg = f"Process {s['chosen_process']} executed.\n"
        msg += f"  Work Available Before: {s['work_before']}\n"
        msg += f"  Work Available After:  {s['work_after']}\n"
        msg += "-"*40 + "\n"
        text_steps.insert("end", msg)
    
    text_steps.config(state="disabled")

    ttk.Button(root, text="Close", command=root.destroy).pack(pady=10)

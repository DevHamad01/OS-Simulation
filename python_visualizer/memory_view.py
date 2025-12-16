import tkinter as tk
from tkinter import ttk, messagebox
import json
import os

class MemoryView:
    def __init__(self, json_path):
        self.root = tk.Toplevel()
        self.root.title("Memory Management Animation")
        self.root.geometry("700x500")
        
        if not os.path.exists(json_path):
            messagebox.showerror("Error", "Output file not found.")
            return

        with open(json_path, 'r') as f:
            self.data = json.load(f)
            
        self.steps = self.data.get("steps", [])
        self.current_step = 0
        self.page_faults = self.data.get("page_faults", 0)
        self.hits = self.data.get("hits", 0)
        
        self.create_widgets()
        
    def create_widgets(self):
        # Stats
        frame_stats = tk.Frame(self.root)
        frame_stats.pack(pady=10)
        
        tk.Label(frame_stats, text=f"Page Faults: {self.page_faults}", fg="red", font=("Arial", 12, "bold")).pack(side="left", padx=20)
        tk.Label(frame_stats, text=f"Hits: {self.hits}", fg="green", font=("Arial", 12, "bold")).pack(side="left", padx=20)
        
        # Canvas for Frames
        self.canvas = tk.Canvas(self.root, width=600, height=200, bg="white")
        self.canvas.pack(pady=20)
        
        # Controls
        frame_controls = tk.Frame(self.root)
        frame_controls.pack(pady=10)
        
        ttk.Button(frame_controls, text="Previous", command=self.prev_step).pack(side="left", padx=5)
        ttk.Button(frame_controls, text="Next", command=self.next_step).pack(side="left", padx=5)
        ttk.Button(frame_controls, text="Play Animation", command=self.play).pack(side="left", padx=5)
        
        self.lbl_info = tk.Label(self.root, text="Start", font=("Arial", 12))
        self.lbl_info.pack(pady=10)
        
        self.draw_step()

    def draw_box(self, x, y, val, color="lightgray"):
        self.canvas.create_rectangle(x, y, x+60, y+60, fill=color, outline="black")
        self.canvas.create_text(x+30, y+30, text=str(val if val != -1 else "-"))

    def draw_step(self):
        self.canvas.delete("all")
        
        if self.current_step < 0 or self.current_step >= len(self.steps):
            return
            
        step_data = self.steps[self.current_step]
        frames = step_data.get("frames", [])
        page = step_data.get("page")
        status = step_data.get("status")
        
        # Info
        self.lbl_info.config(text=f"Step {self.current_step + 1}: Visiting Page {page} -> {status}")
        
        # Draw Frames
        start_x = 50
        y = 70
        
        for i, f_val in enumerate(frames):
            # Highlight if it's the page we just touched? 
            # Ideally we'd know which index was modified, but we just have the state.
            color = "white"
            if f_val == page:
                color = "lightgreen" if status == "HIT" else "salmon"
            
            self.draw_box(start_x + i*70, y, f_val, color)
            self.canvas.create_text(start_x + i*70 + 30, y + 80, text=f"Frame {i}")

    def next_step(self):
        if self.current_step < len(self.steps) - 1:
            self.current_step += 1
            self.draw_step()

    def prev_step(self):
        if self.current_step > 0:
            self.current_step -= 1
            self.draw_step()

    def play(self):
        if self.current_step < len(self.steps) - 1:
            self.next_step()
            self.root.after(1000, self.play)

def show_view(json_path):
    MemoryView(json_path)

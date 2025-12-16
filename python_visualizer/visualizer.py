import tkinter as tk
from tkinter import ttk, messagebox
import json
import subprocess
import os
import sys

# Import views (will be created next)
# from banker_view import show_banker_view
# from rag_view import show_rag_view
# from memory_view import show_memory_view

class OSSimulatorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("OS Simulator - C++ Core & Python Viz")
        self.root.geometry("600x400")
        
        # Paths
        self.base_dir = os.path.dirname(os.path.abspath(__file__))
        self.root_dir = os.path.dirname(self.base_dir)
        self.cpp_exe = os.path.join(self.root_dir, "cpp_core", "os_core.exe")
        self.data_dir = os.path.join(self.root_dir, "data")
        self.input_file = os.path.join(self.data_dir, "input.json")
        self.output_file = os.path.join(self.data_dir, "output.json")
        
        self.create_widgets()

    def create_widgets(self):
        title = tk.Label(self.root, text="Operating System Simulator", font=("Helvetica", 24, "bold"))
        title.pack(pady=20)

        frame = tk.Frame(self.root)
        frame.pack(pady=20)

        btn_banker = ttk.Button(frame, text="Banker's Algorithm", command=self.open_banker_input)
        btn_banker.grid(row=0, column=0, padx=10, pady=10, ipady=10, ipadx=10)

        btn_deadlock = ttk.Button(frame, text="Deadlock Detection (RAG)", command=self.open_deadlock_input)
        btn_deadlock.grid(row=0, column=1, padx=10, pady=10, ipady=10, ipadx=10)

        btn_memory = ttk.Button(frame, text="Memory Management", command=self.open_memory_input)
        btn_memory.grid(row=1, column=0, columnspan=2, padx=10, pady=10, ipady=10, ipadx=10)
        
        lbl_status = tk.Label(self.root, text="Status: Ready", fg="blue")
        lbl_status.pack(side=tk.BOTTOM, pady=10)
        self.status_label = lbl_status

    def run_core(self):
        self.status_label.config(text="Running C++ Core...", fg="orange")
        self.root.update()
        
        if not os.path.exists(self.cpp_exe):
            messagebox.showerror("Error", f"Executable not found:\n{self.cpp_exe}\nPlease compile the C++ core first.")
            self.status_label.config(text="Error: C++ executable missing", fg="red")
            return False

        try:
            result = subprocess.run([self.cpp_exe, self.input_file, self.output_file], capture_output=True, text=True)
            if result.returncode != 0:
                messagebox.showerror("Core Error", f"C++ Core failed:\n{result.stderr}")
                self.status_label.config(text="Error in C++ Core", fg="red")
                return False
            
            self.status_label.config(text="Simulation Complete.", fg="green")
            return True
        except Exception as e:
            messagebox.showerror("Execution Error", str(e))
            self.status_label.config(text="Error Executing Core", fg="red")
            return False

    def open_banker_input(self):
        # Placeholder for inputs
        # For simplicity, we'll write a default input.json and run
        # In a real app, showing a form dialog to collect numbers is better
        
        # Determine if we want to show a dialog or just run a demo
        win = tk.Toplevel(self.root)
        win.title("Banker's Algorithm Input")
        
        tk.Label(win, text="Number of Processes:").grid(row=0, column=0)
        tk.Label(win, text="Number of Resources:").grid(row=1, column=0)
        
        e_proc = tk.Entry(win)
        e_proc.insert(0, "5")
        e_proc.grid(row=0, column=1)
        
        e_res = tk.Entry(win)
        e_res.insert(0, "3")
        e_res.grid(row=1, column=1)
        
        def run_sim():
            # Create a dummy input for demo purposes
            # Real implementation would parse matrices from user input
            data = {
                "mode": "BANKER",
                "banker": {
                    "processes": 5,
                    "resources": 3,
                    "allocation": [
                        [0,1,0], [2,0,0], [3,0,2], [2,1,1], [0,0,2]
                    ],
                    "max": [
                        [7,5,3], [3,2,2], [9,0,2], [2,2,2], [4,3,3]
                    ],
                    "available": [3,3,2]
                }
            }
            
            with open(self.input_file, 'w') as f:
                json.dump(data, f, indent=4)
                
            if self.run_core():
                # Load output and visualize
                # show_banker_view(self.output_file)
                import banker_view
                banker_view.show_view(self.output_file)
                win.destroy()

        ttk.Button(win, text="Run Simulation (Demo Data)", command=run_sim).grid(row=2, columnspan=2, pady=10)

    def open_deadlock_input(self):
        win = tk.Toplevel(self.root)
        win.title("Deadlock Detection Input")
        
        def run_sim():
            data = {
                "mode": "DEADLOCK",
                "deadlock": {
                    "edges": [
                        {"from": "P1", "to": "R1"},
                        {"from": "R1", "to": "P2"},
                        {"from": "P2", "to": "R1"} # Cycle
                    ]
                }
            }
            with open(self.input_file, 'w') as f:
                json.dump(data, f, indent=4)
                
            if self.run_core():
                import rag_view
                rag_view.show_view(self.output_file)
                win.destroy()

        ttk.Button(win, text="Run Simulation (Cycle Demo)", command=run_sim).grid(row=0, column=0, padx=20, pady=20)

    def open_memory_input(self):
        win = tk.Toplevel(self.root)
        win.title("Memory Management Input")
        
        def run_sim():
            data = {
                "mode": "MEMORY",
                "memory": {
                    "frames": 3,
                    "algorithm": "LRU",
                    "references": [7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1] 
                }
            }
            with open(self.input_file, 'w') as f:
                json.dump(data, f, indent=4)
            
            if self.run_core():
                import memory_view
                memory_view.show_view(self.output_file)
                win.destroy()

        ttk.Button(win, text="Run Simulation (LRU Demo)", command=run_sim).grid(row=0, column=0, padx=20, pady=20)


if __name__ == "__main__":
    if not os.path.exists("data"):
        os.makedirs("data", exist_ok=True)
        
    root = tk.Tk()
    app = OSSimulatorApp(root)
    root.mainloop()

# OS Simulator Project

An educational Operating System Simulator that visualizes key OS algorithms using a C++ Core Engine and a Python Frontend.

## Modules Implemented
1. **Banker's Algorithm**: Deadlock Avoidance and Safe Sequence calculation.
2. **Deadlock Detection**: Resource Allocation Graph (RAG) cycle detection.
3. **Memory Management**: Paging and Page Replacement Algorithms (FIFO, LRU, Optimal).

## Prerequisites
- **C++ Compiler**: `g++` (MinGW) or `cl.exe` (MSVC) or `clang++`.
- **Python 3.x**
- **Python Libraries**: `tkinter`, `networkx`, `matplotlib`.
  ```bash
  pip install networkx matplotlib
  ```
  (`tkinter` usually comes with Python)

## Installation & Compilation
1. Navigate to the `cpp_core` directory:
   ```bash
   cd cpp_core
   ```
2. Compile the C++ Core:
   **Using g++:**
   ```bash
   g++ -o os_core.exe main.cpp banker.cpp deadlock.cpp memory.cpp
   ```
   **Using MSVC (cl):**
   ```bash
   cl /Fe:os_core.exe main.cpp banker.cpp deadlock.cpp memory.cpp
   ```
   *Note: Ensure the executable is named `os_core.exe` and is located in `cpp_core/`.*

## How to Run
1. Navigate to the `python_visualizer` directory:
   ```bash
   cd ../python_visualizer
   ```
2. Run the Visualizer:
   ```bash
   python visualizer.py
   ```
3. Initial Steps:
   - Click on a Module (e.g., Banker's Algorithm).
   - Enter input data or use the "Run Demo" button.
   - The Python UI will send data to the C++ Core.
   - The C++ Core computes the result.
   - The UI reads back the result and animates it.

## File Structure
- `cpp_core/`: Source code for OS algorithms.
- `python_visualizer/`: Source code for GUI and Animation.
- `data/`: JSON files used for communication (`input.json` sent to C++, `output.json` received).

## Troubleshooting
- **"Executable not found"**: Make sure you compiled the C++ code and the file `os_core.exe` exists in `cpp_core`.
- **"Error in C++ Core"**: Check if the C++ code runs manually with `os_core.exe ../data/input.json ../data/output.json`.

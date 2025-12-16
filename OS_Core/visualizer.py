import json
import matplotlib.pyplot as plt
import numpy as np
import os

# -------------------- BANKER'S ALGORITHM --------------------
def visualize_bankers():
    json_file = "visualizer/banker_steps.json"
    if not os.path.exists(json_file):
        print("Banker JSON file not found! Run Banker’s Algorithm first.")
        return

    with open(json_file) as f:
        steps = json.load(f)

    print("========== BANKER'S ALGORITHM VISUALIZER ==========")
    for idx, step in enumerate(steps):
        if step.get("action") == "executed":
            process = step["process"]
            work = step["work"]
            print(f"Step {idx+1}: Process P{process} executed → Work: {work}")

            # Plot Work matrix
            fig, ax = plt.subplots()
            work_matrix = np.array([work])
            im = ax.imshow(work_matrix, cmap="Greens", vmin=0)
            for i in range(work_matrix.shape[0]):
                for j in range(work_matrix.shape[1]):
                    ax.text(j, i, work_matrix[i, j], ha='center', va='center', color='black', fontsize=12)

            ax.set_xticks(range(len(work)))
            ax.set_yticks([0])
            ax.set_yticklabels([f"P{process}"])
            ax.set_title(f"Step {idx+1}: Process P{process} executed")
            plt.colorbar(im, ax=ax, label="Resources")
            plt.show()

        elif step.get("action") == "deadlock":
            print("⚠️ DEADLOCK DETECTED")


# -------------------- DEADLOCK DETECTION --------------------
def visualize_deadlock():
    json_file = "visualizer/deadlock_steps.json"
    if not os.path.exists(json_file):
        print("Deadlock JSON file not found! Run Deadlock Detection first.")
        return

    with open(json_file) as f:
        steps = json.load(f)

    print("========== DEADLOCK DETECTION VISUALIZER ==========")
    for idx, step in enumerate(steps[:-1]):  # Last step is summary
        pid = step["process"]
        action = step["action"]
        work_before = step["work_before"]
        work_after = step["work_after"]

        if action == "executed":
            print(f"Step {idx+1}: Process P{pid} executed → Work: {work_after}")
            color_map = "Greens"
            matrix = np.array([work_after])
        elif action == "waiting":
            print(f"Step {idx+1}: Process P{pid} waiting → Work: {work_before}")
            color_map = "Reds"
            matrix = np.array([work_before])

        fig, ax = plt.subplots()
        im = ax.imshow(matrix, cmap=color_map, vmin=0)
        for i in range(matrix.shape[0]):
            for j in range(matrix.shape[1]):
                ax.text(j, i, matrix[i, j], ha='center', va='center', color='black', fontsize=12)

        ax.set_xticks(range(len(matrix[0])))
        ax.set_yticks([0])
        ax.set_yticklabels([f"P{pid}"])
        ax.set_title(f"Step {idx+1}: Process P{pid} {'executed' if action=='executed' else 'waiting'}")
        plt.colorbar(im, ax=ax, label="Resources")
        plt.show()

    summary = steps[-1]
    if summary["deadlock_detected"]:
        dead = ', '.join([f"P{p}" for p in summary["deadlocked_processes"]])
        print(f"⚠️ DEADLOCK DETECTED! Processes in deadlock: {dead}")
    else:
        print("✅ NO DEADLOCK: All processes completed")


# -------------------- MEMORY / PAGE REPLACEMENT --------------------
def visualize_memory():
    input_file = "visualizer/input.json"
    output_file = "visualizer/output.json"

    if not os.path.exists(input_file):
        print("Memory input.json not found! Run Python input step first.")
        return

    if not os.path.exists(output_file):
        print("Memory output.json not found! Run C++ core first (OS_Core.exe).")
        return

    with open(output_file) as f:
        output = json.load(f)

    print("========== MEMORY / PAGE REPLACEMENT VISUALIZER ==========")
    for idx, step in enumerate(output["steps"]):
        pid = step["process"]
        ref = step["reference"]
        frames = step["frames"]
        pf = step["page_fault"]
        pf_text = "⚠️ Page Fault" if pf else "✅ Hit"
        print(f"Step {idx+1}: P{pid} → Ref:{ref} | Frames: {frames} | {pf_text}")

        color_map = "Greens" if not pf else "Reds"
        matrix = np.array([frames])

        fig, ax = plt.subplots()
        im = ax.imshow(matrix, cmap=color_map, vmin=0)
        for i in range(matrix.shape[0]):
            for j in range(matrix.shape[1]):
                ax.text(j, i, matrix[i, j], ha='center', va='center', color='black', fontsize=12)

        ax.set_xticks(range(len(matrix[0])))
        ax.set_yticks([0])
        ax.set_yticklabels([f"P{pid}"])
        ax.set_title(f"Step {idx+1}: P{pid} {'Page Fault' if pf else 'Hit'}")
        plt.colorbar(im, ax=ax, label="Frames")
        plt.show()

    print(f"\nTotal Page Faults: {output['total_page_faults']}")


# -------------------- MAIN MENU --------------------
if __name__ == "__main__":
    while True:
        print("\nWhich module to visualize?")
        print("1. Banker’s Algorithm")
        print("2. Deadlock Detection (RAG)")
        print("3. Memory / Page Replacement")
        print("4. Exit")
        choice = input("Enter choice: ")

        if choice == "1":
            visualize_bankers()
        elif choice == "2":
            visualize_deadlock()
        elif choice == "3":
            visualize_memory()
        elif choice == "4":
            break
        else:
            print("Invalid choice! Try again.")

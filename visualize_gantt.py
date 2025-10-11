import pandas as pd
import matplotlib.pyplot as plt

def plot_gantt_timeline(csv_file, algo_name):
    """
    Plots a Gantt chart from timeline-based CSV:
    CSV must have columns: PID, Start, End, Waiting, Turnaround (optional for timeline CSVs)
    """
    try:
        df = pd.read_csv(csv_file)
    except FileNotFoundError:
        print(f"⚠️ File not found: {csv_file}")
        return

    # For timeline CSVs (RR/MLFQ), compute Duration if End column exists
    if 'End' in df.columns and 'Start' in df.columns:
        df['Duration'] = df['End'] - df['Start']
    elif 'Burst' in df.columns and 'Completion' in df.columns:
        # For FCFS/SRTF
        df['Start'] = df['Completion'] - df['Burst']
        df['Duration'] = df['Burst']
    else:
        print("⚠️ CSV format not recognized.")
        return

    df = df.sort_values(by='Start').reset_index(drop=True)
    colors = plt.cm.tab10.colors

    fig, ax = plt.subplots(figsize=(10, 4))
    for i, row in df.iterrows():
        ax.barh(y=0, width=row['Duration'], left=row['Start'],
                color=colors[int(row['PID']) % len(colors)], edgecolor='black', height=0.2)
        ax.text((row['Start'] + row['Start'] + row['Duration'])/2, 0, f"P{int(row['PID'])}",
                ha='center', va='center', color='white', fontsize=9, fontweight='bold')

    ax.set_xlabel("Time")
    ax.set_title(f"Gantt Chart for {algo_name}")
    ax.set_yticks([])
    ax.grid(axis='x', linestyle='--', alpha=0.5)

    # Display average WT and TAT at top-right corner if available
    if 'Waiting' in df.columns and 'Turnaround' in df.columns:
        avg_wt = df['Waiting'].mean()
        avg_tat = df['Turnaround'].mean()
        ax.text(0.99, 0.95, f"Avg WT: {avg_wt:.2f}\nAvg TAT: {avg_tat:.2f}",
            ha='right', va='top', transform=ax.transAxes,
            fontsize=10, fontweight='bold', bbox=dict(facecolor='white', alpha=0.7, edgecolor='black'))

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    csv_files = {
        "FCFS": "output_fcfs.csv",
        "SRTF": "output_sjf.csv",
        "RR": "gantt_rr.csv",
        "MLFQ": "gantt_mlfq.csv"
    }

    while True:
        print("\nAvailable algorithms:")
        for i, algo in enumerate(csv_files.keys(), 1):
            print(f"{i}. {algo}")
        print(f"{len(csv_files)+1}. Exit")

        try:
            choice = int(input("Enter choice number: "))
        except ValueError:
            print("⚠️ Invalid input. Enter a number.")
            continue

        if choice == len(csv_files)+1:
            print("Exiting...")
            break
        elif 1 <= choice <= len(csv_files):
            algo = list(csv_files.keys())[choice - 1]
            plot_gantt_timeline(csv_files[algo], algo)
        else:
            print("⚠️ Invalid choice. Try again.")

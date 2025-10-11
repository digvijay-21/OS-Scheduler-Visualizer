#include "mlfq.h"
#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>  // for CSV export

MLFQ::MLFQ(int numQueues, int baseQuantum) {
    this->numQueues = numQueues;
    this->baseQuantum = baseQuantum;
}

void MLFQ::schedule(std::vector<Process>& processes) {
    int n = processes.size();
    if (n == 0) return;

    // Sort by arrival
    std::sort(processes.begin(), processes.end(),
              [](const Process &a, const Process &b){ return a.arrival_time < b.arrival_time; });

    std::vector<int> remaining(n);
    for (int i = 0; i < n; ++i) remaining[i] = processes[i].burst_time;

    // Create multiple queues
    std::vector<std::queue<int>> queues(numQueues);

    int current_time = 0, completed = 0, idx = 0;
    std::vector<GanttEntry> gantt;  // record execution timeline

    // Enqueue initially arrived processes
    if (processes[0].arrival_time > current_time)
        current_time = processes[0].arrival_time;

    while (idx < n && processes[idx].arrival_time <= current_time)
        queues[0].push(idx++);

    while (completed < n) {
        bool anyProcess = false;

        for (int level = 0; level < numQueues; ++level) {
            if (queues[level].empty()) continue;

            anyProcess = true;
            int i = queues[level].front();
            queues[level].pop();

            int quantum = baseQuantum * (1 << level); // exponential increase: 2,4,8 etc.
            int timeSlice = std::min(quantum, remaining[i]);

            int start_time = current_time;
            remaining[i] -= timeSlice;
            current_time += timeSlice;
            int end_time = current_time;

            // record Gantt entry
            gantt.push_back({processes[i].pid, start_time, end_time});

            // Enqueue newly arrived processes
            while (idx < n && processes[idx].arrival_time <= current_time)
                queues[0].push(idx++);

            if (remaining[i] == 0) {
                processes[i].completion_time = current_time;
                completed++;
            } else {
                // Not finished: move to lower queue (if exists)
                if (level + 1 < numQueues)
                    queues[level + 1].push(i);
                else
                    queues[level].push(i); // stay in same queue (lowest level FCFS)
            }

            break; // after handling one process, restart from top queue
        }

        if (!anyProcess) {
            // CPU idle: fast-forward
            if (idx < n) {
                current_time = processes[idx].arrival_time;
                while (idx < n && processes[idx].arrival_time <= current_time)
                    queues[0].push(idx++);
            }
        }
    }

    // Calculate metrics
    for (auto &p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }

    // Export results CSV
    exportToCSV(processes, "output_mlfq.csv");

    // Export Gantt timeline CSV with Waiting and Turnaround
    std::ofstream fout("gantt_mlfq.csv");
    fout << "PID,Start,End,Waiting,Turnaround\n";
    for (auto &g : gantt) {
        auto &p = processes[g.pid - 1];  // assuming PID starts from 1
        fout << g.pid << "," << g.start << "," << g.end
             << "," << p.waiting_time << "," << p.turnaround_time << "\n";
    }
    fout.close();

    std::cout << "✅ Exported MLFQ Gantt timeline with WT/TAT to gantt_mlfq.csv\n";
}

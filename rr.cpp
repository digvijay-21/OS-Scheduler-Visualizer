#include "rr.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>  // for CSV export

void RR::schedule(std::vector<Process>& processes) {
    int n = processes.size();
    if (n == 0) return;

    // Local copy sorted by arrival time
    std::vector<Process> proc = processes;
    std::sort(proc.begin(), proc.end(),
              [](const Process &a, const Process &b){ 
                  if (a.arrival_time == b.arrival_time) return a.pid < b.pid;
                  return a.arrival_time < b.arrival_time; 
              });

    std::vector<int> remaining(n);
    for (int i = 0; i < n; ++i) remaining[i] = proc[i].burst_time;

    std::queue<int> q;
    int idx = 0;            // next process to enqueue
    int completed = 0;
    int current_time = 0;

    std::vector<GanttEntry> gantt;  // use shared GanttEntry

    // CPU idle at start
    if (proc[0].arrival_time > current_time) current_time = proc[0].arrival_time;

    // enqueue all initial arrivals
    while (idx < n && proc[idx].arrival_time <= current_time) q.push(idx++);

    // RR main loop
    while (completed < n) {
        if (q.empty()) {
            if (idx < n) {
                current_time = proc[idx].arrival_time;
                while (idx < n && proc[idx].arrival_time <= current_time) q.push(idx++);
            }
            continue;
        }

        int i = q.front(); q.pop();

        int time_spent = std::min(quantum, remaining[i]);
        int start_time = current_time;
        remaining[i] -= time_spent;
        current_time += time_spent;
        int end_time = current_time;

        // record Gantt entry
        gantt.push_back({proc[i].pid, start_time, end_time});

        // enqueue new arrivals during execution
        while (idx < n && proc[idx].arrival_time <= current_time) q.push(idx++);

        if (remaining[i] == 0) {
            proc[i].completion_time = current_time;
            completed++;
        } else {
            q.push(i);
        }
    }

    // Map results back to original vector and calculate metrics
    for (const auto &p_sorted : proc) {
        for (auto &orig : processes) {
            if (orig.pid == p_sorted.pid) {
                orig.completion_time = p_sorted.completion_time;
                orig.turnaround_time = orig.completion_time - orig.arrival_time;
                orig.waiting_time = orig.turnaround_time - orig.burst_time;
                break;
            }
        }
    }

    // Export results CSV
    exportToCSV(processes, "output_rr.csv");

    // Export Gantt timeline CSV with Waiting and Turnaround
    std::ofstream fout("gantt_rr.csv");
    fout << "PID,Start,End,Waiting,Turnaround\n";
    for (auto &g : gantt) {
        auto &p = processes[g.pid - 1]; // PID starts at 1
        fout << g.pid << "," << g.start << "," << g.end
             << "," << p.waiting_time << "," << p.turnaround_time << "\n";
    }
    fout.close();

    std::cout << "✅ Exported RR Gantt timeline with WT/TAT to gantt_rr.csv\n";
}

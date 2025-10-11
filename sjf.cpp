#include "sjf.h"
#include <algorithm>
#include <vector>
#include <limits>
#include <iostream>
using namespace std;

void SJF::schedule(std::vector<Process>& processes) {
    int n = processes.size();
    std::vector<int> remaining(n);
    std::vector<int> start_time(n, -1);
    std::vector<bool> finished(n, false);

    for(int i=0;i<n;i++) remaining[i] = processes[i].burst_time;

    int completed = 0;
    int current_time = 0;

    while(completed < n){
        int idx = -1;
        int min_remain = std::numeric_limits<int>::max();

        // Pick process with shortest remaining time that has arrived
        for(int i=0;i<n;i++){
            if(!finished[i] && processes[i].arrival_time <= current_time && remaining[i] < min_remain){
                min_remain = remaining[i];
                idx = i;
            }
        }

        if(idx == -1){
            current_time++; // CPU idle
            continue;
        }

        if(start_time[idx] == -1) start_time[idx] = current_time; // first time CPU executes this process

        remaining[idx]--;
        current_time++;

        if(remaining[idx] == 0){
            processes[idx].completion_time = current_time;
            processes[idx].waiting_time = current_time - processes[idx].arrival_time - processes[idx].burst_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            finished[idx] = true;
            completed++;
        }
    }

    exportToCSV(processes, "output_sjf.csv");
}

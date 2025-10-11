#include "fcfs.h"
#include <algorithm>
using namespace std;

void FCFS::schedule(vector<Process>& processes) {
    sort(processes.begin(), processes.end(),[](Process a, Process b){
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    for(auto& p : processes){
        if(current_time < p.arrival_time){
            current_time = p.arrival_time;
        }
        current_time += p.burst_time;
        p.completion_time = current_time;
    }

    calculateMetrics(processes);
    exportToCSV(processes, "output_fcfs.csv");

}


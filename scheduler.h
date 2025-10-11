#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
};

struct GanttEntry {
    int pid;
    int start;
    int end;
};

class Scheduler {
public:
    virtual void schedule(vector<Process>& processes) = 0; // pure virtual (abstract)
    void calculateMetrics(vector<Process>& processes);
    void exportToCSV(const vector<Process>& processes, const string& filename);
};

#endif
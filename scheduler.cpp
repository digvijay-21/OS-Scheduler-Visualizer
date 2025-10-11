#include "scheduler.h"
#include <fstream>
using namespace std;

void Scheduler::calculateMetrics(vector<Process>& processes){
    for(auto& p : processes){
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void Scheduler::exportToCSV(const vector<Process>&processes, const string& filename){
    ofstream file(filename);
    file << "PID,Arrival,Burst,Completion,Waiting,Turnaround\n";
    for(auto& p : processes){
        file << p.pid <<","<< p.arrival_time <<","<< p.burst_time <<","<< p.completion_time <<"," << p.waiting_time <<"," << p.turnaround_time<<"\n";
    }
    file.close();
    cout<<"\n Exported to "<<filename<<endl;
}
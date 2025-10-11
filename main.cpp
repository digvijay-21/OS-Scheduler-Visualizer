// main.cpp
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

#include "scheduler.h"
#include "fcfs.h"
#include "sjf.h"   // SRTF implementation
#include "rr.h"
#include "mlfq.h"

using namespace std;

// Helper: read CSV produced by schedulers and print a summary (and averages)
void printCSVSummary(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "  [!] Could not open " << filename << " (was it created?)\n";
        return;
    }

    string header;
    if (!getline(file, header)) {
        cout << "  [!] Empty file: " << filename << "\n";
        return;
    }

    // Print readable table header
    cout << "PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n";

    string line;
    double sumWait = 0.0, sumTurn = 0.0;
    int rows = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> cols;
        while (getline(ss, token, ',')) cols.push_back(token);

        if (cols.size() < 6) continue; // safety
        cout << cols[0] << "\t"   // PID
             << cols[1] << "\t"   // Arrival
             << cols[2] << "\t"   // Burst
             << cols[3] << "\t\t" // Completion
             << cols[4] << "\t"   // Waiting
             << cols[5] << "\n";  // Turnaround

        sumWait += stod(cols[4]);
        sumTurn += stod(cols[5]);
        rows++;
    }

    if (rows > 0) {
        cout << "Average Waiting Time: " << (sumWait / rows) << "\n";
        cout << "Average Turnaround Time: " << (sumTurn / rows) << "\n";
    }

    file.close();
}

int main() {
    // sample process list (pid, arrival_time, burst_time, waiting_time, turnaround_time, completion_time)
    vector<Process> processes = {
        {1, 0, 6, 0, 0, 0},
        {2, 2, 4, 0, 0, 0},
        {3, 3, 8, 0, 0, 0},
        {4, 5, 3, 0, 0, 0},
        {5, 6, 10, 0, 0, 0},
        {6, 8, 2, 0, 0, 0},
        {7, 12, 5, 0, 0, 0}
    };

    cout << "===== Scheduler Runner =====\n\n";

    struct Alg {
        string name;
        shared_ptr<Scheduler> sched;
        string csvFile;
    };

    vector<Alg> algs;
    algs.push_back({"FCFS", make_shared<FCFS>(),      "output_fcfs.csv"});
    algs.push_back({"SRTF (preemptive SJF)", make_shared<SJF>(), "output_sjf.csv"});
    algs.push_back({"RR (quantum=3)", make_shared<RR>(3), "output_rr.csv"});
    algs.push_back({"MLFQ", make_shared<MLFQ>(),      "output_mlfq.csv"});

    for (auto &a : algs) {
        cout << ">>> Running: " << a.name << " <<<\n";

        // pass a fresh copy to scheduler (so each run starts from same initial state)
        vector<Process> pcopy = processes;
        a.sched->schedule(pcopy);

        // print CSV summary (scheduler implementations export CSV)
        cout << "\nResults from " << a.csvFile << ":\n";
        printCSVSummary(a.csvFile);
        cout << "----------------------------------------\n\n";
    }

    cout << "All schedulers finished.\n";
    return 0;
}

#ifndef FCFS_H
#define FCFS_H
#include "scheduler.h"
using namespace std;

class FCFS : public Scheduler {
    public:
        void schedule(vector<Process>& processes) override;
};

#endif

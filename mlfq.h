#ifndef MLFQ_H
#define MLFQ_H

#include "scheduler.h"

class MLFQ : public Scheduler {
public:
    int numQueues;
    int baseQuantum;

    MLFQ(int numQueues = 3, int baseQuantum = 2);
    void schedule(std::vector<Process>& processes) override;
};

#endif

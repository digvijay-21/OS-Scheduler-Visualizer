#ifndef RR_H
#define RR_H

#include "scheduler.h"

class RR : public Scheduler {
    public:
        int quantum;
        RR(int q) : quantum(q) {}  // initializer list to set quantum
        void schedule(std::vector<Process>& processes) override;
};

#endif
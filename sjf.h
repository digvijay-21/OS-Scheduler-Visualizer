#ifndef SJF_H
#define SJF_H
#include "scheduler.h"

class SJF : public Scheduler {
    public:
        void schedule(std::vector<Process>& processes) override;
};

#endif 
//
// Created by lwd on 19-4-8.
//

#ifndef MEC_GREEDY1_H
#define MEC_GREEDY1_H
#include "solution.h"
#include "scheduler.h"
class Greedy{
public:
    Greedy() = default;
    void run(const Dag& dag, const System& sys, int rank = 0);
    void run_with_sched(const Dag& dag, const System& sys, Scheduler& scheduler, int rank = 0); //do_not_use
private:
    void initialize(const Dag &dag, const System &sys, Scheduler& scheduler);
    void assign(const shared_ptr<Node> &node, const vector<shared_ptr<Node>> &nodes, const vector<shared_ptr<Processor>> &processors, Scheduler& scheduler);
};
#endif //MEC_GREEDY1_H

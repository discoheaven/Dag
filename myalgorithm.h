//
// Created by komine on 2019/3/14.
//

#ifndef MEC_ALGORITHM_H
#define MEC_ALGORITHM_H
#include "solution.h"
#include "scheduler.h"
class MyAlgo{
public:
    MyAlgo() = default;
    void run(const Dagger& dag, const System& sys, Scheduler& scheduler, int rank = 0);
private:
    void initialize(const Dagger& dag, const System& sys, Scheduler& scheduler, int rank);
    vector<shared_ptr<Node>> find_critical_path(const vector<shared_ptr<Node>> &nodes) const;
    void migrate(const shared_ptr<Node> &node, const vector<shared_ptr<Node>> &nodes, const vector<shared_ptr<Processor>> &processors, Scheduler& scheduler);
    void reset(const Dagger &dag, const System &sys);
    static const int grasp_iteration = 1;
};
#endif //MEC_ALGORITHM_H

//
// Created by lwd on 19-6-14.
//

#ifndef MEC_MYALGORITHM_TABU_H
#define MEC_MYALGORITHM_TABU_H
#include "solution.h"
#include "scheduler.h"

class MyAlgoTabu{
public:
    MyAlgoTabu() = default;
    void run(const Dag& dag, const System& sys);
private:
    void initialize(const Dag& dag, const System& sys, Scheduler& scheduler);
    vector<pair<int,int>> get_avail_slots(const shared_ptr<Processor>& processor, const shared_ptr<Node>& node);
    static const int tabu_length = 5;
    static const int iterations = 50;
};
#endif //MEC_MYALGORITHM_TABU_H

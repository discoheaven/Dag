//
// Created by komine on 2019/2/27.
//

#ifndef MEC_SOLUTION_H
#define MEC_SOLUTION_H

#include <vector>
#include "scheduler.h"

using std::vector;
class Solution{
public:
    explicit Solution(const Scheduler& sc);
    double get_time() const
    {
        return time;
    }
    double get_energy_cost() const
    {
        return e_cost;
    }
    void print() const
    {
        cout << "Time Cost: " << time << "\n" << "Energy Cost: " << e_cost << endl;
        cout << "**********" << endl;
    }
    void print_solution() const;
private:
    void calTime(const Scheduler& scheduler);
    void calCost(const Scheduler& scheduler);
    double calc_process_time(const shared_ptr<Node>& node)
    {
        return node->get_workload() / node->get_processor()->get_freq();
    }
    map<int,vector<vector<int>>> schedule;
    map<int,double> ests;
    double time;
    double e_cost;
};
#endif //MEC_SOLUTION_H

//
// Created by komine on 2019/3/13.
//
#include "solution.h"
#include "dummy_node.h"
Solution::Solution(const Scheduler& sc)
{
    for(auto &item : sc.get_nodes())
        ests.insert({item->get_id(),item->get_est()});
    for(auto &item : sc.get_processors())
    {
        schedule.insert({item->get_id(),{}});
        for(auto &task : item->get_all_tasks()) {
            vector<int> tsks;
            for(auto &tsk : task)
                tsks.push_back(tsk->get_id());
            schedule[item->get_id()].push_back(tsks);
        }
    }
    calTime(sc);
    calCost(sc);
}
void Solution::calTime(const Scheduler& sc)
{
    double max = 0;
    for(auto &item : sc.get_nodes())
    {
        item->set_finish_time(item->get_est() + calc_process_time(item));
        if(item->get_finish_time() > max)
            max = item->get_finish_time();
    }
    time = max;
}
void Solution::calCost(const Scheduler& sc)
{
    double tcost = 0;
    for(auto &item : sc.get_nodes())
    {
        auto processor = item->get_processor();
        if(processor->isMEC() || processor->isUser())
            tcost += pow(10,-11) * processor->get_freq() * processor->get_freq() * item->get_workload();
    }
    e_cost = tcost;
}

void Solution::print_solution() const
{
    cout << "**********************\n";
    for(auto &item : schedule)
    {
        cout << "processor " << item.first << ":\n";
        for(int i = 0;i < item.second.size();++i)
        {
            cout << "core " << i << ": ";
            for (auto &node : item.second[i])
                cout << node << "(est:" << ests.at(node) << ") ";
            cout << "\n";
        }
        cout << endl;
    }
    cout << "**********************\n";
}


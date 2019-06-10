//
// Created by LWD on 2019/3/14.
//
#include <queue>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include "myalgorithm.h"
#include "greedy.h"
#include "DefaultNodeSelection.h"
#include "DefaultMigrateMetric.h"
using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::default_random_engine;
using std::max_element;
using std::unordered_map;
void MyAlgo::initialize(const Dag &dag, const System &sys, Scheduler &scheduler, int rank)
{
    Greedy g = Greedy();
    g.run_with_sched(dag,sys,scheduler,rank);
    scheduler.update_lst();
    Solution initial_solution(scheduler);
    cout << "Initial Solution:\n";
    initial_solution.print();
    initial_solution.print_solution();
}
void MyAlgo::run(const Dag& dag, const System& sys, int rank)
{
    auto nodes = dag.get_nodes();
    auto edges = dag.get_edges();
    auto processors = sys.get_processors();
    auto scheduler = Scheduler(dag, sys);
    auto node_selection_ = DefaultNodeSelectionPolicy::get_policy();
    vector<Solution> solution_set_outer;
    for(int io = 0;io < grasp_iteration;++io) {
        reset(dag, sys);
        initialize(dag, sys, scheduler, io % processors.size());
        vector<Solution> solution_set_inner;
        for (int i = 0; i < 30; ++i) {
            auto critical_path = find_critical_path(nodes);

            while (!critical_path.empty()) {
                auto node = node_selection_->select(critical_path);
                if (!node->is_fixed_processor()) {
                    migrate(node, nodes, processors, scheduler);
                    scheduler.update();
                }
                auto loc = std::find(critical_path.begin(), critical_path.end(), node);
                assert(loc != critical_path.end());
                critical_path.erase(loc);
            }
            Solution new_solution(scheduler);
            solution_set_inner.push_back(new_solution);
        }
        cout << "Best Solution:\n";
        Solution best_solution = *min_element(solution_set_inner.begin(), solution_set_inner.end(),
                                              [](const Solution &a, const Solution &b) {
                                                  return a.get_time() < b.get_time();
                                              });
        best_solution.print();
        best_solution.print_solution();
        solution_set_outer.push_back(best_solution);
    }
    Solution best_solution = *min_element(solution_set_outer.begin(), solution_set_outer.end(),
                                          [](const Solution &a, const Solution &b) {
                                              return a.get_time() < b.get_time();
                                          });
    //cout << "Final Best Solution:\n";
    //best_solution.print_solution();
}
vector<shared_ptr<Node>> MyAlgo::find_critical_path(const vector<shared_ptr<Node>> &nodes) const
{
    vector<shared_ptr<Node>> res;
    double min = INT_MAX; //fixme
    for(auto &item : nodes)
    {
        if(item->is_scheduled() && item->get_lst() - item->get_est() < min)
            min = item->get_lst() - item->get_est();
    }
    double left = 0.999999 * min;
    double right = 1.000001 * min;
    for(auto &item : nodes)
    {
        if(item->is_scheduled() && (item->get_lst() - item->get_est() >= left && item->get_lst() - item->get_est() <= right))
            res.push_back(item);
    }
    return res;
}

void MyAlgo::migrate(const shared_ptr<Node> &node, const vector<shared_ptr<Node>> &nodes,
                      const vector<shared_ptr<Processor>> &processors, Scheduler& scheduler)
{
    static MigrateMetric *migrate_metric = DefaultMigrateMetric::get_metric();
    auto old_processor = node->get_processor();
    auto old_index = old_processor->retrieve_index(node);
    assert(old_index != -1);
    migrate_metric->initial(node);
    auto min_processor = old_processor;
    auto min_index = -1;
    for(auto &new_processor : processors)
    {
        if(new_processor == old_processor)
            continue;
        auto temp = new_processor->get_tasks();
        unordered_map<shared_ptr<Node>,int> new_processor_tasks;
        for(int i = 0;i < temp.size();++i)
            new_processor_tasks.insert({temp[i],i});

        int father_max = 0;
        for(auto &item : node->get_all_fathers())
        {
            auto sitem = item.lock();
            auto loc = new_processor_tasks.find(sitem);
            if(loc != new_processor_tasks.end() && (loc->second + 1) > father_max)
                father_max = loc->second + 1;
        }

        auto child_min = static_cast<int>(temp.size());
        for(auto &item : node->get_all_children())
        {
            auto sitem = item.lock();
            auto loc = new_processor_tasks.find(sitem);
            if(loc != new_processor_tasks.end() && loc->second < child_min)
                child_min = loc->second;
        }

        assert(father_max <= child_min);

        scheduler.set_schedule_at(node,new_processor,child_min);
        scheduler.update();
        if(migrate_metric->better(node,scheduler))
        {
            min_processor = new_processor;
            min_index = child_min;
        }
    }
    if(min_index == -1)
        scheduler.set_schedule_at(node,old_processor,old_index);
    else
        scheduler.set_schedule_at(node,min_processor,min_index);
}

void MyAlgo::reset(const Dag &dag, const System &sys)
{
    for(auto &item : dag.get_nodes())
    {
        item->reset();
    }
    for(auto &item : sys.get_processors())
    {
        item->reset();
    }
}

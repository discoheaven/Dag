//
// Created by lwd on 19-4-8.
//
#include <queue>
#include "greedy.h"
using std::queue;
using std::multimap;

void Greedy::initialize(const Dag &dag, const System &sys, Scheduler& scheduler)
{
    auto initial_processor = sys.get_dag_initial_processor();
    auto nodes = dag.get_nodes();
    auto begin_dummy_node = nodes.front();
    auto end_dummy_node = nodes.back();
    scheduler.set_schedule(begin_dummy_node,initial_processor);
    scheduler.set_schedule(end_dummy_node,initial_processor);
}

void Greedy::assign(const shared_ptr<Node> &node, const vector<shared_ptr<Node>> &nodes,
                     const vector<shared_ptr<Processor>> &processors, Scheduler &scheduler)
{
    auto end_dummy_node = nodes.back();
    double min_est = LONG_MAX; //fixme
    auto min_processor = node->get_processor();
    auto min_index = -1;
    for(auto &processor : processors)
    {
        auto index = processor->retrieve_index(end_dummy_node);
        scheduler.set_schedule_at(node,processor,index);
        auto node_est = scheduler.calc_est(node);
        if(node_est < min_est)
        {
            min_est = node_est;
            min_processor = processor;
            min_index = index;
        }
    }
    scheduler.set_schedule_at(node,min_processor,min_index);
}


void Greedy::run_with_sched(const Dag &dag, const System &sys, Scheduler &scheduler, int rank)
{
    initialize(dag, sys, scheduler);

    auto nodes = dag.get_nodes();
    auto edges = dag.get_edges();
    auto processors = sys.get_processors();

    queue<shared_ptr<Node>> bfs;
    for(auto &item : nodes)
    {
        item->reset_cnts();
        if(item->ready_for_est_update())
            bfs.push(item);
    }

    while(!bfs.empty())
    {
        auto cur = bfs.front();
        bfs.pop();
        if(!cur->is_fixed_processor()) {
            assign(cur, nodes, processors, scheduler);
        }
        auto cur_est = scheduler.calc_est(cur);
        cur->set_est(cur_est);
        auto children = cur->get_children();
        for(auto &item : children)
        {
            auto sitem = item.lock();
            sitem->dec_pcnt();
            if(sitem->ready_for_est_update())
                bfs.push(sitem);
        }
    }
}
void Greedy::run(const Dag &dag, const System &sys, int rank)
{
    auto scheduler = Scheduler(dag, sys);
    run_with_sched(dag, sys, scheduler, rank);
    Solution solution(scheduler);
    solution.print();
    solution.print_solution();
}

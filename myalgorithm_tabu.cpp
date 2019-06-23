//
// Created by 刘卫东 on 2019-06-17.
//

#include <algorithm>
#include <map>
#include "greedy.h"
#include "myalgorithm_tabu.h"

using std::map;
using std::remove_if;
using std::min_element;
using std::pair;

void MyAlgoTabu::initialize(const Dag &dag, const System &sys, Scheduler &scheduler)
{
    Greedy g = Greedy();
    g.run_with_sched(dag,sys,scheduler);
    scheduler.update_lst();
    Solution initial_solution(scheduler);
    cout << "Initial Solution:\n";
    initial_solution.print();
    initial_solution.print_solution();
}

void MyAlgoTabu::run(const Dag &dag, const System &sys)
{
    map<shared_ptr<Node>, int> tabu_list;
    auto nodes = dag.get_nodes();
    auto processors = sys.get_processors();
    auto scheduler = Scheduler(dag, sys);
    default_random_engine e;
    vector<Solution> solutions;

    initialize(dag,sys,scheduler);

    for(int i=0;i<iterations;++i)
    {
        auto node = nodes[e()%nodes.size()];
        if(tabu_list.find(node) != tabu_list.end())
        {
            --i;
            continue;
        }

        for(auto &item : tabu_list)
            --item.second;
        auto new_end = remove_if(tabu_list.begin(),tabu_list.end(),[](const pair<const shared_ptr<Node>, int> &a){return a.second == 0;});
        tabu_list.erase(new_end, tabu_list.end());
        tabu_list.insert({node,tabu_length});

        map<pair<shared_ptr<Processor>,pair<int,int>>, Solution> inner_solutions;
        for(const auto &processor : processors)
        {
            auto avail_slots = get_avail_slots(processor, node);
            auto slot = avail_slots[e()%avail_slots.size()];
            scheduler.set_schedule_at(node,processor,slot);
            scheduler.update();
            inner_solutions.insert({{processor,slot},Solution(scheduler)});
        }
        auto best_item = *min_element(inner_solutions.begin(),inner_solutions.end(),[](const pair<const pair<shared_ptr<Processor>,pair<int,int>>, Solution> &a,
                const pair<const pair<shared_ptr<Processor>,pair<int,int>>, Solution> &b){return a.second.get_time() < b.second.get_time();});
        solutions.push_back(best_item.second);
        scheduler.set_schedule_at(node, best_item.first.first, best_item.first.second);
        scheduler.update();
    }
    auto best_solution = *min_element(solutions.begin(),solutions.end(),[](const Solution &a, const Solution &b){return a.get_time() < b.get_time();});
    best_solution.print();
    best_solution.print_solution();
}

vector<pair<int,int>> MyAlgoTabu::get_avail_slots(const shared_ptr<Processor> &processor, const shared_ptr<Node> &node)
{
    if(processor->isCloud())
        return {processor->get_avail_index()};

    vector<pair<int,int>> res;

    int core_size = processor->get_core_size();

    for(int core = 0;core < core_size;++core)
    {
        int father_max = -1;
        for (auto &item : node->get_all_fathers()) {
            auto sitem = item.lock();
            auto index = processor->get_node_index(sitem, core);
            if (index != -1 && index > father_max)
                father_max = index;
        }

        auto child_min = static_cast<int>(processor->get_core_tasks_size(core));
        for (auto &item : node->get_all_children()) {
            auto sitem = item.lock();
            auto index = processor->get_node_index(sitem, core);
            if (index != -1 && index < child_min)
                child_min = index;
        }

        assert(father_max < child_min);

        for(int index = father_max;index<child_min;++index)
        {
            res.emplace_back(core, index + 1);
        }
    }
    return res;
}
//
// Created by 刘卫东 on 2019-03-30.
//
#include <queue>
#include "common.h"
#include "scheduler.h"
using std::queue;

double Scheduler::calc_est_processors(const shared_ptr<Node> &node)
{
    auto processor = node->get_processor();
    auto prev_task = processor->get_prev_task(node);
    if(prev_task == nullptr)
        return 0;
    auto prev_task_est =  prev_task->get_est();
    auto prev_task_exec_time = processor->process_time(prev_task);
    return prev_task_est + prev_task_exec_time;
}

double Scheduler::get_comm_time(double transload, const shared_ptr<Node> &node1, const shared_ptr<Node> &node2)  const
{
    auto processor1 = node1->get_processor();
    auto processor2 = node2->get_processor();
    auto rate = get_comm_rate(processor1,processor2);
    if(rate == 0)
        return 0;
    double time = 0;
    if(processor1->get_type() == Processor::ClOUD || processor2->get_type() == Processor::ClOUD)
        time = cloud_delay;
    return time + transload / rate;
}

double Scheduler::get_comm_rate(const shared_ptr<Processor> &processor1, const shared_ptr<Processor> &processor2)  const
{
    if(processor1 == processor2)
        return 0;
    else if(processor1->get_type() == processor2->get_type())
        return 10 * CCR;
    else if((processor1->isUser() && processor2->isMEC()) || (processor1->isMEC() && processor2->isUser()))
        return 8 * CCR;
    else if((processor1->isUser() && processor2->isCloud()) || (processor1->isCloud() && processor2->isUser()))
        return 5 * CCR;
    else
        return 5 * CCR;
}

double Scheduler::calc_est_parents(const shared_ptr<Node> &node)
{
    auto fathers = node->get_fathers();
    double max = 0;
    for(auto &item : fathers)
    {
        auto sitem = item.lock();
        const pair<int,int> key(sitem->get_id(),node->get_id());
        assert(edges.find(key) != edges.end());
        auto transload = edges[key];
        auto transtime = get_comm_time(transload, sitem, node);
        auto cur_est = sitem->get_est() + sitem->get_processor()->process_time(sitem) + transtime;
        if(cur_est > max)
            max = cur_est;
    }
    return max;
}

double Scheduler::calc_est(const shared_ptr<Node> &node)
{
    auto est_parents = calc_est_parents(node);
    auto est_processors = calc_est_processors(node);
    auto cur_est = std::max(est_parents,est_processors);
    if(cur_est > deadline)
    {
        cout << "can't finish the job,exiting..." << endl;
        exit(1);
    }
    return cur_est;
}


double Scheduler::calc_lst_processors(const shared_ptr<Node> &node)
{
    auto processor = node->get_processor();
    auto next_task = processor->get_next_task(node);
    auto next_task_lst = next_task == nullptr ? deadline : next_task->get_lst();
    auto cur_process_time = processor->process_time(node);
    return next_task_lst - cur_process_time;
}


double Scheduler::calc_lst_parents(const shared_ptr<Node> &node)
{
    auto childrens = node->get_children();
    auto cur_process_time = node->get_processor()->process_time(node);
    double min = deadline;
    for(auto &item : childrens)
    {
        auto sitem = item.lock();
        const pair<int,int> key(node->get_id(),sitem->get_id());
        assert(edges.find(key) != edges.end());
        auto transload = edges[key];
        auto transtime = get_comm_time(transload, sitem, node);
        auto cur_lst = sitem->get_lst() - cur_process_time - transtime;
        if(cur_lst < min)
            min = cur_lst;
    }
    return min;
}

double Scheduler::calc_lst(const shared_ptr<Node> &node)
{
    auto lst_parents = calc_lst_parents(node);
    auto lst_processors = calc_lst_processors(node);
    auto cur_lst = std::min(lst_parents,lst_processors);
    if(cur_lst < 0)
    {
        cout << "can't finish the job,exiting..." << endl;
        exit(1);
    }
    return cur_lst;
}

void Scheduler::update()
{
    update_est();
    update_lst();
}

void Scheduler::update_est()
{
    vector<shared_ptr<Node>> all_nodes = nodes;
    queue<shared_ptr<Node>> est_ready_nodes;

    for(auto it = all_nodes.begin();it != all_nodes.end();)
    {
        auto item = *it;
        item->reset_cnts();
        if(ready_for_est_update(item))
        {
            it = all_nodes.erase(it);
            est_ready_nodes.push(item);
        }
        else
        {
            ++it;
        }
    }


    while(!est_ready_nodes.empty())
    {
        auto cur = est_ready_nodes.front();
        est_ready_nodes.pop();
        cur->set_est(calc_est(cur));
        auto children = cur->get_children();
        for(auto &item : children)
        {
            auto sitem = item.lock();
            sitem->dec_pcnt();
        }

        for(auto it = all_nodes.begin();it != all_nodes.end();)
        {
            auto item = *it;
            if(ready_for_est_update(item))
            {
                it = all_nodes.erase(it);
                est_ready_nodes.push(item);
            }
            else
            {
                ++it;
            }
        }
    }
}
void Scheduler::update_lst()
{
    vector<shared_ptr<Node>> all_nodes = nodes;
    queue<shared_ptr<Node>> lst_ready_nodes;

    for(auto it = all_nodes.begin();it != all_nodes.end();)
    {
        auto item = *it;
        item->reset_cnts();
        if(ready_for_lst_update(item))
        {
            it = all_nodes.erase(it);
            lst_ready_nodes.push(item);
        }
        else
        {
            ++it;
        }
    }

    while(!lst_ready_nodes.empty())
    {
        auto cur = lst_ready_nodes.front();
        lst_ready_nodes.pop();
        cur->set_lst(calc_lst(cur));
        auto fathers = cur->get_fathers();
        for(auto &item : fathers)
        {
            auto sitem = item.lock();
            sitem->dec_ccnt();
        }

        for(auto it = all_nodes.begin();it != all_nodes.end();)
        {
            auto item = *it;
            if(ready_for_lst_update(item))
            {
                it = all_nodes.erase(it);
                lst_ready_nodes.push(item);
            }
            else
            {
                ++it;
            }
        }

    }
}

void Scheduler::set_schedule(const shared_ptr<Node> &node, const shared_ptr<Processor> &processor)
{
    auto index = processor->get_avail_index();
    set_schedule_at(node,processor,index);
}

void Scheduler::set_schedule_at(const shared_ptr<Node> &node, const shared_ptr<Processor> &processor, pair<int,int> index)
{
    if(node->is_scheduled())
    {
        //if(node->is_fixed_processor())
            //return;
        auto last_processor = node->get_processor();
        last_processor->erase(node);
    }
    node->set_processor({processor, index.first});
    processor->insert_at(node,index);
}

bool Scheduler::ready_for_est_update(const shared_ptr<Node> &node)
{
    if(node->ready_for_est_update())
    {
        auto processor = node->get_processor();
        auto prev_task = processor->get_prev_task(node);
        return !prev_task || prev_task->est_set();
    }
    return false;
}

bool Scheduler::ready_for_lst_update(const shared_ptr<Node> &node)
{
    if(node->ready_for_lst_update())
    {
        auto processor = node->get_processor();
        auto next_task = processor->get_next_task(node);
        return !next_task || next_task->lst_set();
    }
    return false;
}

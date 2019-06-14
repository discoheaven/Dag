//
// Created by lwd on 2019/3/11.
//
#include "processor.h"

void Processor::insert_at(const shared_ptr<Node> &node,pair<int,int> index)
{
    assert(index.first < core_size_);
    assert(index.second >= 0);
    if(index.second > tasks[index.first].size())
    {
        int a = 1;
    }
    assert(index.second <= tasks[index.first].size());
    tasks[index.first].insert(tasks[index.first].begin()+index.second,node);
}

pair<int,int> Processor::retrieve_index(const shared_ptr<Node> &node)
{
    for(int i = 0;i < core_size_;++i)
    {
        auto loc = std::find(tasks[i].begin(), tasks[i].end(), node);
        if(loc != tasks[i].end())
            return {i, loc-tasks[i].begin()};
    }
    return get_avail_index();
}

void Processor::erase(const shared_ptr<Node> &node)
{
    assert(node->is_scheduled());
    for(auto &tsk: tasks)
    {
        auto loc = std::find(tsk.begin(), tsk.end(), node);
        if(loc != tsk.end())
        {
            tsk.erase(loc);
            return;
        }
    }
    assert(false);
}

int Processor::get_node_index(const shared_ptr<Node> &node, int core) const
{
    assert(node->is_scheduled());
    assert(core < core_size_);
    auto loc = std::find(tasks[core].begin(),tasks[core].end(), node);
    if(loc != tasks[core].end())
        return loc-tasks[core].begin();
    else
        return -1;
}

shared_ptr<Node> Processor::get_prev_task(const shared_ptr<Node> &node) const
{
    assert(node->is_scheduled());
    auto core = node->get_core();
    for (int i = -1; i <= static_cast<int>(tasks[core].size()) - 1; ++i) {
        if (tasks[core][i + 1] == node)
            return i == -1 ? nullptr : tasks[core][i];
    }
    return nullptr;
}

shared_ptr<Node> Processor::get_next_task(const shared_ptr<Node> &node) const
{
    assert(node->is_scheduled());
    auto core = node->get_core();
    for (int i = 1; i <= static_cast<int>(tasks[core].size()); ++i) {
        if (tasks[core][i - 1] == node)
            return i == static_cast<int>(tasks[core].size()) ? nullptr : tasks[core][i];
    }
    return nullptr;
}

pair<int,int> Processor::get_avail_index() const
{
    int min_num = INT_MAX; //fixme
    int min_core = 0;
    for(int i = 0;i < core_size_;++i)
    {
        if(tasks[i].size() < min_num)
        {
            min_num = tasks[i].size();
            min_core = i;
        }
    }
    return {min_core, min_num};
}

void Processor::reset()
{
    for(auto &tsk : tasks)
        tsk.clear();
}

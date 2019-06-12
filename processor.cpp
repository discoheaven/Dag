//
// Created by lwd on 2019/3/11.
//
#include "processor.h"

void User::insert_at(const shared_ptr<Node> &node,pair<int,int> index)
{
    assert(index.first == 0);
    assert(index.second >= 0 && index.second <= tasks[0].size());
    tasks[0].insert(tasks[0].begin()+index.second,node);
}

pair<int,int> User::retrieve_index(const shared_ptr<Node> &node)
{
    auto loc = std::find(tasks[0].begin(),tasks[0].end(),node);
    return {0, loc - tasks[0].begin()};
}

void User::erase(const shared_ptr<Node> &node)
{
    auto loc = std::find(tasks[0].begin(),tasks[0].end(),node);
    assert(loc != tasks.end());
    tasks[0].erase(loc);
}

shared_ptr<Node> User::get_prev_task(const shared_ptr<Node> &node) const
{
    for(auto &tsk : tasks)
    {
        for (int i = -1; i <= static_cast<int>(tsk.size()) - 1; ++i) {
            if (tsk[i + 1] == node)
                return i == -1 ? nullptr : tsk[i];
        }
    }
    return nullptr;
}

shared_ptr<Node> User::get_next_task(const shared_ptr<Node> &node) const
{
    for(auto &tsk : tasks)
    {
        for (int i = 1; i <= static_cast<int>(tsk.size()); ++i) {
            if (tsk[i - 1] == node)
                return i == static_cast<int>(tsk.size()) ? nullptr : tsk[i];
        }
    }
    return nullptr;
}

void Mec::insert_at(const shared_ptr<Node> &node,pair<int,int> index)
{
    assert(index.first < mec_core_size);
    assert(index.second >= 0 && index.second <= tasks[index.first].size());
    tasks[index.first].insert(tasks[index.first].begin()+index.second,node);
}

pair<int,int> Mec::retrieve_index(const shared_ptr<Node> &node)
{
    for(int i = 0;i < mec_core_size;++i)
    {
        auto loc = std::find(tasks[i].begin(), tasks[i].end(), node);
        if(loc != tasks[i].end())
            return {i, loc-tasks[i].begin()};
    }
    return get_avail_index();
}

void Mec::erase(const shared_ptr<Node> &node)
{
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

shared_ptr<Node> Mec::get_prev_task(const shared_ptr<Node> &node) const
{
    for(auto &tsk : tasks)
    {
        for (int i = -1; i <= static_cast<int>(tsk.size()) - 1; ++i) {
            if (tsk[i + 1] == node)
                return i == -1 ? nullptr : tsk[i];
        }
    }
    return nullptr;
}

shared_ptr<Node> Mec::get_next_task(const shared_ptr<Node> &node) const
{
    for(auto &tsk : tasks)
    {
        for (int i = 1; i <= static_cast<int>(tsk.size()); ++i) {
            if (tsk[i - 1] == node)
                return i == static_cast<int>(tsk.size()) ? nullptr : tsk[i];
        }
    }
    return nullptr;
}

pair<int,int> Mec::get_avail_index() const
{
    int min_num = INT_MAX; //fixme
    int min_core = 0;
    for(int i = 0;i < mec_core_size;++i)
    {
        if(tasks[i].size() < min_num)
        {
            min_num = tasks[i].size();
            min_core = i;
        }
    }
    return {min_core, min_num};
}

void Mec::reset()
{
    for(auto &tsk : tasks)
        tsk.clear();
}

void Cloud::insert_at(const shared_ptr<Node> &node,pair<int,int> index)
{
    assert(index.first <= tasks.size() && index.second == 0);
    tasks.insert(tasks.begin()+index.first, {node});
}

pair<int,int> Cloud::retrieve_index(const shared_ptr<Node> &node)
{
    int i = 0;
    for(;i < tasks.size();++i)
    {
        if(tasks[i][0] == node)
            break;
    }
    return {i,0};
}

void Cloud::erase(const shared_ptr<Node> &node)
{
    auto it = tasks.begin();
    for(;it != tasks.end();++it)
    {
        if(it->at(0) == node)
            tasks.erase(it);
        return;
    }
    assert(false);
}
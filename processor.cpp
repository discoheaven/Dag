//
// Created by komine on 2019/3/11.
//
#include "processor.h"
shared_ptr<Node> User::get_prev_task(const shared_ptr<Node> &node) const
{
    for(int i = -1;i <= static_cast<int>(tasks.size())-1;++i)
    {
        if(tasks[i+1] == node)
            return i == -1 ? nullptr : tasks[i];
    }
    return nullptr;
}

shared_ptr<Node> User::get_next_task(const shared_ptr<Node> &node) const
{
    for(int i = 1;i <= static_cast<int>(tasks.size());++i)
    {
        if(tasks[i-1] == node)
            return i == static_cast<int>(tasks.size()) ? nullptr : tasks[i];
    }
    return nullptr;
}

//
// Created by liuweidong02 on 2019/4/18.
//
#include "MinBESTMigratePolicy.h"
void MinBESTMigrateMetric::initial(const shared_ptr<Node> &node)
{
    min_best = node->get_est();
    for(auto &item : node->get_children())
        min_best += item.lock()->get_est();
}
bool MinBESTMigrateMetric::better(const shared_ptr<Node> &node, const scheduler &sched)
{
    auto best = node->get_est();
    for(auto &item : node->get_children())
        best += item.lock()->get_est();
    if(best < min_best)
    {
        min_best = best;
        return true;
    }
    return false;
}

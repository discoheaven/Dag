//
// Created by liuweidong02 on 2019/4/18.
//
#include "MinEstMigrateMetric.h"
bool MinESTMigrateMetric::better(const shared_ptr<Node> &node, const scheduler &sched)
{
    if(node->get_est() < min_est)
    {
        min_est = node->get_est();
        return true;
    }
    return false;
}

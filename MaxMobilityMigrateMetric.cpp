//
// Created by liuweidong02 on 2019/4/18.
//
#include "MaxMobilityMigrateMetric.h"
void MaxMobilityMigrateMetric::initial(const shared_ptr<Node> &node)
{
    max_mobility = node->get_lst() - node->get_est();
}
bool MaxMobilityMigrateMetric::better(const shared_ptr<Node> &node, const scheduler &sched)
{
    if(node->get_lst() - node->get_est() > max_mobility)
    {
        max_mobility = node->get_lst() - node->get_est();
        return true;
    }
    return false;
}

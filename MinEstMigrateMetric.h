//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_MINESTMIGRATEMETRIC_H
#define MEC_MINESTMIGRATEMETRIC_H
#include "MigrateMetric.h"
class MinESTMigrateMetric : public MigrateMetric{
public:
    void initial(const shared_ptr<Node>& node) override
    {
        min_est = node->get_est();
    }
    bool better(const shared_ptr<Node>& node,const scheduler& sched) override;
private:
    double min_est;
};
#endif //MEC_MINESTMIGRATEMETRIC_H

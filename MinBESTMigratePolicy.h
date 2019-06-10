//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_MINBESTMIGRATEPOLICY_H
#define MEC_MINBESTMIGRATEPOLICY_H
#include "MigrateMetric.h"
class MinBESTMigrateMetric : public MigrateMetric{
public:
    void initial(const shared_ptr<Node>& node) override;
    bool better(const shared_ptr<Node>& node,const scheduler& sched) override;
private:
    double min_best;
};
#endif //MEC_MINBESTMIGRATEPOLICY_H

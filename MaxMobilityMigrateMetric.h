//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_MAXMOBILITYMIGRATEMETRIC_H
#define MEC_MAXMOBILITYMIGRATEMETRIC_H
#include "MigrateMetric.h"
class MaxMobilityMigrateMetric : public MigrateMetric{
public:
    void initial(const shared_ptr<Node>& node) override;
    bool better(const shared_ptr<Node>& node,const scheduler& sched) override;
private:
    double max_mobility;
};
#endif //MEC_MAXMOBILITYMIGRATEMETRIC_H

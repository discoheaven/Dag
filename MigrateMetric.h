//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_MIGRATEMETRIC_H
#define MEC_MIGRATEMETRIC_H
#include <memory>
#include "node.h"
#include "scheduler.h"
using std::shared_ptr;
class MigrateMetric
{
public:
    virtual void initial(const shared_ptr<Node>& node) = 0;
    virtual bool better(const shared_ptr<Node>& node,const scheduler& sched) = 0;
    virtual ~MigrateMetric() = default;
};
#endif //MEC_MIGRATEMETRIC_H

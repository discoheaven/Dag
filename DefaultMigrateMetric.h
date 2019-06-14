//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_DEFAULTMIGRATEMETRIC_H
#define MEC_DEFAULTMIGRATEMETRIC_H
#include "MinEstMigrateMetric.h"
#include "MaxMobilityMigrateMetric.h"
#include "MinBESTMigratePolicy.h"
class DefaultMigrateMetric{
public:
    static MigrateMetric* get_metric()
    {
        return new MaxMobilityMigrateMetric();
    }
};
#endif //MEC_DEFAULTMIGRATEMETRIC_H

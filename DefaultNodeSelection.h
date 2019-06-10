//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_DEFAULTNODESELECTION_H
#define MEC_DEFAULTNODESELECTION_H
#include "NodeSelectionPolicy.h"
#include "RandomNodeSelection.h"
class DefaultNodeSelectionPolicy {
public:
    static NodeSelection* get_policy()
    {
        return new RandomNodeSelection();
    }
};
#endif //MEC_DEFAULTNODESELECTION_H

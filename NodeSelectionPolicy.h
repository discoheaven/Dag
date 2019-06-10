//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_NODESELECTIONPOLICY_H
#define MEC_NODESELECTIONPOLICY_H
#include <vector>
#include <memory>
#include "node.h"
using std::vector;
using std::shared_ptr;
class NodeSelection {
public:
    virtual shared_ptr<Node> select(vector<shared_ptr<Node>> &nodes) = 0;
    virtual ~NodeSelection() = default;
};
#endif //MEC_NODESELECTIONPOLICY_H

//
// Created by liuweidong02 on 2019/4/18.
//

#ifndef MEC_RANDOMNODESELECTION_H
#define MEC_RANDOMNODESELECTION_H
#include <random>
using std::default_random_engine;
#include "NodeSelectionPolicy.h"
class RandomNodeSelection : public NodeSelection{
public:
    RandomNodeSelection() = default;
    shared_ptr<Node> select(vector<shared_ptr<Node>> &nodes) override
    {
        assert(!nodes.empty());
        auto index = e() % nodes.size();
        return nodes[index];
    }
private:
    default_random_engine e;
};
#endif //MEC_RANDOMNODESELECTION_H

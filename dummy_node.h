//
// Created by liuweidong02 on 2019/4/16.
//

#ifndef MEC_DUMMYNODE_H
#define MEC_DUMMYNODE_H
#include "node.h"
class DummyNode : public Node
{
public:
    DummyNode(int id,double weight):Node(id,weight,true)
    {}
    void set_processor(pair<const shared_ptr<Processor>,int> processor) override;
private:

};
#endif //MEC_DUMMYNODE_H

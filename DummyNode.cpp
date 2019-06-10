//
// Created by liuweidong02 on 2019/4/16.
//
#include "DummyNode.h"
void DummyNode::set_processor(const shared_ptr<Processor> &processor)
{
    if(is_scheduled_)
        return;
    is_scheduled_ = true;
    processor_ = processor;
}

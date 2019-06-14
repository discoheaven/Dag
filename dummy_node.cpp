//
// Created by liuweidong02 on 2019/4/16.
//
#include "dummy_node.h"
void DummyNode::set_processor(pair<const shared_ptr<Processor>, int> processor)
{
    if(is_scheduled_){
        assert(processor.first == processor_.first.lock());
        processor_.second = processor.second;
    }else{
        is_scheduled_ = true;
        processor_ = processor;
    }
}

//
// Created by komine on 2019/3/11.
//
#include "node.h"
void Node::set_processor(pair<const shared_ptr<Processor>, int> processor)
{
    is_scheduled_ = true;
    processor_ = processor;
}


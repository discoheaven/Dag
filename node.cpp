//
// Created by komine on 2019/3/11.
//
#include "node.h"
void Node::set_processor(const shared_ptr<Processor> &processor)
{
    is_scheduled_ = true;
    processor_ = processor;
}


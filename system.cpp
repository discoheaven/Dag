//
// Created by komine on 2019/3/11.
//

#include <iostream>
#include "system.h"
using std::cout;
using std::endl;
System::System()
{
    int i = 0;
    for (; i < user_num; ++i) {
        auto user = new User(i, user_freq, Processor::TYPE::USER);
        processors_.emplace_back(user);
    }

    for(;i < user_num + mec_num;++i) {
        auto mec_sever = new Server(i, mec_freq, Processor::TYPE::MEC);
        processors_.emplace_back(mec_sever);
    }

    for(;i < user_num + mec_num + cloud_num;++i) {
        auto cloud_server = new Server(i, cloud_freq, Processor::TYPE::ClOUD);
        processors_.emplace_back(cloud_server);
    }
    dag_initial_processor_ = processors_.at(e_() % user_num);
    cout << "\nThe initial Processor with the whole DAG is Processor " << dag_initial_processor_->get_id() << "!\n" << endl;
}

shared_ptr<Processor> System::get_random_processor()
{
    return processors_[e_() % processors_.size()];
}
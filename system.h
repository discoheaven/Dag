//
// Created by komine on 2019/3/11.
//

#ifndef MEC_SYSTEM_H
#define MEC_SYSTEM_H

#include <vector>
#include <map>
#include <random>
#include "processor.h"
using std::vector;
using std::map;
using std::default_random_engine;
class System{
public:
    System();
    vector<shared_ptr<Processor>> get_processors() const {
        return processors_;
    }
    shared_ptr<Processor> get_random_processor();
    shared_ptr<Processor> get_dag_initial_processor() const{
        return dag_initial_processor_;
    }
private:
    default_random_engine e_;
    shared_ptr<Processor> dag_initial_processor_;
    vector<shared_ptr<Processor>> processors_;
    static const int user_freq = 10000;
    static const int mec_freq = 20000;
    static const int cloud_freq = 50000;
};
#endif //MEC_SYSTEM_H

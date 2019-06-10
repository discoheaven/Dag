//
// Created by komine on 2019/2/27.
//

#ifndef MEC_PROCESSOR_H
#define MEC_PROCESSOR_H
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <limits.h>
#include "node.h"

using std::vector;
using std::shared_ptr;
using std::map;
class Processor {
public:
    enum TYPE{
        USER,
        MEC,
        ClOUD
    };
    Processor(int id,double f ,TYPE t):processor_id_(id),frequency_(f),type(t){}
    void insert(const shared_ptr<Node> &node)
    {
        tasks.push_back(node);
    }
    void insert_at(const shared_ptr<Node> &node,int index)
    {
        assert(index >= 0 && index <= tasks.size());
        tasks.insert(tasks.begin()+index,node);
    }
    int retrieve_index(const shared_ptr<Node> &node)
    {
        auto loc = std::find(tasks.begin(),tasks.end(),node);
        if(loc == tasks.end())
            return -1;
        else
            return loc - tasks.begin();
    }
    void erase(const shared_ptr<Node> &node)
    {
        auto loc = std::find(tasks.begin(),tasks.end(),node);
        assert(loc != tasks.end());
        tasks.erase(loc);
    }
    virtual shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const = 0;
    virtual shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const = 0;
    virtual shared_ptr<Node> get_last_task() const = 0;
    vector<shared_ptr<Node>> get_tasks() const
    {
        return tasks;
    }
    virtual ~Processor() = default;
    double process_time(const shared_ptr<Node> &node){
        return node->get_workload() / get_freq();
    }
    double get_freq() const{
        return frequency_;
    }
    int get_id() const{
        return processor_id_;
    }
    TYPE get_type() const{
        return type;
    }
    bool isUser() const{
        return type == USER;
    }
    bool isMEC() const{
        return type == MEC;
    }
    bool isCloud() const{
        return type == ClOUD;
    }
    unsigned long get_task_size() const{
        return tasks.size();
    }
    void reset(){
        tasks.clear();
    }
protected:
    double frequency_;
    int processor_id_;
    TYPE type;
    vector<shared_ptr<Node>> tasks;
};
class Server :public Processor{
public:
    Server(int id,double f ,TYPE t):Processor(id,f,t) {}

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override{
        return nullptr;
    }
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override{
        return nullptr;
    }
    shared_ptr<Node> get_last_task() const override{
        return nullptr;
    }
};

class User :public Processor{
public:
    User(int id,double f,TYPE t):Processor(id,f,t){
    }

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override;
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override;
    shared_ptr<Node> get_last_task() const override{
        return tasks.empty()? nullptr : tasks.back();
    }
};
#endif //MEC_PROCESSOR_H

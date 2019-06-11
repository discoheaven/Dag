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
    virtual void insert_at(const shared_ptr<Node> &node, int index) = 0;
    virtual int retrieve_index(const shared_ptr<Node> &node) = 0;
    virtual void erase(const shared_ptr<Node> &node) = 0;
    virtual shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const = 0;
    virtual shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const = 0;
    virtual vector<shared_ptr<Node>> get_tasks() const = 0;

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
    virtual unsigned long get_task_size() const = 0;
    virtual void reset() = 0;

private:
    double frequency_;
    int processor_id_;
    TYPE type;
};

class User : public Processor {
public:
    User(int id,double f,TYPE t): Processor(id,f,t){}

    void insert_at(const shared_ptr<Node> &node,int index) override
    {
        assert(index >= 0 && index <= tasks.size());
        tasks.insert(tasks.begin()+index,node);
    }

    int retrieve_index(const shared_ptr<Node> &node) override
    {
        auto loc = std::find(tasks.begin(),tasks.end(),node);
        if(loc == tasks.end())
            return -1;
        else
            return loc - tasks.begin();
    }

    void erase(const shared_ptr<Node> &node) override
    {
        auto loc = std::find(tasks.begin(),tasks.end(),node);
        assert(loc != tasks.end());
        tasks.erase(loc);
    }

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override;
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override;

    vector<shared_ptr<Node>> get_tasks() const override
    {
        return tasks;
    }

    unsigned long get_task_size() const override{
        return tasks.size();
    }

    void reset() override {
        tasks.clear();
    }

private:
    vector<shared_ptr<Node>> tasks;
};

class Mec : public Processor{
public:
    Mec(int id,double f ,TYPE t): Processor(id,f,t) {}

};

class Cloud : public Processor{
public:
    Cloud(int id,double f,TYPE t): Processor(id,f,t){}

    void insert_at(const shared_ptr<Node> &node,int index) override {}

    int retrieve_index(const shared_ptr<Node> &node) override
    {
        return -1;
    }

    void erase(const shared_ptr<Node> &node) override {}

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override
    {
        return nullptr;
    }
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override
    {
        return nullptr;
    }

    vector<shared_ptr<Node>> get_tasks() const override
    {
        return vector<shared_ptr<Node>>();
    }

    unsigned long get_task_size() const override{
        return 0;
    }

    void reset() override {}
};
#endif //MEC_PROCESSOR_H

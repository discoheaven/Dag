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
using std::pair;

class Processor {
public:
    enum TYPE{
        USER,
        MEC,
        ClOUD
    };
    Processor(int id,double f ,TYPE t):processor_id_(id),frequency_(f),type(t){}
    virtual void insert_at(const shared_ptr<Node> &node, pair<int,int> index) = 0;
    virtual pair<int,int> retrieve_index(const shared_ptr<Node> &node) = 0;
    virtual void erase(const shared_ptr<Node> &node) = 0;
    virtual shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const = 0;
    virtual shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const = 0;
    virtual pair<int,int> get_avail_index() const = 0;

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
    virtual void reset() = 0;

private:
    double frequency_;
    int processor_id_;
    TYPE type;
};

class User : public Processor {
public:
    User(int id,double f,TYPE t): Processor(id,f,t), tasks(1, vector<shared_ptr<Node>>()){}

    void insert_at(const shared_ptr<Node> &node,pair<int,int> index) override;

    pair<int,int> retrieve_index(const shared_ptr<Node> &node) override;

    void erase(const shared_ptr<Node> &node) override;

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override;
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override;

    pair<int,int> get_avail_index() const override
    {
        return {0, tasks[0].size()};
    }

    void reset() override
    {
        tasks[0].clear();
    }

private:
    vector<vector<shared_ptr<Node>>> tasks;
};

class Mec : public Processor{
public:
    Mec(int id,double f ,TYPE t): Processor(id,f,t), tasks(mec_core_size, vector<shared_ptr<Node>>()) {}

    void insert_at(const shared_ptr<Node> &node,pair<int,int> index) override;

    pair<int,int> retrieve_index(const shared_ptr<Node> &node) override;

    void erase(const shared_ptr<Node> &node) override;

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override;

    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override;

    pair<int,int> get_avail_index() const override;

    void reset() override;
private:
    vector<vector<shared_ptr<Node>>> tasks;
    static const int mec_core_size = 4;
};

class Cloud : public Processor{
public:

    Cloud(int id,double f,TYPE t): Processor(id,f,t){}

    void insert_at(const shared_ptr<Node> &node,pair<int,int> index) override;

    pair<int,int> retrieve_index(const shared_ptr<Node> &node) override;

    void erase(const shared_ptr<Node> &node) override;

    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const override
    {
        return nullptr;
    }
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const override
    {
        return nullptr;
    }

    pair<int,int> get_avail_index() const override
    {
        return {tasks.size(),0};
    }

    void reset() override
    {
        tasks.clear();
    }
private:
    vector<vector<shared_ptr<Node>>> tasks;
};
#endif //MEC_PROCESSOR_H

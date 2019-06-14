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
#include "common.h"

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
    Processor(int id,double f,TYPE t,int core_sz):processor_id(id),frequency(f),
        type(t),core_size_(core_sz),tasks(core_sz,vector<shared_ptr<Node>>()){}
    virtual ~Processor() = default;

    void insert_at(const shared_ptr<Node> &node, pair<int,int> index);
    pair<int,int> retrieve_index(const shared_ptr<Node> &node);
    void erase(const shared_ptr<Node> &node);
    shared_ptr<Node> get_prev_task(const shared_ptr<Node> &node) const;
    shared_ptr<Node> get_next_task(const shared_ptr<Node> &node) const;
    pair<int,int> get_avail_index() const;
    shared_ptr<Node> get_node(pair<int,int> slot) const {return tasks[slot.first][slot.second];}

    vector<vector<shared_ptr<Node>>> get_all_tasks() const {return tasks;}
    int get_node_index(const shared_ptr<Node> &node,int core) const;
    int get_core_size() const {return core_size_;}
    int get_core_tasks_size(int core) const
    {
        assert(core < core_size_);
        return tasks[core].size();
    }
    void reset();

    double process_time(const shared_ptr<Node> &node){
        return node->get_workload() / get_freq();
    }
    double get_freq() const{
        return frequency;
    }
    int get_id() const{
        return processor_id;
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

private:
    double frequency;
    int processor_id;
    TYPE type;
    int core_size_;
protected:
    vector<vector<shared_ptr<Node>>> tasks;
};

class User : public Processor {
public:
    User(int id,double f,TYPE t): Processor(id,f,t,user_core_size){}
private:
    static const int user_core_size = 1;
};

class Mec : public Processor{
public:
    Mec(int id,double f ,TYPE t): Processor(id,f,t,mec_core_size){}
private:
    static const int mec_core_size = 4;
};

class Cloud : public Processor{
public:
    Cloud(int id,double f,TYPE t): Processor(id,f,t,nodes_size){}
private:
    const int cloud_core_sz = nodes_size;
};
#endif //MEC_PROCESSOR_H

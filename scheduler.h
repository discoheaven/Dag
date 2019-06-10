//
// Created by 刘卫东 on 2019-03-30.
//

#ifndef MEC_SCHEDULER_H
#define MEC_SCHEDULER_H
#include "dag.h"
#include "system.h"
class Scheduler {
public:
    Scheduler(const Dag& dag, const System& sy): nodes(dag.get_nodes()),
        edges(dag.get_edges()),processors(sy.get_processors())
    {
    }
    void set_schedule(const shared_ptr<Node> &node,const shared_ptr<Processor> &processor);
    void set_schedule_at(const shared_ptr<Node> &node,const shared_ptr<Processor> &processor,int index);
    void update();
    double calc_est(const shared_ptr<Node> &node);
    double calc_lst(const shared_ptr<Node> &node);
    void update_est();
    void update_lst();
    vector<shared_ptr<Node>> get_nodes() const {
        return nodes;
    }
    vector<shared_ptr<Processor>> get_processors() const {
        return processors;
    }
private:
    double get_comm_rate(const shared_ptr<Node> &node1,const shared_ptr<Node> &node2) const;
    double get_comm_rate(const shared_ptr<Processor> &node1,const shared_ptr<Processor> &node2) const;
    double calc_est_parents(const shared_ptr<Node> &node);
    double calc_est_processors(const shared_ptr<Node> &node);
    double calc_lst_parents(const shared_ptr<Node> &node);
    double calc_lst_processors(const shared_ptr<Node> &node);
    bool ready_for_est_update(const shared_ptr<Node> &node);
    bool ready_for_lst_update(const shared_ptr<Node> &node);
    vector<shared_ptr<Node>> nodes;
    map<pair<int,int>,double> edges;
    vector<shared_ptr<Processor>> processors;
};
#endif //MEC_SCHEDULER_H

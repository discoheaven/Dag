//
// Created by lwd on 19-3-28.
//

#ifndef MEC_DAGGER_H
#define MEC_DAGGER_H
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include "node.h"
using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::map;
using std::pair;
class Dag {
public:
    enum Dag_Selector
    {
        RANDOM_DAG,
        WORKFLOW_DAG,
        TREE_DAG
    };
    explicit Dag(Dag_Selector s = RANDOM_DAG);
    vector<shared_ptr<Node>> get_nodes() const{
        return nodes;
    }
    map<pair<int,int>,double> get_edges() const{
        return edges;
    }
    void print_dag() const;
private:
    void random_dag_generator();
    void workflow_dag_generator();
    void tree_dag_generator();
    void set_all_children_fathers();
    Dag_Selector selector;
    vector<shared_ptr<Node>> nodes;
    map<pair<int,int>,double> edges;
};
#endif //MEC_DAGGER_H

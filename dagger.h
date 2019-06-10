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
class Dagger {
public:
    enum Dag_Selector
    {
        DAG1,
        DAG2,
        DAG3,
        DAG4,
        DAG5,
        DAG6,
        RANDOM_DAG
    };
    Dagger(Dag_Selector s);
    vector<shared_ptr<Node>> get_nodes() const{
        return nodes;
    }
    map<pair<int,int>,double> get_edges() const{
        return edges;
    }
    void print_dag() const;
private:
    void dag1_generator();
    void dag2_generator();
    void dag3_generator();
    void dag4_generator();
    void dag5_generator();
    void dag6_generator();
    void random_dag_generator();
    void set_all_children_fathers();
    Dag_Selector selector;
    vector<shared_ptr<Node>> nodes;
    map<pair<int,int>,double> edges;
    static const int random_nodes_size = 100;
};
#endif //MEC_DAGGER_H

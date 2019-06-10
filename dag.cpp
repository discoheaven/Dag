//
// Created by lwd on 19-3-28.
//
#include <vector>
#include <memory>
#include <random>
#include <set>
#include <cassert>
#include <ctime>
#include "dag.h"
#include "dummy_node.h"
#include "common.h"
using std::vector;
using std::shared_ptr;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::set;
using std::pair;

Dag::Dag(Dag::Dag_Selector s) : selector(s)
{
    switch(selector){
        case DAG1:
            dag1_generator();
            break;
        case DAG2:
            dag2_generator();
            break;
        case RANDOM_DAG:
            random_dag_generator();
            break;
        default:
            break;
    }
    for(auto &item : nodes)
        item->reset_cnts();
    set_all_children_fathers();
    //print_dag();
}


void Dag::dag1_generator()
{
    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,100);
    uniform_real_distribution<double> ue(0,1);
    for(int i = 0;i < 5;++i)
        nodes.emplace_back(new Node(i,u(e)));
    nodes[0]->InsertChildren(nodes[3]);
    edges.insert({{0,3}, ue(e)});
    nodes[1]->InsertChildren(nodes[3]);
    edges.insert({{1,3},ue(e)});
    nodes[2]->InsertChildren(nodes[4]);
    edges.insert({{2,4},ue(e)});
    nodes[3]->InsertFather(nodes[0]);
    nodes[3]->InsertFather(nodes[1]);
    nodes[3]->InsertChildren(nodes[4]);
    edges.insert({{3,4},ue(e)});
    nodes[4]->InsertFather(nodes[3]);
    nodes[4]->InsertFather(nodes[2]);
}

void Dag::dag2_generator()
{
    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,100);
    uniform_int_distribution<unsigned> ue(1,10);
    for(int i = 0;i < 6;++i)
        nodes.emplace_back(new Node(i,u(e)));
    nodes[0]->InsertChildren(nodes[1]);
    edges.insert({{0,1},ue(e)});
    nodes[0]->InsertChildren(nodes[2]);
    edges.insert({{0,2},ue(e)});
    nodes[1]->InsertFather(nodes[0]);
    nodes[1]->InsertChildren(nodes[3]);
    edges.insert({{1,3},ue(e)});
    nodes[1]->InsertChildren(nodes[4]);
    edges.insert({{1,4},ue(e)});
    nodes[2]->InsertFather(nodes[0]);
    nodes[2]->InsertChildren(nodes[5]);
    edges.insert({{2,5},ue(e)});
    nodes[3]->InsertFather(nodes[1]);
    nodes[4]->InsertFather(nodes[1]);
    nodes[5]->InsertFather(nodes[2]);
}

void Dag::random_dag_generator()
{
    static int nodes_sz = nodes_size;

    int id = 0;

    nodes.emplace_back(new DummyNode(id++,0));

    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,100);
    uniform_int_distribution<unsigned> ue(1,10);

    vector<shared_ptr<Node>> first_layer_nodes;
    vector<shared_ptr<Node>> last_layer_nodes;

    auto inital_layer_nodes_size = static_cast<int>(sqrt(nodes_sz));

    for(int i = 0;i <inital_layer_nodes_size;++i)
    {
        shared_ptr<Node> node(new Node(id++, u(e)));
        nodes.push_back(node);
        first_layer_nodes.push_back(node);
    }

    int remain = nodes_sz - inital_layer_nodes_size;
    int last_layer_index = 1;
    int last_layer_size = inital_layer_nodes_size;
    int cur_layer_size = 0;
    int cur_layer_index= 1;
    while(remain > 0)
    {
        if(remain < inital_layer_nodes_size)
            cur_layer_size = remain;
        else
            cur_layer_size = static_cast<int>(e() % inital_layer_nodes_size);

        if(cur_layer_size == 0)
            continue;

        cur_layer_index = last_layer_index + last_layer_size;

        for(int i = 0;i < cur_layer_size;++i)
        {
            shared_ptr<Node> node(new Node(id++,u(e)));
            nodes.push_back(node);
            if(remain - cur_layer_size == 0)
                last_layer_nodes.push_back(node);
        }

        set<pair<int,int>> st;

        auto edge_size = last_layer_size * sqrt(cur_layer_size);
        auto last_layer_max_index = last_layer_index + last_layer_size;

        for(int i = 0,select1_index = last_layer_index;i < edge_size;select1_index++)
        {
            if(select1_index >= last_layer_max_index)
                select1_index = last_layer_index;
            auto select2_offset = e() % cur_layer_size;
            auto select2_index = cur_layer_index + select2_offset;
            assert(select1_index < cur_layer_index);
            auto node1 = nodes[select1_index];
            auto node2 = nodes[select2_index];
            if(st.find({node1->get_id(),node2->get_id()}) == st.end())
            {
                node1->InsertChildren(node2);
                node2->InsertFather(node1);
                edges.insert({{node1->get_id(),node2->get_id()},ue(e)});
                st.insert({node1->get_id(), node2->get_id()});
                ++i;
            }
        }

        remain -= cur_layer_size;
        last_layer_index = cur_layer_index;
        last_layer_size = cur_layer_size;
    }

    nodes.emplace_back(new DummyNode(id,0));

    auto begin_dummy_node = nodes.front();
    auto end_dummy_node = nodes.back();

    assert(first_layer_nodes.size() == inital_layer_nodes_size);
    assert(last_layer_nodes.size() == last_layer_size);

    for(auto &item : first_layer_nodes)
    {
        begin_dummy_node->InsertChildren(item);
        item->InsertFather(begin_dummy_node);
        edges.insert({{begin_dummy_node->get_id(),item->get_id()},ue(e)});
    }

    for(auto &item : last_layer_nodes)
    {
        end_dummy_node->InsertFather(item);
        item->InsertChildren(end_dummy_node);
        edges.insert({{item->get_id(),end_dummy_node->get_id()},ue(e)});
    }

    cout << "random dag generation done!\n" << endl;

}

void helper_father(const shared_ptr<Node>& node,const shared_ptr<Node>& cur_node,set<shared_ptr<Node>>& st)
{
    auto fathers = cur_node->get_fathers();
    for(auto &item : fathers)
    {
        auto sitem = item.lock();
        st.insert(sitem);
        helper_father(node,sitem,st);
    }
}

void helper_child(const shared_ptr<Node>& node,const shared_ptr<Node>& cur_node,set<shared_ptr<Node>>& st)
{
    auto children = cur_node->get_children();
    for(auto &item : children)
    {
        auto sitem = item.lock();
        st.insert(sitem);
        helper_child(node,sitem,st);
    }
}

void Dag::set_all_children_fathers()
{
    set<shared_ptr<Node>> st;
    for(auto &item : nodes)
    {
        st.clear();
        helper_father(item,item,st);
        for(auto &t_item : st)
            item->insert_all_fathers(t_item);
        st.clear();
        helper_child(item,item,st);
        for(auto &t_item : st)
            item->insert_all_children(t_item);
    }
}

void Dag::print_dag() const
{
    for(auto &item : nodes)
    {
        auto father_id = item->get_id();
        auto node_weight = item->get_workload();
        cout << father_id << "(" << node_weight <<"): ";
        for(auto &child : item->get_children())
        {
            auto s_child = child.lock();
            auto child_id = s_child->get_id();
            auto loc = edges.find({father_id,child_id});
            assert(loc != edges.end());
            auto edge_weight = loc->second;
            cout << child_id << "(" << edge_weight <<") ";
        }
        cout << endl;
    }
}



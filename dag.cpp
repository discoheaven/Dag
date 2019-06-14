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
        case RANDOM_DAG:
            random_dag_generator();
            break;
        case WORKFLOW_DAG:
            workflow_dag_generator();
            break;
        case TREE_DAG:
            tree_dag_generator();
            break;
        default:
            break;
    }
    for(auto &item : nodes)
        item->reset_cnts();
    set_all_children_fathers();
    //print_dag();
}

void Dag::random_dag_generator()
{
    int nodes_sz = nodes_size;

    int id = 0;

    nodes.emplace_back(new DummyNode(id++,0));

    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,100);
    uniform_int_distribution<unsigned> ue(1,100);

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

void Dag::workflow_dag_generator()
{
    int nodes_sz = nodes_size;

    int id = 0;

    nodes.emplace_back(new DummyNode(id++,0));

    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,100);
    uniform_int_distribution<unsigned> ue(1,100);

    auto each_layer_size = static_cast<int>(sqrt(nodes_sz));
    int remaining_nodes_sz = nodes_sz;
    int cur_layer_size = 0;
    bool single_node_flag = false;

    vector<shared_ptr<Node>> last_layer_nodes;
    shared_ptr<Node> last_node = nodes.front();

    while(remaining_nodes_sz > 0)
    {
        if(single_node_flag)
        {
            auto node = std::make_shared<Node>(id++, u(e));
            nodes.push_back(node);
            for(auto &item : last_layer_nodes)
            {
                item->InsertChildren(node);
                node->InsertFather(item);
                edges.insert({{item->get_id(),node->get_id()},ue(e)});
            }
            last_layer_nodes.clear();
            last_node = node;
            remaining_nodes_sz -= 1;
        }
        else
        {
            if(remaining_nodes_sz < each_layer_size)
                cur_layer_size = remaining_nodes_sz;
            else {
                cur_layer_size = static_cast<int>(e() % each_layer_size);
                if (cur_layer_size == 0)
                    continue;
            }

            for(int i = 0;i < cur_layer_size;++i)
            {
                auto node = std::make_shared<Node>(id++, u(e));
                nodes.push_back(node);
                last_node->InsertChildren(node);
                node->InsertFather(last_node);
                edges.insert({{last_node->get_id(),node->get_id()},ue(e)});
                last_layer_nodes.push_back(node);
            }

            last_node = nullptr;
            remaining_nodes_sz -= cur_layer_size;
        }
        single_node_flag = !single_node_flag;
    }

    nodes.emplace_back(new DummyNode(id,0));

    auto end_dummy_node = nodes.back();

    if(single_node_flag)
    {
        assert(last_node == nullptr);
        for(auto &item : last_layer_nodes)
        {
            item->InsertChildren(end_dummy_node);
            end_dummy_node->InsertFather(item);
            edges.insert({{item->get_id(),end_dummy_node->get_id()},ue(e)});
        }
    }
    else
    {
        assert(last_layer_nodes.empty());
        last_node->InsertChildren(end_dummy_node);
        end_dummy_node->InsertFather(last_node);
        edges.insert({{last_node->get_id(),end_dummy_node->get_id()},ue(e)});
    }
    cout << "workflow dag generation done!\n" << endl;
}

void Dag::tree_dag_generator()
{
    int nodes_sz = nodes_size;

    int id = 0;

    nodes.emplace_back(new DummyNode(id++,0));

    default_random_engine e;
    uniform_int_distribution<unsigned> u(1,100);
    uniform_int_distribution<unsigned> ue(1,100);

    int nary = 2;
    unsigned long cur_nary = 0;
    int remaining_nodes_sz = nodes_sz;
    vector<shared_ptr<Node>> last_layer_nodes{nodes.front()};
    vector<shared_ptr<Node>> cur_layer_nodes;

    while(remaining_nodes_sz > 0)
    {
        for(auto &item : last_layer_nodes)
        {
            cur_nary = e() % nary + 1;
            for(int i = 0;i < cur_nary;++i) {
                auto node = std::make_shared<Node>(id++, u(e));
                nodes.push_back(node);
                item->InsertChildren(node);
                node->InsertFather(item);
                edges.insert({{item->get_id(), node->get_id()}, ue(e)});
                cur_layer_nodes.push_back(node);
                remaining_nodes_sz -= 1;
                if(remaining_nodes_sz == 0)
                    break;
            }
            if(remaining_nodes_sz == 0)
                break;
        }
        last_layer_nodes = cur_layer_nodes;
        cur_layer_nodes.clear();
    }

    nodes.emplace_back(new DummyNode(id,0));

    auto end_dummy_node = nodes.back();

    for(auto &item : last_layer_nodes)
    {
        item->InsertChildren(end_dummy_node);
        end_dummy_node->InsertFather(item);
        edges.insert({{item->get_id(), end_dummy_node->get_id()},ue(e)});
    }
    cout << "tree dag generation done!\n" << endl;
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



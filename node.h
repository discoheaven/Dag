//
// Created by komine on 2019/2/27.
//

#ifndef MEC_NODE_H
#define MEC_NODE_H

#include <vector>
#include <random>
#include <memory>
#include <assert.h>
class Processor;
using std::weak_ptr;
using std::vector;
using std::shared_ptr;
class Node{
public:
    Node() = delete;

    Node(int id, double weight, bool is_fixed = false) : node_id_(id), weight_(weight),is_scheduled_(false),
                                  est_set_(false), lst_set_(false), is_fixed_(is_fixed) {}
    ~Node() = default;

    double get_workload() const { return weight_; }
    int get_id() const { return node_id_; }
    void InsertFather(const shared_ptr<Node>& father){
        fathers_.push_back(father);
    }
    void InsertChildren(const shared_ptr<Node>& children){
        children_.push_back(children);
    }

    void insert_all_fathers(const shared_ptr<Node>& father){
        all_fathers_.push_back(father);
    }

    vector<weak_ptr<Node>> get_all_fathers(){
        return all_fathers_;
    }

    void insert_all_children(const shared_ptr<Node>& child){
        all_children_.push_back(child);
    }

    vector<weak_ptr<Node>> get_all_children(){
        return all_children_;
    }

    const vector<weak_ptr<Node>> get_fathers() const {
        return fathers_;
    }

    const vector<weak_ptr<Node>> get_children() const{
        return children_;
    }

    void set_finish_time(double time){
        finish_time_ = time;
    }

    double get_finish_time() const{
        return finish_time_;
    }

    void set_est(double tm){
        est_set_ = true;
        est_ = tm;
    }

    double get_est() const{
        return est_;
    }

    void set_lst(double tm){
        lst_set_ = true;
        lst_ = tm;
    }

    double get_lst() const{
        return lst_;
    }

    bool is_scheduled() const{
        return is_scheduled_;
    }

    virtual void set_processor(const shared_ptr<Processor> &processor);

    shared_ptr<Processor> get_processor() const{
        return processor_.lock();
    }


    void dec_pcnt(){
        if(pcnt_ == 0)
            return;
        --pcnt_;
    }

    int get_pcnt() const{
        return pcnt_;
    }


    int get_ccnt() const{
        return ccnt_;
    }

    void dec_ccnt(){
        if(ccnt_ == 0)
            return;
        --ccnt_;
    }

    void reset_cnts(){
        pcnt_ = static_cast<int>(fathers_.size());
        ccnt_ = static_cast<int>(children_.size());
        lst_set_ = false;
        est_set_ = false;
    }

    bool ready_for_est_update() const{
        return pcnt_ == 0;
    }

    bool ready_for_lst_update() const{
        return ccnt_ == 0;
    }

    bool lst_set() const{
        return lst_set_;
    }

    bool est_set() const{
        return est_set_;
    }

    bool is_fixed_processor() const{
        return is_fixed_;
    }

    void reset()
    {
        finish_time_ = 0;
        is_scheduled_ = false;
        est_set_ = false;
        lst_set_ = false;
        est_ = 0;
        lst_ = 0;
        pcnt_ = 0;
        ccnt_ = 0;
    }

protected:
    int node_id_;
    double weight_;
    vector<weak_ptr<Node>> fathers_;
    vector<weak_ptr<Node>> children_;
    vector<weak_ptr<Node>> all_fathers_;
    vector<weak_ptr<Node>> all_children_;
    weak_ptr<Processor> processor_;
    double finish_time_;
    bool is_scheduled_;
    bool is_fixed_;
    bool est_set_;
    bool lst_set_;
    double est_;
    double lst_;
    int pcnt_;
    int ccnt_;
};

#endif //MEC_NODE_H

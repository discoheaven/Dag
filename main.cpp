#include "dag.h"
#include "system.h"
#include "myalgorithm.h"
#include "myalgorithm_tabu.h"
#include "greedy.h"
#include "common.h"
using std::cout;
using std::endl;

int main() {
    vector<int> nodes_range{30,40,50,60,70};
    vector<int> processor_range{3,6,9,12,15};
    vector<int> ccr_range{1000,2000,3000,4000,5000};
    vector<Dag::Dag_Selector > dag_range{Dag::TREE_DAG, Dag::WORKFLOW_DAG, Dag::RANDOM_DAG};
    /*for(auto dag_class : dag_range)
    {
        for(auto item : nodes_range)
        {
            nodes_size = item;
            Dag dag = Dag(dag_class);
            System sys = System();
            MyAlgo algo = MyAlgo();
            algo.run(dag, sys);
        }
    }
    nodes_size = 50;
    for(auto dag_class : dag_range)
    {
        for(auto item : processor_range)
        {
            processors_size = item;
            Dag dag = Dag(dag_class);
            System sys = System();
            MyAlgo algo = MyAlgo();
            algo.run(dag, sys);
        }
    }
    processors_size = 15;
    for(auto dag_class : dag_range)
    {
        for(auto item : ccr_range)
        {
            CCR = item;
            Dag dag = Dag(dag_class);
            System sys = System();
            MyAlgo algo = MyAlgo();
            algo.run(dag, sys);
        }
    }
    CCR = 3000;*/

    Dag dag = Dag(Dag::RANDOM_DAG);
    System sys = System();
    MyAlgoTabu algo = MyAlgoTabu();
    algo.run(dag, sys);
}
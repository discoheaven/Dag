#include "dag.h"
#include "system.h"
#include "myalgorithm.h"
#include "greedy.h"
using std::cout;
using std::endl;

int main() {
    Dag dag = Dag(Dag::TREE_DAG);
    System sys = System();
    MyAlgo algo = MyAlgo();
    algo.run(dag, sys);
}
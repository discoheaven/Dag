cmake_minimum_required(VERSION 3.12)
project(mec)

set(CMAKE_CXX_STANDARD 11)

add_executable(mec main.cpp myalgorithm.h myalgorithm.cpp processor.h processor.cpp solution.h solution.cpp system.h system.cpp dag.h dag.cpp scheduler.h scheduler.cpp node.h node.cpp greedy.h greedy.cpp dummy_node.h dummy_node.cpp NodeSelectionPolicy.h RandomNodeSelection.h DefaultNodeSelection.h MinEstMigrateMetric.h MinEstMigrateMetric.cpp MigrateMetric.h DefaultMigrateMetric.h MaxMobilityMigrateMetric.h MaxMobilityMigrateMetric.cpp MinBESTMigratePolicy.h MinBESTMigrateMetric.cpp common.h common.cpp myalgorithm_tabu.h myalgorithm_tabu.cpp)
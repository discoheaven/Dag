cmake_minimum_required(VERSION 3.12)
project(mec)

set(CMAKE_CXX_STANDARD 11)

add_executable(mec main.cpp myalgorithm.h myalgorithm.cpp processor.h processor.cpp solution.h solution.cpp system.h system.cpp dagger.h dagger.cpp scheduler.h scheduler.cpp node.h node.cpp greedy1.h greedy1.cpp DummyNode.h DummyNode.cpp NodeSelectionPolicy.h RandomNodeSelection.h DefaultNodeSelection.h MinEstMigrateMetric.h MinEstMigrateMetric.cpp MigrateMetric.h DefaultMigrateMetric.h MaxMobilityMigrateMetric.h MaxMobilityMigrateMetric.cpp MinBESTMigratePolicy.h MinBESTMigrateMetric.cpp common.h)
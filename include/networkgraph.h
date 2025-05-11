
#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <graph.hpp>
#include "definitions.h"
#include <map>
#include <queue>
#include <iostream>


class NetworkGraph
{
public:
    struct NodeData {
        int capacity;
        int flow = 0;
        std::string label;
    };

    struct ArcData {
        int capacity;
        int flow = 0;
    };

    using FlowGraph = Designar::Digraph<NodeData, ArcData>;
    using Node = Designar::Digraph<NetworkGraph::NodeData, NetworkGraph::ArcData>::Node*;

    NetworkGraph(const std::vector<Teacher>& teachers, const StudyPlan& plan);

    void printGraph() const;
    void simulateAssignments();

private:
    FlowGraph graph;
    const std::vector<Teacher>& teachers;
    const StudyPlan& study_plan;

    std::map<std::string, Node> teacher_nodes;
    std::map<std::string, Node> subject_nodes;
    Node source_node;
    Node sink_node;

    void buildNetwork();
};

#endif // NETWORKGRAPH_H


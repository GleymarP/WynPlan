
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
    enum class NodeType { Source, Sink, Subject, Teacher};
    struct NodeData
    {
        NodeType type;
        int capacity;
        int flow = 0;
        std::string id;
    };

    struct ArcData
    {
        int capacity;
        int flow = 0;
    };

    using FlowGraph = Designar::Digraph<NodeData, ArcData>;
    using Node = Designar::Digraph<NetworkGraph::NodeData, NetworkGraph::ArcData>::Node*;

    NetworkGraph(const std::vector<Teacher>& teachers, const std::vector<Subject>& subjects);

    void print_graph() const;

private:
    FlowGraph graph;
    const std::vector<Teacher>& teachers;
    const std::vector<Subject>& subjects;

    std::map<std::string, Node> teacher_nodes;
    std::map<std::string, Node> subject_nodes;
    Node source_node;
    Node sink_node;

    void build_network();
};

#endif // NETWORKGRAPH_H


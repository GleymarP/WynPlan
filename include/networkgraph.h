#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <graph.hpp>
#include "definitions.h"
#include <map>
#include <queue>
#include <iostream>
#include <set>
#include <tuple>


class NetworkGraph
{
public:
    enum class NodeType { Source, Sink, Subject, Teacher, Time};
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

    bool bfs(std::map<Node, Node>& parent);

    int max_flow();

    std::vector<std::tuple<std::string, std::string, int, int>> get_final_assignments() const;
    static void save_assignment(std::vector<std::tuple<std::string, std::string, int, int>> assignments, const QString& file_path, const Semester& semester);

private:
    FlowGraph graph;
    const std::vector<Teacher>& teachers;
    const std::vector<Subject>& subjects;

    std::map<std::string, Node> teacher_nodes;
    std::map<std::string, Node> subject_nodes;
    std::map<std::pair<int, int>, Node> timetable_nodes;

    Node source_node;
    Node sink_node;

    void build_network();
};

#endif // NETWORKGRAPH_H


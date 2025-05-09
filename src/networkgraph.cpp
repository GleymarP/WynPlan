// NetworkGraph.cpp
#include "networkgraph.h"
using Node = Designar::Digraph<NetworkGraph::NodeData, NetworkGraph::ArcData>::Node*;

NetworkGraph::NetworkGraph(const std::vector<Teacher>& teachers, const StudyPlan& plan)
    : teachers(teachers), study_plan(plan)
{
    buildNetwork();
}

void NetworkGraph::buildNetwork()
{
    source_node = graph.insert_node({INT_MAX, 0, "FUENTE"});
    sink_node = graph.insert_node({INT_MAX, 0, "SUMIDERO"});


    for (const auto& teacher : teachers)
    {
        Node node_id = graph.insert_node({1, 0, "Prof: " + teacher.get_full_name()});
        teacher_nodes[teacher.get_full_name()] = node_id;
        graph.insert_arc(source_node, node_id, {1, 0});
    }

    for (const auto& semester : study_plan.get_semester())
    {
        for (const auto& subject : semester.get_subjects_semester())
        {
            Node node_id = graph.insert_node({1, 0, "Materia: " + subject.get_id() + " - " + subject.get_subject_name()});
            subject_nodes[subject.get_id()] = node_id;
        }
    }


    for (const auto& teacher : teachers)
    {
        Node teacher_node = teacher_nodes[teacher.get_full_name()];
        for (const auto& subject : teacher.get_subjects()) {
            if (subject_nodes.count(subject.get_id()))
            {
                graph.insert_arc(teacher_node, subject_nodes[subject.get_id()], {1, 0});
            }
        }
    }

    for (const auto& [subject_id, node_id] : subject_nodes)
    {
        graph.insert_arc(node_id, sink_node, {1, 0});
    }
}

void NetworkGraph::printGraph() const
{
    std::cout << "\n=== ESTRUCTURA DEL GRAFO ===\n";
    std::cout << "Nodos totales: " << graph.get_num_nodes() << "\n";
    std::cout << "Arcos totales: " << graph.get_num_arcs() << "\n\n";


    std::cout << "NODOS:\n";
    graph.for_each_node([&](auto node) {
        const auto& info = node->get_info();
        std::cout << "Label: " << info.label
                  << " | Cap: " << info.capacity
                  << " | Flow: " << info.flow << "\n";
    });

    // Mostrar arcos
    std::cout << "\nARCOS:\n";
    graph.for_each_arc([&](auto arc) {
        const auto& info = arc->get_info();
        const auto& src_label = arc->get_src_node()->get_info().label;
        const auto& tgt_label = arc->get_tgt_node()->get_info().label;

        std::cout << src_label << " -> " << tgt_label
                  << " [Cap: " << info.capacity
                  << ", Flow: " << info.flow << "]\n";
    });
}


#include "networkgraph.h"

using Node = Designar::Digraph<NetworkGraph::NodeData, NetworkGraph::ArcData>::Node*;

NetworkGraph::NetworkGraph(const std::vector<Teacher>& teachers, const std::vector<Subject>& subjects)
    : teachers(teachers), subjects(subjects)
{
    build_network();
}

void NetworkGraph::build_network()
{
    source_node = graph.insert_node({NodeType::Source, INT_MAX, 0, "source"});
    sink_node = graph.insert_node({NodeType::Sink, INT_MAX, 0, "sink"});

    for(const auto& subject : subjects)
    {
        Node subject_node = graph.insert_node({NodeType::Subject, subject.get_hours(), 0, subject.get_id()});
        subject_nodes[subject.get_id()] = subject_node;
        graph.insert_arc(source_node, subject_node, {subject.get_hours(), 0});
    }

    for(const auto& teacher: teachers)
    {
        Node teacher_node = graph.insert_node({NodeType::Teacher, 0, 0,  teacher.get_id()});
        teacher_nodes[teacher.get_id()] = teacher_node;
        for (int day = 0; day < 7; day++)
        {
            for (int hour = 0; hour < 12; hour++)
            {
                if(teacher.available_block(day, hour))
                {
                    std::string time_id = "T_" + teacher.get_id() + "_D_" + std::to_string(day) + "_H_" + std::to_string(hour);
                    Node time_node = graph.insert_node({NodeType::TimeTable, 1, 0 ,time_id});
                    timetable_nodes[std::make_pair(day, hour)] = time_node;
                    graph.insert_arc(teacher_node, time_node, {1, 0});

                    for (const Subject& subject : teacher.get_subjects())
                    {
                        if (subject_nodes.count(subject.get_id()))
                        {
                            graph.insert_arc(subject_nodes[subject.get_id()], time_node, {1, 0});
                        }
                    }
                    graph.insert_arc(time_node, sink_node, {1, 0});
                }
            }
        }
    }
}

bool NetworkGraph::bfs(std::map<Node, Node> &parent)
{
    std::queue<Node> q;
    std::set<Node> visited;

    q.push(source_node);
    visited.insert(source_node);
    parent[source_node] = nullptr;

    while(!q.empty())
    {
        Node current = q.front();
        q.pop();


        for(auto arc : graph.adjacent_arcs(current))
        {
            Node neighbor = arc->get_tgt_node();
            int residual = arc->get_info().capacity - arc->get_info().flow;

            if(residual > 0 && !visited.count(neighbor))
            {
                parent[neighbor] = current;
                if(neighbor == sink_node)
                {
                    return true;
                }
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return false;
}

int NetworkGraph::max_flow()
{
    int total_flow = 0;
    std::map<Node, Node> parent;

    while(bfs(parent))
    {
        int path_flow = INT_MAX;

        for(Node v = sink_node; v != source_node; v = parent[v])
        {
            Node u = parent[v];
            auto arc = graph.search_arc(u, v);
            path_flow = std::min(path_flow, arc->get_info().capacity - arc->get_info().flow);

        }

        for(Node v = sink_node; v != source_node; v = parent[v])
        {
            Node u = parent[v];
            auto arc = graph.search_arc(u, v);
            arc->get_info().flow += path_flow;

            if(auto rev = graph.search_arc(v, u))
            {
                rev->get_info().flow -= path_flow;

            }
            else
            {
                graph.insert_arc(v, u, {0, -path_flow});
            }
        }

        total_flow += path_flow;
    }

    return total_flow;

}

void NetworkGraph::print_graph() const
{

    qDebug() << "\n=== ESTRUCTURA DEL GRAFO ===";
    qDebug() << "Nodos totales:" << graph.get_num_nodes();
    qDebug() << "Arcos totales:" << graph.get_num_arcs();

    qDebug() << "\nNODOS:";
    graph.for_each_node([&](auto node) {
        const auto& info = node->get_info();
        qDebug() << "Label:" << QString::fromStdString(info.id)
                 << "| Cap:" << info.capacity
                 << "| Flow:" << info.flow;
    });

    qDebug() << "\nARCOS:";
    graph.for_each_arc([&](auto arc) {
        const auto& info = arc->get_info();
        const auto& src_label = arc->get_src_node()->get_info().id;
        const auto& tgt_label = arc->get_tgt_node()->get_info().id;

        qDebug() << QString::fromStdString(src_label)
                 << "->" << QString::fromStdString(tgt_label)
                 << "[Cap:" << info.capacity
                 << ", Flow:" << info.flow << "]";
    });
}

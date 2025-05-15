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
        //Node subject_node = graph.insert_node({NodeType::Subject, 1, 1, subject.get_id()});
        //subject_nodes[subject.get_id()] = subject_node;
        //graph.insert_arc(source_node, subject_node, {1, 1});
        Node subject_node = graph.insert_node({NodeType::Subject, subject.get_hours(), 0, subject.get_id()});
        subject_nodes[subject.get_id()] = subject_node;
        graph.insert_arc(source_node, subject_node, {subject.get_hours(), 0});
    }

/*    for(const auto& teacher: teachers)
    {
        for(const auto& teacher_subject: teacher.get_subjects())
        {
            if(subject_nodes.count(teacher_subject.get_id()))
            {
                Node teacher_node = graph.insert_node({NodeType::Teacher, 1, 0, teacher.get_id()});
                teacher_nodes[teacher.get_id()] = teacher_node;
                graph.insert_arc(subject_nodes[teacher_subject.get_id()], teacher_node, {1,0});
            }
        }
    }

    for(const auto& [teacher_id, node_id] : teacher_nodes)
    {
        graph.insert_arc(node_id, sink_node, {1,0});
    }

    std::cout << "Teachers: " << teachers.size() << ", Subjects: " << subjects.size() << "\n";
*/
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
                    timetable_nodes[{day, hour}] = time_node;
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

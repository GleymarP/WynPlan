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


    for (const auto& subject : subjects)
    {
        Node subject_node = graph.insert_node({NodeType::Subject, subject.get_hours(), 0, subject.get_id()});
        subject_nodes[subject.get_id()] = subject_node;
        graph.insert_arc(source_node, subject_node, {subject.get_hours(), 0});
    }


    for (const auto& teacher : teachers)
    {
        Node teacher_node = graph.insert_node({NodeType::Teacher, 0, 0, teacher.get_id()});
        teacher_nodes[teacher.get_id()] = teacher_node;


        for (const auto& subject : teacher.get_subjects())
        {
            if (subject_nodes.count(subject.get_id()))
            {
                graph.insert_arc(subject_nodes[subject.get_id()], teacher_node, {subject.get_hours(), 0});
            }
        }


        for (int day = 0; day < 7; ++day)
        {
            for (int hour = 0; hour < 12; ++hour)
            {
                if (teacher.available_block(day, hour))
                {
                    std::pair<int, int> time_key = {day, hour};
                    Node timeblock_node;


                    if (!timetable_nodes.count(time_key))
                    {
                        std::string time_id = "D_" + std::to_string(day) + "H_" + std::to_string(hour);
                        timeblock_node = graph.insert_node({NodeType::Time, 1, 0, time_id});
                        timetable_nodes[time_key] = timeblock_node;


                        graph.insert_arc(timeblock_node, sink_node, {1, 0});
                    }
                    else
                    {
                        timeblock_node = timetable_nodes[time_key];
                    }

                    graph.insert_arc(teacher_node, timeblock_node, {1, 0});
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

std::vector<std::tuple<std::string, std::string, int, int>> NetworkGraph::get_final_assignments() const
{
    std::vector<std::tuple<std::string, std::string, int, int>> assignments;

    for (const auto& [subject_id, subject_node] : subject_nodes)
    {
        for (auto arc_st : graph.adjacent_arcs(subject_node))
        {
            if (arc_st->get_info().flow > 0)
            {
                Node teacher_node = arc_st->get_tgt_node();
                std::string teacher_id = teacher_node->get_info().id;

                for (auto arc_tt : graph.adjacent_arcs(teacher_node))
                {
                    if (arc_tt->get_info().flow > 0)
                    {
                        Node timeblock_node = arc_tt->get_tgt_node();
                        const std::string& time_id = timeblock_node->get_info().id;

                        int day, hour;
                        if (sscanf(time_id.c_str(), "D_%dH_%d", &day, &hour) == 2)
                        {
                            assignments.emplace_back(subject_id, teacher_id, day, hour);
                        }
                    }
                }
            }
        }
    }

    return assignments;
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

void NetworkGraph::save_assignment(std::vector<std::tuple<std::string, std::string, int, int>> assignments, const QString& file_path, const Semester& semester)
{
    QJsonDocument doc;
    QFile file(file_path);
    QJsonArray assignments_array;
    if(file.exists() && file.open(QIODevice::ReadOnly))
    {
        doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        assignments_array = doc.array();
    }
    int option_counter = 1;

    for(const auto& item : assignments_array)
    {
        QJsonObject obj = item.toObject();

        if(obj.contains("Option") && obj["Semester"].toString() == semester.get_semester_name())
        {
            QString option = obj["Option"].toString();
            int num = option.mid(7).toInt();
            if(num >= option_counter)
            {
               option_counter = num + 1;
            }
        }
    }

    QJsonObject option;

    option["Option"] = "Opción " + QString::number(option_counter);
    option["Semester"] = QString::fromStdString(semester.get_semester_name());

    QJsonArray new_assignments_array;

    for (const auto& [subject_id, teacher_id, day, hour] : assignments)
    {
        QJsonObject assignment;
        assignment["subject_id"] = QString::fromStdString(subject_id);
        assignment["teacher_id"] = QString::fromStdString(teacher_id);
        assignment["day"] = QString::number(day);
        assignment["hour"] = QString::number(hour);
        new_assignments_array.append(assignment);
    }
    option["Assignment"] = new_assignments_array;
    assignments_array.append(option);

    if(file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(assignments_array).toJson());
        file.close();
    }
    else
    {
        qWarning() << "No se pudo abrir el archivo para escritura:" << file_path;
    }
}

/*
void Teacher::save_teachers_json(const std::vector<Teacher>& teachers, const QString& file_path)
{
    QJsonArray teachers_array;
    for(const auto& teacher : teachers)
    {
        QJsonObject teacher_obj;
        teacher_obj["nombre"] = QString::fromStdString(teacher.get_full_name());
        teacher_obj["cedula"] = QString::fromStdString(teacher.get_id());

        QJsonArray materias_array;
        for (const auto& subject : teacher.get_subjects())
        {
            materias_array.append(QString::fromStdString(subject.get_id()).toInt());
        }
        teacher_obj["materias"] = materias_array;
        QJsonArray weekly_schedule_array;
        for (int day = 0; day < 7; day++)
        {
            QJsonArray day_array;
            for (int hour = 0; hour < 12; hour++)
            {
                TimeBlock block = teacher.get_timeblock(day, hour);
                QJsonObject block_obj;
                QString state;

                if(block.state == BlockState::DISPONIBLE)
                {
                    state = "DISPONIBLE";
                }
                else if(block.state == BlockState::OCUPADO)
                {
                    state = "OCUPADO";
                }
                else
                {
                    state = "NO_DISPONIBLE";
                }

                block_obj["state"] = state;
                block_obj["id_subject"] = QString::fromStdString(block.id_subject);
                day_array.append(block_obj);
            }
            weekly_schedule_array.append(day_array);
        }
        teacher_obj["weekly_schedule"] = weekly_schedule_array;
        teachers_array.append(teacher_obj);
    }
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "No se pudo abrir el archivo para escritura:" << file_path;
    }
    QJsonDocument doc(teachers_array);
    file.write(doc.toJson());
    file.close();
}*/


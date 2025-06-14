#include "networkgraph.h"

using Node = Designar::Digraph<NetworkGraph::NodeData, NetworkGraph::ArcData>::Node*;

NetworkGraph::NetworkGraph(const std::vector<Professor>& professors_, const std::vector<Subject>& subjects_)
    : professors(professors_), subjects(subjects_)
{
    build_network();
}

void NetworkGraph::build_network()
{
    source_node = graph.insert_node({NodeType::Source, std::numeric_limits<int>::max(), 0, "source"});
    sink_node = graph.insert_node({NodeType::Sink, std::numeric_limits<int>::max(), 0, "sink"});


    for (const auto& subject : subjects)
    {
        Node subject_node = graph.insert_node({NodeType::Subject, subject.get_required_hours(), 0, subject.get_id()});
        subject_nodes[subject.get_id()] = subject_node;
        graph.insert_arc(source_node, subject_node, {subject.get_required_hours(), 0});
    }


    for (const auto& professor : professors)
    {
        Node professor_node = graph.insert_node({NodeType::Professor, 0, 0, professor.get_id()});
        professor_nodes[professor.get_id()] = professor_node;


        for (const auto& subject : professor.get_subjects())
        {
            if (subject_nodes.count(subject.get_id()))
            {
                graph.insert_arc(subject_nodes[subject.get_id()], professor_node, {subject.get_required_hours(), 0});
            }
        }


        for (int day = 0; day < 7; ++day)
        {
            for (int hour = 0; hour < 12; ++hour)
            {
                if (professor.available_block(day, hour))
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

                    graph.insert_arc(professor_node, timeblock_node, {1, 0});
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
        int bottleneck = std::numeric_limits<int>::max();

        for(Node v = sink_node; v != source_node; v = parent[v])
        {
            Node u = parent[v];
            auto arc = graph.search_arc(u, v);
            bottleneck = std::min(bottleneck, arc->get_info().capacity - arc->get_info().flow);

        }

        for(Node v = sink_node; v != source_node; v = parent[v])
        {
            Node u = parent[v];
            auto arc = graph.search_arc(u, v);
            arc->get_info().flow += bottleneck;

            if(auto rev = graph.search_arc(v, u))
            {
                rev->get_info().flow -= bottleneck;

            }
            else
            {
                graph.insert_arc(v, u, {0, -bottleneck});
            }
        }

        total_flow += bottleneck;
    }

    return total_flow;

}

std::vector<Section> NetworkGraph::get_final_assign_section() const
{
    std::vector<Section> sections;
    std::set<std::string> assigned_subjects;
    std::set<std::pair<int, int>> used_time_blocks;
    size_t section_counter = 0;

    std::map<std::pair<std::string, std::string>, std::vector<std::pair<int, int>>> professor_subject_blocks;

    for (const auto& [subject_id, subject_node] : subject_nodes)
    {
        int required_hours = 0;
        bool subject_assigned = false;

        for (const auto& subject : subjects)
        {
            if (subject.get_id() == subject_id)
            {
                required_hours = subject.get_required_hours();
                break;
            }
        }

        if(required_hours == 0)
        {
            continue;
        }

        for (auto arc_subject : graph.adjacent_arcs(subject_node))
        {
            if (arc_subject->get_info().flow > 0)
            {
                Node professor_node = arc_subject->get_tgt_node();
                std::string professor_id = professor_node->get_info().id;
                std::vector<std::pair<int, int>> available_vector;

                for (auto arc_time : graph.adjacent_arcs(professor_node))
                {
                    if (arc_time->get_info().flow > 0)
                    {
                        Node timeblock_node = arc_time->get_tgt_node();
                        const std::string& time_id = timeblock_node->get_info().id;

                        int day, hour;
                        if (sscanf(time_id.c_str(), "D_%dH_%d", &day, &hour) == 2)
                        {
                            auto time_slot = std::make_pair(day, hour);
                            if (used_time_blocks.count(time_slot) == 0)
                            {
                                available_vector.push_back(time_slot);
                            }
                        }
                    }
                }

                if (available_vector.size() >= required_hours)
                {
                    auto key = std::make_pair(professor_id, subject_id);

                    for (int i = 0; i < required_hours; i++)
                    {
                        auto [day, hour] = available_vector[i];
                        professor_subject_blocks[key].push_back(std::make_pair(day, hour));
                        used_time_blocks.insert(std::make_pair(day, hour));
                    }
                    subject_assigned = true;
                    break;
                }
            }
        }

        if(subject_assigned)
        {
            assigned_subjects.insert(subject_id);
        }
    }

    for (auto& [professor_subject, blocks] : professor_subject_blocks)
    {
        Section section;
        section.set_professor_section(professor_subject.first);
        section.set_subject_section(professor_subject.second);
        section.set_id_section(section_counter++);

        std::sort(blocks.begin(), blocks.end(), [](const auto& a, const auto& b)
        {
            return a.first < b.first || (a.first == b.first && a.second < b.second);
        });

        section.set_blocks(blocks);
        sections.push_back(section);
    }

    if(assigned_subjects.size() < subject_nodes.size())
    {
        return {};
    }

    return sections;
}

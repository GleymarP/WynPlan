#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <graph.hpp>
#include "definitions.h"

struct NodeInfo
{
    /*std::string nombre;
    int tipo; // 0: profesor, 1: materia, 2: horario, etc.*/

    std::string id;
    std::string type;  // "source", "sink", "teacher", "subject", "schedule"
    std::string label;
};
struct ArcInfo {
    int capacidad;
    int flujo;
    std::string description;
    // otros campos relevantes
};
struct GraphInfo {
    std::string nombre_red;
    // otros campos relevantes
};
/*
using RedHorarios = Designar::Digraph<NodeInfo, ArcInfo, GraphInfo>;

class RedHorariosFlujo {
private:
    RedHorarios red;
    RedHorarios::Node* fuente;
    RedHorarios::Node* sumidero;

public:
    RedHorariosFlujo()
    {
        fuente = red.insert_node({"Fuente", -1});
        sumidero = red.insert_node({"Sumidero", -1});
    }
    void build_red(const std::vector<Teacher>& profesores, const std::vector<Subject>& materias, const std::vector<Schedule>& horarios)
    {
        for (const auto& profesor : profesores)
        {
            auto nodoProf = red.insert_node({profesor.get_full_name(), 0});
            // Conectar fuente a profesor con capacidad = max materias que puede dictar
            red.insert_arc(fuente, nodoProf, {3, 0});
        }
        for (const auto& materia : materias) {
            auto nodoMat = red.insert_node({materia.get_subject_name(), 1});
            // Conectar materia a sumidero con capacidad = 1 (cada materia se dicta una vez)
            red.insert_arc(nodoMat, sumidero, {1, 0});
        }
        for (auto prof_node : red.get_nodes()) {
            if (prof_node->get_info().tipo != 0) continue;

            for (auto mat_node : red.get_nodes()) {
                if (mat_node->get_info().tipo != 1) continue;

                // Verificar si este profesor puede dictar esta materia
                if ( lógica de compatibilidad ) {
                    red.insert_arc(prof_node, mat_node, {1, 0}); // Capacidad 1
                }
            }
        }
    }
    bool asignarHorarios();
    void agregarRestricciones();
    int computeMaxFlow(RedHorarios::Node* source, RedHorarios::Node* sink);
    int getFlow(RedHorarios::Node* u, RedHorarios::Node* v) const;
};*/

using ScheduleGraph = Designar::Digraph<NodeInfo, ArcInfo, std::string>;
struct Assignment {
    std::string teacher;
    std::string subject;
    std::string schedule;
    std::string semester;
};

class ScheduleManager {
private:
    ScheduleGraph graph;
    ScheduleGraph::Node* source;
    ScheduleGraph::Node* sink;
    std::unordered_map<std::string, ScheduleGraph::Node*> teacher_nodes;
    std::unordered_map<std::string, ScheduleGraph::Node*> subject_nodes;
    std::unordered_map<std::string, ScheduleGraph::Node*> schedule_nodes;

    // Datos del problema
    std::vector<Teacher> teachers;
    StudyPlan study_plan;

public:
    ScheduleManager(const std::vector<Teacher>& teachers_, const StudyPlan& plan)
        : teachers(teachers_), study_plan(plan) {

        // Inicializamos la red con fuente y sumidero
        source = graph.insert_node({"source", "source", "Source"});
        sink = graph.insert_node({"sink", "sink", "Sink"});

        buildNetwork();
    }

    // Métodos principales
    bool assignSchedules();
    std::vector<Assignment> getAssignments() const;

private:
    void buildNetwork();
    void addTeachersToNetwork();
    void addSubjectsToNetwork();
    void addSchedulesToNetwork();
    void connectTeachersToSubjects();
    void connectSubjectsToSchedules();
    void addCollisionConstraints();

    // Algoritmo de flujo
    bool findAugmentingPath(std::vector<ScheduleGraph::Arc*>& path, int& min_flow);
    void updateResidualGraph(const std::vector<ScheduleGraph::Arc*>& path, int flow);
};

// Estructura para resultados


#endif // NETWORKGRAPH_H

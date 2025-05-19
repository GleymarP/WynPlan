#include "definitions.h"
#include "networkgraph.h"
#include <QApplication>
#include <homepage.h>
#include <studyplanwindow.h>
#include <QStandardPaths>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path_json = ":/resources/flujograma.json";
    StudyPlan plan = StudyPlan::load_from_json(path_json);

    /*qDebug() << "\n=== DETALLES DEL PLAN DE ESTUDIOS ===";
    qDebug() << "Nombre del programa:" << QString::fromStdString(plan.get_degree());
    qDebug() << "Total de semestres:" << plan.get_semester().size();*/

    auto semesters = plan.get_semester();
    Semester first_semester;

    for(const auto& semester : semesters)
    {
        if(semester.get_semester_name() == "Semestre 1")
        {
            first_semester = semester;
            break;
        }
    }

    QString path_json_teacher = ":/resources/teachers.json";
    std::vector<Teacher> teachers = Teacher::load_from_json(path_json_teacher, plan);

    NetworkGraph network(teachers, first_semester.get_subjects_semester());
    network.print_graph();
    auto max = network.max_flow();
    network.print_graph();


    auto assignments = network.get_final_assignments();
    //Teacher::update_teachers_with_assignments(teachers, assignments);
    //Teacher::save_teachers_json(teachers, QCoreApplication::applicationDirPath() + "/../../resources/teachers.json");
    if(!assignments.empty())
    {
        NetworkGraph::save_assignment(assignments, QCoreApplication::applicationDirPath() + "/../../resources/assigments.json", first_semester);
    }
    //std::vector<Assigment> assign = Assigment::load_from_json(QCoreApplication::applicationDirPath() + "/../../resources/assigments.json", plan, teachers);
    //Teacher::save_teachers_json(teachers, QCoreApplication::applicationDirPath() + "/../../resources/teachers.json");

    for (const auto& [subject_id, teacher_id, day, hour] : assignments)
    {
        std::cout << "Subject: " << subject_id
                  << ", Teacher: " << teacher_id
                  << ", Day: " << day
                  << ", Hour: " << hour << std::endl;
    }

    HomePage homepage;
    homepage.show();

    return a.exec();
}

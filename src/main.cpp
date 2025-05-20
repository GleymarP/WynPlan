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

    QString path_json_teacher = ":/resources/teachers.json";
    std::vector<Teacher> teachers = Teacher::load_from_json(path_json_teacher, plan);

    QString path_assign_json = QCoreApplication::applicationDirPath() + "/../../resources/assign.json";
    std::vector<Assigment> assign = Assigment::load_from_json_assing(path_assign_json, plan, teachers);

    auto semesters = plan.get_semester();
    Semester first_semester;

    for(const auto& semester : semesters)
    {
        if(semester.get_semester_name() == "Semestre 2")
        {
            first_semester = semester;
            break;
        }
    }

    NetworkGraph network(teachers, first_semester.get_subjects_semester());
    network.print_graph();
    auto max = network.max_flow();
    network.print_graph();

    auto section_veector = network.get_final_assign_section();
    if(!section_veector.empty())
    {
        Assigment assignments;
        assignments.set_sections_vector(section_veector);
        assignments.set_semester_name(first_semester.get_semester_name());
        assignments.set_option("Opción 4");
        assign.push_back(assignments);
        Assigment::save_assigments_json(assign, path_assign_json, plan);
        assign = Assigment::load_from_json_assing(path_assign_json, plan, teachers);
        Teacher::save_teachers_json(teachers, QCoreApplication::applicationDirPath() + "/../../resources/teachers.json");
        teachers = Teacher::load_from_json(path_json_teacher, plan);
    }

    Semester second_semester;

    for(const auto& semester : semesters)
    {
        if(semester.get_semester_name() == "Semestre 2")
        {
            second_semester = semester;
            break;
        }
    }


    HomePage homepage;
    homepage.show();

    return a.exec();
}

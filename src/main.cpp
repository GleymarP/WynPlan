#include "definitions.h"
#include "networkgraph.h"
#include <QApplication>
#include <homepage.h>
#include <studyplanwindow.h>

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

   /* qDebug() << "=== PROFESORES CARGADOS ===";
    qDebug() << "Total de profesores:" << teachers.size();
    qDebug() << "-----------------------------------";

    for (const Teacher& teacher : teachers) {
        qDebug() << "\nProfesor:" << QString::fromStdString(teacher.get_full_name());
        qDebug() << "cedula: " << QString::fromStdString(teacher.get_id());

        qDebug() << "Horarios disponibles:";
        for (const Schedule& avail : teacher.get_availability()) {
            qDebug() << "- Día:" << QString::fromStdString(avail.get_day())
            << "de" << QString::fromStdString(avail.get_start_time())
            << "a" << QString::fromStdString(avail.get_end_time());
        }

        qDebug() << "Materias:";
        for (const Subject& subject : teacher.get_subjects()) {
            qDebug() << "-" << QString::fromStdString(subject.get_subject_name())
            << "(Código:" << QString::fromStdString(subject.get_id())
            << ")";
        }
    }*/

    NetworkGraph network(teachers, first_semester.get_subjects_semester());
    network.print_graph();

    StudyPlanWindow studywindow(plan);
    studywindow.show();
    //HomePage homepage;
    //homepage.show();

    return a.exec();
}

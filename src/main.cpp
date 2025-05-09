#include "mainwindow.h"
#include "definitions.h"
#include "networkgraph.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path_json = ":/resources/flujograma.json";
    StudyPlan plan = StudyPlan::load_from_json(path_json);

    qDebug() << "\n=== DETALLES DEL PLAN DE ESTUDIOS ===";
    qDebug() << "Nombre del programa:" << QString::fromStdString(plan.get_degree());
    qDebug() << "Total de semestres:" << plan.get_semester().size();


    QString path_json_teacher = ":/resources/teachers.json";
    std::vector<Teacher> teachers = Teacher::load_from_json(path_json_teacher, plan);

    qDebug() << "=== PROFESORES CARGADOS ===";
    qDebug() << "Total de profesores:" << teachers.size();
    qDebug() << "-----------------------------------";

    for (const Teacher& teacher : teachers) {
        qDebug() << "\nProfesor:" << QString::fromStdString(teacher.get_full_name());

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
    }

    NetworkGraph network(teachers, plan);

    network.printGraph();


    MainWindow w;
    w.show();
    return a.exec();
}

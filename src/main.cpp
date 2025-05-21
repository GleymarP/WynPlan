#include "definitions.h"
#include "networkgraph.h"
#include <QApplication>
#include <homepage.h>
#include <studyplanwindow.h>
#include <QStandardPaths>
#include <QDir>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path_json = ":/resources/flujograma.json";
    StudyPlan plan = StudyPlan::load_from_json(path_json);

    QString path_json_teacher = ":/resources/teachers.json";
    std::vector<Teacher> teachers = Teacher::load_from_json(path_json_teacher, plan);

    QString path_assign_json = QCoreApplication::applicationDirPath() + "/../../resources/assign.json";
    std::vector<Assigment> assign = Assigment::load_from_json_assing(path_assign_json, plan, teachers);


    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::ButtonText, Qt::black);
    a.setPalette(palette);


    HomePage homepage;
    homepage.show();

    return a.exec();
}

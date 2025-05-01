#include "mainwindow.h"
#include "definitions.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path_json = ":/resources/flujograma.json";
    StudyPlan plan = StudyPlan::load_from_json(path_json);

    QString path_json_teacher = ":/resources/teachers.json";

    std::vector<Teacher> teachers = Teacher::load_from_json(path_json_teacher, plan);


    MainWindow w;
    w.show();
    return a.exec();
}

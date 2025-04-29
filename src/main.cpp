#include "mainwindow.h"
#include "definitions.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path_json = ":/flujograma.json";
    StudyPlan plan;

    plan.load_from_json(path_json);

    MainWindow w;
    w.show();
    return a.exec();
}

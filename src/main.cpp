#include "definitions.h"
#include "networkgraph.h"
#include <QApplication>
#include <homepage.h>
#include <studyplanwindow.h>
#include <QStandardPaths>
#include <QDir>
#include <QPalette>

void create_json_if_not_exists(const QString& path, const QJsonDocument& default_doc)
{
    QFile file(path);
    if (!file.exists())
    {
        QDir().mkpath(QFileInfo(path).absolutePath());

        if (file.open(QIODevice::WriteOnly))
        {
            file.write(default_doc.toJson(QJsonDocument::Indented));
            file.close();
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QJsonArray default_array;
    QString path_assign_json = QCoreApplication::applicationDirPath() + "/../../resources/assign.json";
    QString path_teachers_json = QCoreApplication::applicationDirPath() + "/../../resources/teachers.json";
    QString path_plan_json = QCoreApplication::applicationDirPath() + "/../../resources/flujograma.json";

    create_json_if_not_exists(path_assign_json, QJsonDocument(default_array));
    create_json_if_not_exists(path_teachers_json, QJsonDocument(default_array));
    create_json_if_not_exists(path_plan_json, QJsonDocument(default_array));


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

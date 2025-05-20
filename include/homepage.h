#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <studyplanwindow.h>
#include <teacherswindow.h>
#include <schedulewindow.h>
#include <sectionwindow.h>

namespace Ui {
class HomePage;
}

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:

    void on_teacher_button_clicked();
    void on_studyplan_button_clicked();

    void on_section_button_clicked();

    void on_schedule_button_clicked();

private:
    Ui::HomePage *ui;
    const QString path_studyplan_json = ":/resources/flujograma.json";
    const QString path_teachers_json = ":/resources/teachers.json";
    const QString path_assigments_json = QCoreApplication::applicationDirPath() + "/../../resources/assigments.json";
    const QString path_assign_json = QCoreApplication::applicationDirPath() + "/../../resources/assign.json";
    StudyPlan plan;
    std::vector<Teacher> teachers;
    std::vector<Assigment> assigments;
};

#endif // HOMEPAGE_H

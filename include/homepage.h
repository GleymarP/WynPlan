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
    StudyPlan plan;
    std::vector<Teacher> teachers;

};

#endif // HOMEPAGE_H

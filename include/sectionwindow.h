#ifndef SECTIONWINDOW_H
#define SECTIONWINDOW_H

#include <QMainWindow>
#include "definitions.h"
#include <sectiondialog.h>
#include <assignprofessordialog.h>

namespace Ui {
class SectionWindow;
}

class SectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SectionWindow(StudyPlan& plan_, std::vector<Assigment>& assigments_, std::vector<Professor>& professors_, QWidget *parent = nullptr);
    ~SectionWindow();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_menu_clicked();

    void on_comboBox_semester_currentTextChanged(const QString &arg1);

    void on_comboBox_option_currentTextChanged(const QString &arg1);

    void on_pushButton_delete_section_clicked();

    void on_pushButton_assign_professor_clicked();

    bool professor_to_be_assigned();

    void update_professors_schedule_from_assigments(Assigment update);

private:
    Ui::SectionWindow *ui;
    std::vector<Assigment> assigments;
    std::vector<Professor> professors;
    StudyPlan plan;
    Assigment current_assigment;
    std::string current_txt;

};

std::string get_day(int day);
QString get_hour(int hour);

#endif // SECTIONWINDOW_H

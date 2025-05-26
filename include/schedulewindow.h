#ifndef SCHEDULEWINDOW_H
#define SCHEDULEWINDOW_H

#include <QMainWindow>
#include "definitions.h"
#include <networkgraph.h>
#include <QMessageBox>

namespace Ui {
class ScheduleWindow;
}

class ScheduleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScheduleWindow(StudyPlan& plan, std::vector<Professor>& professors, std::vector<Assigment>& assigments, QWidget *parent = nullptr);
    ~ScheduleWindow();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_menu_clicked();

    void on_pushButton_generate_schedule_clicked();

    void on_comboBox_schedule_currentTextChanged(const QString &arg1);

    void update_table();

    void on_pushButton_read_clicked();

    void on_comboBox_options_windowIconTextChanged(const QString &iconText);

private:

    Ui::ScheduleWindow *ui;
    StudyPlan plan_;
    std::vector<Professor> professors_;
    Semester current_semester;
    std::string current_option;
    std::vector<Assigment> assignments_;
    QString path_assign_json = QCoreApplication::applicationDirPath() + "/../../resources/assign.json";

};

#endif // SCHEDULEWINDOW_H

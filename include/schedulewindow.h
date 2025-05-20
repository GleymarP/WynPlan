#ifndef SCHEDULEWINDOW_H
#define SCHEDULEWINDOW_H

#include <QMainWindow>
#include "definitions.h"
#include <networkgraph.h>

namespace Ui {
class ScheduleWindow;
}

class ScheduleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScheduleWindow(StudyPlan& plan, std::vector<Teacher>& teachers, QWidget *parent = nullptr);
    ~ScheduleWindow();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_menu_clicked();

    void on_pushButton_clicked();



    void on_pushButton_generate_schedule_clicked();

    void on_comboBox_schedule_currentTextChanged(const QString &arg1);

private:

    Ui::ScheduleWindow *ui;
    StudyPlan plan_;
    std::vector<Teacher> teachers_;
    Semester current_semester;
};

#endif // SCHEDULEWINDOW_H

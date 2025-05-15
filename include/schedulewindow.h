#ifndef SCHEDULEWINDOW_H
#define SCHEDULEWINDOW_H

#include <QMainWindow>
#include "definitions.h"

namespace Ui {
class ScheduleWindow;
}

class ScheduleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScheduleWindow(StudyPlan& plan, std::vector<Teacher>& teacher, QWidget *parent = nullptr);
    ~ScheduleWindow();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_menu_clicked();

private:
    StudyPlan plan_;
    std::vector<Teacher>& teacher_;
    Ui::ScheduleWindow *ui;
};

#endif // SCHEDULEWINDOW_H

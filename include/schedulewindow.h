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
    explicit ScheduleWindow(StudyPlan& plan, QWidget *parent = nullptr);
    ~ScheduleWindow();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_menu_clicked();

    void on_pushButton_clicked();

private:

    Ui::ScheduleWindow *ui;
    StudyPlan plan_;
};

#endif // SCHEDULEWINDOW_H

#ifndef SCHEDULEWINDOW_H
#define SCHEDULEWINDOW_H

#include <QMainWindow>

namespace Ui {
class ScheduleWindow;
}

class ScheduleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScheduleWindow(QWidget *parent = nullptr);
    ~ScheduleWindow();

private:
    Ui::ScheduleWindow *ui;
};

#endif // SCHEDULEWINDOW_H

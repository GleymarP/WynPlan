#include "schedulewindow.h"
#include "ui_schedulewindow.h"

ScheduleWindow::ScheduleWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScheduleWindow)
{
    ui->setupUi(this);
}

ScheduleWindow::~ScheduleWindow()
{
    delete ui;
}

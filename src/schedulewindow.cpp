#include "schedulewindow.h"
#include "ui_schedulewindow.h"

ScheduleWindow::ScheduleWindow(StudyPlan& plan, std::vector<Teacher>& teacher, QWidget *parent)
    : QMainWindow(parent), plan_(plan), teacher_(teacher)
    , ui(new Ui::ScheduleWindow)
{
    ui->setupUi(this);
}

ScheduleWindow::~ScheduleWindow()
{
    delete ui;
}

void ScheduleWindow::on_pushButton_menu_clicked()
{
    emit back_to_menu();
    this->close();
}


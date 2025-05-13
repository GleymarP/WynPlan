#include "studyplanwindow.h"
#include "ui_studyplanwindow.h"

StudyPlanWindow::StudyPlanWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudyPlanWindow)
{
    ui->setupUi(this);
}

StudyPlanWindow::~StudyPlanWindow()
{
    delete ui;
}

#include "sectionwindow.h"
#include "ui_sectionwindow.h"

SectionWindow::SectionWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SectionWindow)
{
    ui->setupUi(this);
}

SectionWindow::~SectionWindow()
{
    delete ui;
}

void SectionWindow::on_pushButton_menu_clicked()
{
    emit back_to_menu();
    this->close();
}


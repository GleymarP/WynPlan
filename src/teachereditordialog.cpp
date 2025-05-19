#include "teachereditordialog.h"
#include "ui_teachereditordialog.h"

TeacherEditorDialog::TeacherEditorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TeacherEditorDialog)
{
    ui->setupUi(this);
}

TeacherEditorDialog::~TeacherEditorDialog()
{
    delete ui;
}

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

void TeacherEditorDialog::set_teacher_info(const Teacher &teacher)
{
    ui->label_title->setText("Eliminar profesor");
    ui->label_teacger_id->setText("Cédula: " + QString::fromStdString(teacher.get_id()));
    ui->label_teacher_name->setText("Nombre: " + QString::fromStdString(teacher.get_full_name()));
    ui->label_teacher_subjects->setText("Materias:");
    ui->label_question->setText("¿Desea eliminarlo?");
    ui->listWidget_subjects->clear();


    for(const auto& subject : teacher.get_subjects())
    {
        ui->listWidget_subjects->addItem(QString::fromStdString(subject.get_subject_name()));
    }

}


void TeacherEditorDialog::on_pushButton_ok_clicked()
{
    confirm_delete = true;
    accept();
}


void TeacherEditorDialog::on_pushButton_cancel_clicked()
{
    confirm_delete = false;
    reject();
}

bool TeacherEditorDialog::is_deletion_confirmed() const
{
    return confirm_delete;
}

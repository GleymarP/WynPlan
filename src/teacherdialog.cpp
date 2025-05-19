#include "teacherdialog.h"
#include "ui_teacherdialog.h"

TeacherDialog::TeacherDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TeacherDialog)
{
    ui->setupUi(this);
}

TeacherDialog::~TeacherDialog()
{
    delete ui;
}

void TeacherDialog::set_edit_mode(bool editing)
{
    is_editing = editing;
    if (is_editing)
    {
        ui->label_title->setText("Modificar profesor");
        ui->label_question->setText("¿Deseas modificar los datos del profesor?");
    }
    else
    {
        ui->label_title->setText("Agregar nuevo profesor");
        ui->label_question->setText("¿Deseas agregar nuevo profesor?");
        ui->lineEdit_id->clear();
        ui->lineEdit_name->clear();
    }

}

void TeacherDialog::set_teacher(const Teacher &teacher)
{
    if(is_editing)
    {
        editable_teacher = teacher;
        ui->lineEdit_name->setText(QString::fromStdString(teacher.get_full_name()));
        ui->lineEdit_id->setText(QString::fromStdString(teacher.get_id()));
    }
}

Teacher TeacherDialog::get_update_teacher() const
{
    return editable_teacher;
}

void TeacherDialog::on_pushButton_ok_clicked()
{
    if(is_editing)
    {
        QString id = ui->lineEdit_id->text().trimmed();
        QString name = ui->lineEdit_name->text().trimmed();

        if(id.isEmpty() || name.isEmpty())
        {
            QMessageBox::warning(this, "Campos vacios", "Por favor, llene todos los campos");
            return;
        }

        static const QRegularExpression format_id("^[VE]-\\d{6,8}$");
        if(!format_id.match(id).hasMatch())
        {
            QMessageBox::warning(this, "Formato inválido", "Ingrese una cédula válida (ej: V-12345678, E-8765432)." );
            return;
        }

        editable_teacher.set_id(id.toStdString());
        editable_teacher.set_full_name(name.toStdString());

        accept();
    }

}


void TeacherDialog::on_pushButton_cancel_clicked()
{
    reject();
}


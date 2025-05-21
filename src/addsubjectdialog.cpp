#include "addsubjectdialog.h"
#include "ui_addsubjectdialog.h"

AddSubjectDialog::AddSubjectDialog(std::vector<Semester>& semestres, std::vector<Subject>& subjects, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddSubjectDialog)
    , semestres_(semestres)
    , subjects_(subjects)
{
    ui->setupUi(this);
    ui->label_question->setText("¿Desea añadir esta materia?");
}

AddSubjectDialog::~AddSubjectDialog()
{
    delete ui;
}

Subject AddSubjectDialog::get_subject()
{
    return subject;
}

void AddSubjectDialog::on_pushButton_ok_clicked()
{
    QString id = ui->textEdit_id_subject->toPlainText().simplified();
    QString name = ui->textEdit_subject_name->toPlainText().simplified();

    if(id.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Campos vacios", "Por favor, llene todos los campos");
        return;
    }

    std::string text_id = id.toStdString();
    std::string text_name = name.toStdString();
    bool found = false;

    ui->label_aviso_repetido_id->clear();
    ui->label_aviso_repetido_nombre->clear();

    for(Semester& semester : semestres_)
    {
        for(Subject& subject : semester.get_subjects_semester())
        {
            if(text_id == subject.get_id())
            {
                ui->label_aviso_repetido_id->setText("Código de materia repetido");
                found = true;
                break;
            }
            if(text_name == subject.get_subject_name())
            {
                ui->label_aviso_repetido_nombre->setText("Nombre de materia repetido");
                found = true;
                break;
            }
        }
        if(found == true)
        {
            break;
        }
    }
    if(found != true)
    {
        for(Subject& subject : subjects_)
        {
            if(text_id == subject.get_id())
            {
                ui->label_aviso_repetido_id->setText("Código de materia repetido en este semestre");
                found = true;
                break;
            }
            if(text_name == subject.get_subject_name())
            {
                ui->label_aviso_repetido_nombre->setText("Nombre de materia repetido en este semestre");
                found = true;
                break;
            }
        }
    }

    if(found == true)
    {
        QMessageBox::warning(this, "Campos incorrectos", "Por favor, cambie el código y/o el  nombre de la materia");
        return;
    }

    int hours_per_week = ui->spinBox_hours_week->value();
    if(hours_per_week == 0)
    {
        QMessageBox::warning(this, "Hora inválida", "Por favor, seleccione una cantidad de horas por semana distinta de 0");
        return;
    }

    subject.set_id(text_id);
    subject.set_name(text_name);
    subject.set_hours(hours_per_week);
    confirm_create = true;
    accept();
}


void AddSubjectDialog::on_pushButton_cancel_clicked()
{
    confirm_create = false;
    reject();
}

bool AddSubjectDialog::is_creation_confirmed() const
{
    return confirm_create;
}

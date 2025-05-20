#include "sectiondialog.h"
#include "ui_sectiondialog.h"

sectiondialog::sectiondialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::sectiondialog)
{
    ui->setupUi(this);

}

sectiondialog::~sectiondialog()
{
    delete ui;
}

void sectiondialog::set_option_info(Assigment& assigment)
{
    ui->label_title->setText("Eliminar Opción");
    ui->label_option->setText("Opción:");
    ui->label_semester->setText("Semester:");
    ui->label_subjects->setText("Información:");
    ui->label_question->setText("¿Desea eliminarlo?");
    ui->listWidget->clear();
    ui->lineEdit_semester->setText(QString::fromStdString(assigment .get_semester_name()));
    ui->lineEdit_option->setText(QString::fromStdString(assigment.get_option()));

    std::vector<Section> sections = assigment.get_sections_vector();
    for(Section&  section : sections)
    {
        ui->listWidget->addItem("Código de la materia:");
        ui->listWidget->addItem(QString::fromStdString(section.get_subject_section()));
        ui->listWidget->addItem("ID del profesor:");
        ui->listWidget->addItem(QString::fromStdString(section.get_teacher_section()));
        ui->listWidget->addItem("\n");
    }
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
}

void sectiondialog::on_pushButton_ok_clicked()
{
    confirm_delete = true;
    accept();
}


void sectiondialog::on_pushButton_cancel_clicked()
{
    confirm_delete = false;
    reject();
}

bool sectiondialog::is_deletion_confirmed() const
{
    return confirm_delete;
}

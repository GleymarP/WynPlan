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

void TeacherDialog::set_professor(const Professor &professor)
{
    if(is_editing)
    {
        editable_professor = professor;
        ui->lineEdit_name->setText(QString::fromStdString(professor.get_full_name()));
        ui->lineEdit_id->setText(QString::fromStdString(professor.get_id()));
    }
}

Professor TeacherDialog::get_update_professor() const
{
    return editable_professor;
}

void TeacherDialog::set_available_subjects(const std::vector<Subject> &all_subjects)
{
    available_subjects = all_subjects;

    ui->listWidget_subjects->clear();

    for(const auto& subject : all_subjects)
    {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(subject.get_subject_name()));
        item->setData(Qt::UserRole, QString::fromStdString(subject.get_id()));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget_subjects->addItem(item);
    }

    for(int i = 0; i < ui->listWidget_subjects->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget_subjects->item(i);
        QString id = item->data(Qt::UserRole).toString();
        for(const auto& subject : editable_professor.get_subjects())
        {
            if(QString::fromStdString(subject.get_id()) == id)
            {
                item->setCheckState(Qt::Checked);
                break;
            }
        }
    }


}

void TeacherDialog::on_pushButton_ok_clicked()
{

    QString id = ui->lineEdit_id->text().trimmed();
    QString name = ui->lineEdit_name->text().trimmed();
    QString name_uppercase = get_uppercase(name);

    if(id.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Campos vacios", "Por favor, llene todos los campos");
        return;
    }

    static const QRegularExpression format_id("^[VE]-\\d+$");

    if(!format_id.match(id).hasMatch())
    {
        QMessageBox::warning(this, "Formato inválido", "Ingrese una cédula válida (ej: V-12345678, E-8765432)." );
        return;
    }

    std::vector<Subject> seleted_subjects;

    for(int i = 0; i < ui->listWidget_subjects->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget_subjects->item(i);
        if(item->checkState() == Qt::Checked)
        {
            QString id = item->data(Qt::UserRole).toString();

            auto it = std::find_if(available_subjects.begin(), available_subjects.end(), [&](const Subject& subj)
            {
                return QString::fromStdString(subj.get_id()) == id;
            });

            if (it != available_subjects.end())
            {
                seleted_subjects.push_back(*it);
            }
        }
    }

    if(seleted_subjects.empty())
    {
        QMessageBox::warning(this, "Materias vacías", "Debe seleccionar al menos una materia.");
        return;
    }

    editable_professor.set_subjects(seleted_subjects);
    editable_professor.set_id(id.toStdString());
    editable_professor.set_full_name(name_uppercase.toStdString());

    accept();

}

QString TeacherDialog::get_uppercase(QString input)
{
    QStringList words = input.toLower().split(' ', Qt::SkipEmptyParts);
    for(auto& word : words)
    {
        if(!word.isEmpty())
        {
            word[0] = word[0].toUpper();
        }
    }

    return words.join(' ');
}

void TeacherDialog::on_pushButton_cancel_clicked()
{
    reject();
}


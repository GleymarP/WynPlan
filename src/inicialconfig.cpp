#include "inicialconfig.h"
#include "ui_inicialconfig.h"

InicialConfig::InicialConfig(StudyPlan& plan, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InicialConfig)
    , plan_(plan)
{
    ui->setupUi(this);

    ui->textEdit_name_degree->show();
    ui->spinBox_n_semestres->show();
    ui->label_degree->show();
    ui->label_number_semesters->show();
    ui->pushButton_save_n_semesters->show();

    ui->rectangel->hide();
    ui->label_name_semester->hide();
    ui->label_materias->hide();
    ui->pushButton_add_subject->hide();
    ui->listWidget->hide();
    ui->label_save_materias->hide();
    ui->pushButton_save_semester->hide();
    ui->pushButton_delete_subject->hide();
}

InicialConfig::~InicialConfig()
{
    delete ui;
}

void InicialConfig::on_pushButton_add_subject_clicked()
{
    AddSubjectDialog dialog(semesters_vector, subjects_vector, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        Subject new_subject = dialog.get_subject();
        subjects_vector.push_back(new_subject);
        QMessageBox::information(this, "Agregado", "Nueva materia agregada");
        update_window();
    }
}

void InicialConfig::update_window()
{
    ui->listWidget->clear();
    for(Subject& subject : subjects_vector)
    {
        ui->listWidget->addItem(QString::fromStdString(subject.get_subject_name()));
    }
}

void InicialConfig::on_pushButton_save_n_semesters_clicked()
{
    QString name_degree = ui->textEdit_name_degree->toPlainText().trimmed();
    if(name_degree.isEmpty())
    {
        QMessageBox::warning(this, "Campo vacío", "Por favor, llene todos los campos");
        return;
    }

    int number_semesters = ui->spinBox_n_semestres->value();
    if(number_semesters ==  0)
    {
        QMessageBox::warning(this, "Cantidad inválida", "Por favor, ingrese un número válido de semestres");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Guardar datos", QString("¿Desea guardar estos datos de nombre de la materia y cantidad de semestres?"), QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        plan_.set_degree(name_degree.toStdString());
        for(int i = 1; i <= number_semesters; i++)
        {
            semester_vector_name.push_back("Semestre " + QString::number(i).toStdString());
        }
        ui->textEdit_name_degree->hide();
        ui->spinBox_n_semestres->hide();
        ui->label_degree->hide();
        ui->label_number_semesters->hide();
        ui->pushButton_save_n_semesters->hide();
        update_next();
    }
}

void InicialConfig::on_pushButton_save_semester_clicked()
{
    if(subjects_vector.empty())
    {
        QMessageBox::warning(this, "No se han agregado materias", "Por favor, ingrese las materias del semestre");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Guardar materias", QString("¿Desea guardar estas materias en este semestre?"), QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        Semester semester;
        QString name = ui->label_name_semester->text();
        std::string text_name = name.toStdString();

        semester.set_semester_name(text_name);
        semester.set_subjects(subjects_vector);
        semesters_vector.push_back(semester);
        subjects_vector.clear();
        var++;

        if(var < semester_vector_name.size())
        {
            update_next();
        }
        else
        {
            plan_.set_semester(semesters_vector);
            StudyPlan::save_studyplan_json(plan_, QCoreApplication::applicationDirPath() + "/../../resources/flujograma.json");
            emit back_to_menu();
            this->close();
        }
    }
}

void InicialConfig::update_next()
{
    ui->rectangel->show();
    ui->label_name_semester->show();
    ui->label_materias->show();
    ui->pushButton_add_subject->show();
    ui->listWidget->show();
    ui->label_save_materias->show();
    ui->pushButton_save_semester->show();

    ui->textEdit_name_degree->hide();
    ui->spinBox_n_semestres->hide();
    ui->label_degree->hide();
    ui->label_number_semesters->hide();
    ui->pushButton_save_n_semesters->hide();

    ui->label_name_semester->setText(QString::fromStdString(semester_vector_name[var]));
    ui->listWidget->clear();

    for(Subject& subject : subjects_vector)
    {
        ui->listWidget->addItem(QString::fromStdString(subject.get_subject_name()));
    }
    ui->pushButton_delete_subject->hide();
}

void InicialConfig::on_pushButton_delete_subject_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();

    if(subjects_vector.empty())
    {
        QMessageBox::warning(this, "No hay materias", "No hay materias para eliminar");
        return;
    }

    if(!item)
    {
        QMessageBox::warning(this, "Ninguna materia seleccionada", "Por favor, seleccione una materia para eliminar");
        return;
    }

    QString textoQString = item->text().trimmed();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Eliminar materia", QString("¿Desea eliminar esta materia: " + textoQString + " ?"), QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        std::vector<Subject> new_subjects;
        for(Subject& subject : subjects_vector)
        {
            if(textoQString.toStdString() != subject.get_subject_name())
            {
                new_subjects.push_back(subject);
            }
        }
        subjects_vector = std::move(new_subjects);
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
}


void InicialConfig::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->pushButton_delete_subject->show();
}


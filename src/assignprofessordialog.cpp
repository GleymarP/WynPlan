#include "assignprofessordialog.h"
#include "ui_assignprofessordialog.h"
#include <sectionwindow.h>

AssignProfessorDialog::AssignProfessorDialog(Assigment current_assigment, std::vector<Professor> professors_, StudyPlan& plan_, QWidget *parent)
    : QDialog(parent)
    , current_assign(current_assigment)
    , professors(professors_)
    , plan(plan_)
    , ui(new Ui::AssignProfessorDialog)
{
    ui->setupUi(this);
    set_data();
}

AssignProfessorDialog::~AssignProfessorDialog()
{
    delete ui;
}

void AssignProfessorDialog::set_data()
{
    std::string texto = "TEMP_";

    for(auto section : current_assign.get_sections_vector())
    {
        std::string professor_section_id = section.get_professor_section();
        if(professor_section_id.find(texto) != std::string::npos)
        {
            ui->comboBox_subjects->addItem(QString::fromStdString(section.get_subject_section()));
        }
    }
}

void AssignProfessorDialog::on_comboBox_subjects_currentTextChanged(const QString &arg1)
{
    std::string current_text = arg1.toStdString();
    std::vector<Subject> subjects;
    bool subject_found = false;

    ui->listWidget_info->clear();
    ui->listWidget_professors->clear();

    for (auto& semester : plan.get_semester())
    {
        if (current_assign.get_semester_name() == semester.get_semester_name())
        {
            for (auto& subject : semester.get_subjects_semester())
            {
                if (subject.get_id() == current_text)
                {
                    current_subj = subject;
                    subject_found = true;
                    break;
                }
            }
            if (subject_found)
            {
                break;
            }
        }
    }

    if (!subject_found)
    {
        ui->listWidget_info->addItem("Materia no encontrada");
        return;
    }

    ui->listWidget_info->addItem("Materia: " + QString::fromStdString(current_subj.get_subject_name()));
    ui->listWidget_info->addItem("Código de la materia: " + QString::fromStdString(current_subj.get_id()));


    for(auto section : current_assign.get_sections_vector())
    {
        std::string professor_section_id = section.get_professor_section();
        std::string subject_id = section.get_subject_section();

        if (section.get_subject_section() == current_subj.get_id())
        {
            std::unordered_map<int, bool> map_days;
            std::vector<std::pair<int, int>> blocks = section.get_assigned_blocks();
            int current_day = -1;
            int hour_start = -1;
            int hour_end = -1;

            for(std::pair<int, int>& pair : blocks)
            {
                int day = pair.first;
                int hour = pair.second;
                if(get_day(day).empty() || get_hour(hour).isEmpty())
                {
                    continue;
                }

                auto find = map_days.find(day);
                if(find == map_days.end())
                {
                    if (current_day != -1)
                    {
                        ui->listWidget_info->addItem(get_hour(hour_start) + " - " + get_hour(hour_end + 1));
                    }
                    ui->listWidget_info->addItem("Día: " + QString::fromStdString(get_day(day)));
                    ui->listWidget_info->addItem("Hora: ");
                    map_days[day] = true;
                    current_day = day;
                    hour_start = hour;
                    hour_end = hour;
                }
                else
                {
                    if (hour == hour_end + 1)
                    {
                        hour_end = hour;
                    }
                    else
                    {
                        ui->listWidget_info->addItem(get_hour(hour_start) + " - " + get_hour(hour_end + 1));
                        hour_start = hour;
                        hour_end = hour;
                    }
                }

            }
            if (current_day != -1)
            {
                ui->listWidget_info->addItem(get_hour(hour_start) + " - " + get_hour(hour_end + 1));
            }

            int counter;
            bool flag = false;

            for(Professor& prof_subject : get_professors_subject(current_subj))
            {
                counter = 0;
                for(std::pair<int, int>& pair : blocks)
                {
                    int day = pair.first;
                    int hour = pair.second;
                    const TimeBlock& block = prof_subject.get_timeblock(day, hour);

                    if(block.state == DISPONIBLE)
                    {
                        counter++;
                    }
                }

                if(counter == current_subj.get_required_hours())
                {
                    ui->listWidget_professors->addItem(QString::fromStdString(prof_subject.get_full_name() + " - " + prof_subject.get_id()));
                    flag = true;
                }
            }
            if(flag == false)
            {
                ui->label->setText("No hay profesores con horarios disponibles para asignar esta materia");
                ui->pushButton_save->setText("Ok");
            }
            else
            {
                ui->label->setText("Seleccione un profesor");
            }
        }
    }
}



std::vector<Professor> AssignProfessorDialog::get_professors_subject(Subject& subject)
{
    std::vector<Professor> professors_vector;
    std::unordered_set<std::string> added_professor_ids;

    for(Professor& professor : professors)
    {
        if (professor.get_id().find("TEMP_") != std::string::npos)
        {
            continue;
        }

        for(Subject& subject_prof : professor.get_subjects())
        {
            if(subject_prof.get_id() ==  subject.get_id())
            {
                if (added_professor_ids.insert(professor.get_id()).second)
                {
                    professors_vector.push_back(professor);
                }
                break;
            }
        }
    }

    return professors_vector;
}

void AssignProfessorDialog::on_pushButton_save_clicked()
{

    accept();
}


void AssignProfessorDialog::on_pushButton_cancel_clicked()
{
    reject();
}


void AssignProfessorDialog::on_listWidget_professors_itemClicked(QListWidgetItem *item)
{
    QString item_text = item->text();
    QStringList parts = item_text.split(" - ");
    if(parts.size() != 2)
    {
        return;
    }

    QString name = parts[0];
    QString id = parts[1];

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Asignar profesor", "¿Desea asignar a " + name + " a esta sección?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        std::string profesor_id = id.toStdString();
        std::string subject_id = current_subj.get_id();

        auto sections = current_assign.get_sections_vector();

        for(auto& section : sections)
        {
            if (section.get_subject_section() == subject_id && section.get_professor_section().find("TEMP_") != std::string::npos)
            {
                section.set_professor_section(profesor_id);
                break;
            }
        }
        current_assign.set_sections_vector(sections);
        QMessageBox::information(this, "Asignado", "Profesor asignado correctamente.");

        ui->comboBox_subjects->removeItem(ui->comboBox_subjects->currentIndex());
        ui->listWidget_info->clear();
        ui->listWidget_professors->clear();

        if(ui->comboBox_subjects->count() == 0)
        {
            QMessageBox::information(this, "Completado", "Se asignaron todos los profesores.");
            accept();
        }

    }
}

Assigment AssignProfessorDialog::get_update_assigment() const
{
    return current_assign;
}

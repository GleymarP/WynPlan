#include "sectionwindow.h"
#include "ui_sectionwindow.h"

SectionWindow::SectionWindow(StudyPlan& plan_, std::vector<Assigment>& assigments_, std::vector<Teacher>& teachers_, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SectionWindow)
    , plan(plan_)
    , assigments(assigments_)
    , teachers(teachers_)
{
    ui->setupUi(this);
    ui->listWidget->hide();
    ui->label_4->hide();
    ui->label_info->hide();
    ui->pushButton_delete_section->hide();

    ui->listWidget->clear();
    std::vector<Semester> semesters = plan_.get_semester();

    for(const auto &semester : semesters)
    {
        ui->comboBox_semester->addItem(QString::fromStdString(semester.get_semester_name()));
    }

    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    qApp->setStyleSheet("QMessageBox QLabel { color: black; }");

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


void SectionWindow::on_comboBox_semester_currentTextChanged(const QString &arg1)
{
    std::string text_changed = arg1.toStdString();
    ui->comboBox_option->clear();
    for(Assigment& assigment : assigments)
    {
        if(text_changed == assigment.get_semester_name())
        {
            current_txt = assigment.get_semester_name();
            ui->comboBox_option->addItem(QString::fromStdString(assigment.get_option()));
        }
    }
}


void SectionWindow::on_comboBox_option_currentTextChanged(const QString &arg1)
{
    QString current_semester = ui->comboBox_semester->currentText();
    std::string text_changed = arg1.toStdString();
    ui->listWidget->clear();
    bool found_assignment = false;

    for(Assigment& assigment : assigments)
    {
        if(text_changed == assigment.get_option() && assigment.get_semester_name() == current_semester.toStdString())
        {
            found_assignment = true;
            current_assigment = assigment;
            Semester semester_value;

            for(Semester& semester : plan.get_semester())
            {
                if(semester.get_semester_name() == assigment.get_semester_name())
                {
                    semester_value = semester;
                    break;
                }
            }

            if(semester_value.get_semester_name().empty())
            {
                break;
            }

            std::vector<Section> sections = assigment.get_sections_vector();

            for(Section&  section : sections)
            {
                for(Subject& subject : semester_value.get_subjects_semester())
                {
                    if(subject.get_id() == section.get_subject_section())
                    {
                        ui->listWidget->addItem("Materia: " + QString::fromStdString(subject.get_subject_name()));
                        ui->listWidget->addItem("Código de la materia: " + QString::fromStdString(subject.get_id()));
                        break;
                    }
                }

                for(Teacher& teacher : teachers)
                {
                    if(section.get_teacher_section() == teacher.get_id())
                    {
                        ui->listWidget->addItem("Profesor: " + QString::fromStdString(teacher.get_full_name()));
                        ui->listWidget->addItem("ID del profesor: " + QString::fromStdString(teacher.get_id()));
                        break;
                    }
                }
                ui->listWidget->addItem("Horario: ");

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
                            ui->listWidget->addItem(get_hour(hour_start) + " - " + get_hour(hour_end + 1));
                        }
                        ui->listWidget->addItem("Día: " + QString::fromStdString(get_day(day)));
                        ui->listWidget->addItem("Hora: ");
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
                            ui->listWidget->addItem(get_hour(hour_start) + " - " + get_hour(hour_end + 1));
                            hour_start = hour;
                            hour_end = hour;
                        }
                    }

                }
                if (current_day != -1)
                {
                    ui->listWidget->addItem(get_hour(hour_start) + " - " + get_hour(hour_end + 1));
                }
                ui->listWidget->addItem("");
            }
        }
    }

    if(found_assignment)
    {
        ui->listWidget->show();
        ui->label_4->show();
        ui->pushButton_delete_section->show();
        ui->label_info->hide();
    }
    else
    {
        ui->listWidget->hide();
        ui->label_4->hide();
        ui->pushButton_delete_section->hide();
        ui->label_info->show();

    }
}

std::string get_day(int day)
{
    switch (day)
    {
        case 0:
            return "Lunes";

        case 1:
            return "Martes";

        case 2:
            return "Miércoles";

        case 3:
            return "Jueves";

        case 4:
            return "Viernes";

        case 5:
            return "Sábado";

        case 6:
            return "Domingo";

        default:
            break;
    }
    return std::string();
}

QString get_hour(int hour)
{
    QString hour_qs;
    switch (hour)
    {
        case 0:
            hour_qs = QString("%1:00").arg(7 + 0);
            break;

        case 1:
            hour_qs = QString("%1:00").arg(7 + 1);
            break;

        case 2:
            hour_qs = QString("%1:00").arg(7 + 2);
            break;

        case 3:
            hour_qs = QString("%1:00").arg(7 + 3);
            break;

        case 4:
            hour_qs = QString("%1:00").arg(7 + 4);
            break;

        case 5:
            hour_qs = QString("%1:00").arg(7 + 5);
            break;

        case 6:
            hour_qs = QString("%1:00").arg(7 + 6);
            break;

        case 7:
            hour_qs = QString("%1:00").arg(7 + 7);
            break;

        case 8:
            hour_qs = QString("%1:00").arg(7 + 8);
            break;

        case 9:
            hour_qs = QString("%1:00").arg(7 + 9);
            break;

        case 10:
            hour_qs = QString("%1:00").arg(7 + 10);
            break;

        case 11:
            hour_qs = QString("%1:00").arg(7 + 11);
            break;

        case 12:
            hour_qs = QString("%1:00").arg(7 + 12);
            break;

        default:
            break;
    }
    return hour_qs;
}

void SectionWindow::on_pushButton_delete_section_clicked()
{
    if (assigments.empty())
    {
        QMessageBox::warning(this, "Advertencia", "No hay secciones para eliminar.");
        return;
    }

    sectiondialog dialog(this);
    dialog.set_option_info(current_assigment);
    if(dialog.exec() == QDialog::Accepted && dialog.is_deletion_confirmed())
    {
        std::vector<Assigment> assigments_temp;


        for(Assigment& assigment : assigments)
        {
            if(assigment.get_option() == current_assigment.get_option()  && assigment.get_semester_name() == current_assigment.get_semester_name()  )
            {
                continue;
            }
            else
            {
                assigments_temp.push_back(assigment);
            }
        }

        assigments = assigments_temp;

        Assigment::save_assigments_json(assigments, QCoreApplication::applicationDirPath() + "/../../resources/assign.json", plan);
        assigments = Assigment::load_from_json_assing(QCoreApplication::applicationDirPath() + "/../../resources/assign.json", plan, teachers);
        teachers = Teacher::load_from_json(QCoreApplication::applicationDirPath() + "/../../resources/teachers.json", plan);
        Teacher::save_teachers_json(teachers, QCoreApplication::applicationDirPath() + "/../../resources/teachers.json" );

        QMessageBox::information(this, "Eliminado","Sección eliminada correctamente");
        ui->listWidget->clear();
        ui->comboBox_option->clear();

        for(Assigment& assigment : assigments)
        {
            if(current_txt == assigment.get_semester_name())
            {
                ui->comboBox_option->addItem(QString::fromStdString(assigment.get_option()));
            }
        }

    }
    else
    {
        QMessageBox::information(this, "Cancelado", "No se realizó ninguna acción");
    }

}


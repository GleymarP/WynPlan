#include "schedulewindow.h"
#include "ui_schedulewindow.h"

ScheduleWindow::ScheduleWindow(StudyPlan& plan, std::vector<Professor>& professors, std::vector<Assigment>& assigments, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScheduleWindow)
    , plan_(plan)
    , professors_(professors)
    , assignments_(assigments)
{
    ui->setupUi(this);
    ui->tableWidget->hide();

    std::vector<Semester> semesters = plan_.get_semester();
    for(const auto &semester : semesters)
    {
        ui->comboBox_schedule->addItem(QString::fromStdString(semester.get_semester_name()));
    }

    if(assignments_.empty())
    {
        ui->pushButton_read->hide();
    }
    qApp->setStyleSheet("QMessageBox QLabel { color: black; }");
}

ScheduleWindow::~ScheduleWindow()
{
    delete ui;
}

void ScheduleWindow::on_pushButton_menu_clicked()
{
    emit back_to_menu();
    this->close();
}

void ScheduleWindow::update_table()
{
    ui->tableWidget->hide();
    ui->tableWidget->clear();
    ui->pushButton_read->show();

    bool found_assigment = false;


    QString selected_option = ui->comboBox_options->currentText();

    for(auto& assigment : assignments_)
    {
        if(assigment.get_semester_name() == current_semester.get_semester_name() && QString::fromStdString(assigment.get_option()) == selected_option)
        {
            found_assigment = true;

            QStringList days = { "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo" };
            QStringList hours;

            for(int i = 0; i < 12; ++i)
            {
                hours << QString("%1:00").arg(7 + i);
            }

            ui->tableWidget->setRowCount(12);
            ui->tableWidget->setColumnCount(7);
            ui->tableWidget->setHorizontalHeaderLabels(days);
            ui->tableWidget->setVerticalHeaderLabels(hours);

            ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            std::vector<Section> sections = assigment.get_sections_vector();
            std::map<std::string, QColor> subject_colors;
            QList<QColor> palette = {
                QColor(249, 176, 195),
                QColor(174, 218, 237),
                QColor(131, 239, 196),
                QColor(241, 250, 173),
                QColor(254, 197, 162),
                QColor(233, 129, 129),
                QColor(22, 139, 204),
                QColor(212, 190, 242)
            };
            int color_index = 0;

            for(auto& section : sections)
            {
                std::string subject_id = section.get_subject_section();
                if(subject_colors.find(subject_id) == subject_colors.end())
                {
                    subject_colors[subject_id] = palette[color_index % palette.size()];
                    ++color_index;
                }

                for(const auto& [day, hour] : section.get_assigned_blocks())
                {
                    if(day >= 0 && day < 7 && hour >= 0 && hour < 12)
                    {
                        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(subject_id));
                        item->setTextAlignment(Qt::AlignCenter);
                        item->setBackground(subject_colors[subject_id]);

                        std::string subject_name;

                        for(auto& subject : current_semester.get_subjects_semester())
                        {
                            if(subject.get_id() == subject_id)
                            {
                                subject_name = subject.get_subject_name();
                                break;
                            }
                        }

                        std::string professor_id = section.get_professor_section();
                        std::string professor_name;

                        for(const auto& professor : professors_)
                        {
                            if(professor.get_id() == professor_id)
                            {
                                professor_name = professor.get_full_name();
                            }

                        }

                        item->setToolTip(QString::fromStdString("Materia: " + subject_name + "\nProfesor: " + professor_name));

                        ui->tableWidget->setItem(hour, day, item);
                    }
                }

            }
            break;
        }
    }

    if(found_assigment)
    {
        ui->tableWidget->show();
    }
    else
    {
        ui->tableWidget->hide();
        QMessageBox::information(this, "Sin asignaciones", "Este semestre no tiene asignaciones para la opción seleccionada.");
    }

}


void ScheduleWindow::on_pushButton_generate_schedule_clicked()
{
    ui->tableWidget->clear();
    std::vector<Subject> semester_subjects = current_semester.get_subjects_semester();

    int i = 0;
    std::vector<Professor> semester_professors;
    std::vector<Professor> temporal_professors;
    std::unordered_set<std::string> added_professor_ids;


    for (auto& subject : semester_subjects)
    {
        bool has_professor = false;
        for (auto& professor : professors_)
        {
            for (auto& prof_subject : professor.get_subjects())
            {
                if (prof_subject.get_id() == subject.get_id())
                {
                    if (added_professor_ids.insert(professor.get_id()).second)
                    {
                        semester_professors.push_back(professor);
                    }
                    has_professor = true;
                    break;

                }
            }
            if (has_professor)
            {
                break;
            }
        }
        if (!has_professor)
        {
            Professor temporal;
            temporal.set_full_name("Profesor por asignar " + std::to_string(i) + " _ " + subject.get_id());
            i++;
            temporal.set_id("TEMP_" + subject.get_id());
            temporal.set_subjects({subject});
            for (int dia = 0; dia < 7; dia++)
            {
                for (int hora = 0; hora < 12; hora++)
                {
                    temporal.set_state_block(dia, hora, DISPONIBLE);
                }
            }
            temporal_professors.push_back(temporal);
            semester_professors.push_back(std::move(temporal));
        }
    }


    NetworkGraph network_graph(semester_professors, semester_subjects);

    //NetworkGraph network_graph(professors_, semester_subjects);
    network_graph.max_flow();

    std::vector<Section> sections = network_graph.get_final_assign_section();

    if(!sections.empty())
    {
        Assigment assigment;
        assigment.set_sections_vector(sections);
        assigment.set_semester_name(current_semester.get_semester_name());

        int number_option = 1;

        for(auto& assigment : assignments_)
        {
            if(assigment.get_semester_name() == current_semester.get_semester_name())
            {
                ++number_option;
            }
        }

        current_option = "Opción " + std::to_string(number_option);
        assigment.set_option(current_option);


        assignments_.push_back(assigment);
        Assigment::save_assigments_json(assignments_, path_assign_json, plan_);
        assignments_ = Assigment::load_from_json_assing(path_assign_json, plan_, professors_);

        ui->comboBox_options->addItem(QString::fromStdString(current_option));
        ui->comboBox_options->setCurrentText(QString::fromStdString(current_option));

        update_table();
    }
    else
    {
        ui->tableWidget->hide();
        ui->tableWidget->clear();
        QMessageBox::information(this, "No hay opción disponible", "No se pudieron asignar todas las materias requeridas para el semestre.");
    }

}

void ScheduleWindow::on_pushButton_read_clicked()
{
    if(!assignments_.empty())
    {
        update_table();
    }

}

void ScheduleWindow::on_comboBox_schedule_currentTextChanged(const QString &arg1)
{
    auto semesters = plan_.get_semester();

    for(const auto& semester : semesters)
    {
        if(QString::fromStdString(semester.get_semester_name()) == arg1)
        {
            current_semester = semester;
            break;
        }
    }

    ui->comboBox_options->clear();

    for(auto& assigment : assignments_)
    {
        if(QString::fromStdString(assigment.get_semester_name()) == arg1)
        {
            ui->comboBox_options->addItem(QString::fromStdString(assigment.get_option()));

        }
    }

}

void ScheduleWindow::on_comboBox_options_windowIconTextChanged(const QString &iconText)
{
    current_option = iconText.toStdString();
    update_table();
}


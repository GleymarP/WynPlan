#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
    , plan(StudyPlan::load_from_json(path_studyplan_json))
    , professors(Professor::load_from_json(path_professors_json, plan))
    , assigments(Assigment::load_from_json_assing(path_assign_json, plan, professors))
{
    ui->setupUi(this);
    update_ui();
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::reload_data()
{
    professors = Professor::load_from_json(path_professors_json, plan);
    assigments = Assigment::load_from_json_assing(path_assign_json, plan, professors);
}

void HomePage::on_teacher_button_clicked()
{
    reload_data();
    TeachersWindow *professor_window = new TeachersWindow(professors, plan, assigments);
    connect(professor_window, &TeachersWindow::back_to_menu, this, &HomePage::handle_back_to_menu);
    professor_window->show();
    this->hide();
}

void HomePage::on_studyplan_button_clicked()
{
    StudyPlanWindow *study_window =  new StudyPlanWindow(plan);
    connect(study_window, &StudyPlanWindow::back_to_menu, this, &HomePage::show);
    study_window->show();
    this->hide();
}

void HomePage::on_section_button_clicked()
{
    reload_data();
    SectionWindow *section_window = new SectionWindow(plan, assigments, professors);
    connect(section_window, &SectionWindow::back_to_menu, this, &HomePage::show);
    section_window->show();
    this->hide();
}

void HomePage::on_schedule_button_clicked()
{
    reload_data();
    ScheduleWindow *schedule_window = new ScheduleWindow(plan, professors, assigments);
    connect(schedule_window, &ScheduleWindow::back_to_menu, this, &HomePage::show);
    schedule_window->show();
    this->hide();
}

void HomePage::on_config_button_clicked()
{
    reload_data();
    InicialConfig *incial_window = new InicialConfig(plan);
    connect(incial_window, &InicialConfig::back_to_menu, this, &HomePage::handle_back_to_menu);
    incial_window->show();
    this->hide();
}

void HomePage::handle_back_to_menu()
{
    this->show();
    plan = StudyPlan::load_from_json(path_studyplan_json);
    reload_data();
    update_ui();
}

void HomePage::update_ui()
{
    if(plan.get_semester().empty())
    {
        ui->frame_7->show();
        ui->config_button->show();
        ui->config_label->show();
        ui->frame_3->hide();
        ui->frame_4->hide();
        ui->frame_5->hide();
        ui->frame_2->hide();

        ui->schedule_button->setEnabled(false);
        ui->teacher_button->setEnabled(false);
        ui->section_button->setEnabled(false);
        ui->studyplan_button->setEnabled(false);
        ui->config_button->show();
        ui->config_label->show();
        ui->label_warning->show();
        ui->label_warning->setText("No hay un plan de estudio agregado, ve a Añadir Plan de Estudio");
    }
    else if(professors.empty())
    {
        ui->frame_7->show();
        ui->config_button->hide();
        ui->config_label->hide();
        ui->frame_3->show();
        ui->frame_4->show();
        ui->frame_5->hide();
        ui->frame_2->hide();

        ui->config_button->hide();
        ui->config_label->hide();
        ui->label_warning->show();
        ui->label_warning->setText("No hay profesores agregados, ve a la parte de profesor para añadir");
        ui->schedule_button->setEnabled(false);
        ui->teacher_button->setEnabled(true);
        ui->section_button->setEnabled(false);
        ui->studyplan_button->setEnabled(true);
    }
    else
    {
        ui->frame_7->hide();
        ui->config_button->hide();
        ui->config_label->hide();
        ui->frame_3->show();
        ui->frame_4->show();
        ui->frame_5->show();
        ui->frame_2->show();

        ui->config_button->hide();
        ui->config_label->hide();
        ui->label_warning->hide();
        ui->schedule_button->setEnabled(true);
        ui->teacher_button->setEnabled(true);
        ui->section_button->setEnabled(true);
        ui->studyplan_button->setEnabled(true);
    }
}



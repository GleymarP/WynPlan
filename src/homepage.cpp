#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
    , plan(StudyPlan::load_from_json(path_studyplan_json))
    , teachers(Teacher::load_from_json(path_teachers_json, plan))
    //, assigments(Assigment::load_from_json(path_assigments_json, plan, teachers))
    , assigments(Assigment::load_from_json_assing(path_assign_json, plan, teachers))
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::reload_data()
{
    teachers = Teacher::load_from_json(path_teachers_json, plan);
    assigments = Assigment::load_from_json_assing(path_assign_json, plan, teachers);
}

void HomePage::on_teacher_button_clicked()
{
    reload_data();
    TeachersWindow *teacher_window = new TeachersWindow(teachers, plan);
    connect(teacher_window, &TeachersWindow::back_to_menu, this, &HomePage::show);
    teacher_window->show();
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
    SectionWindow *section_window = new SectionWindow(plan, assigments, teachers);
    connect(section_window, &SectionWindow::back_to_menu, this, &HomePage::show);
    section_window->show();
    this->hide();
}


void HomePage::on_schedule_button_clicked()
{
    ScheduleWindow *schedule_window = new ScheduleWindow(plan);
    connect(schedule_window, &ScheduleWindow::back_to_menu, this, &HomePage::show);
    schedule_window->show();
    this->hide();
}


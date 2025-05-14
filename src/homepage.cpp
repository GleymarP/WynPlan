#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}





void HomePage::on_teacher_button_clicked()
{
    QString path_json = ":/resources/flujograma.json";
    StudyPlan plan = StudyPlan::load_from_json(path_json);
    QString path_json_teacher = ":/resources/teachers.json";
    std::vector<Teacher> teachers = Teacher::load_from_json(path_json_teacher, plan);

    TeachersWindow *teacher_window = new TeachersWindow(teachers);
    connect(teacher_window, &TeachersWindow::back_to_menu, this, &HomePage::show);
    teacher_window->show();
    this->hide();
}

void HomePage::on_studyplan_button_clicked()
{
    QString path_json = ":/resources/flujograma.json";
    StudyPlan plan = StudyPlan::load_from_json(path_json);
    StudyPlanWindow *study_window =  new StudyPlanWindow(plan);
    connect(study_window, &StudyPlanWindow::back_to_menu, this, &HomePage::show);
    study_window->show();
    this->hide();
}


#include "studyplanwindow.h"
#include "ui_studyplanwindow.h"
#include "definitions.h"

StudyPlanWindow::StudyPlanWindow(StudyPlan &plan,QWidget *parent)
    : QMainWindow(parent), plan_(plan)
    , ui(new Ui::StudyPlanWindow)
{
    ui->setupUi(this);

    ui->label_name_degree->setText(QString::fromStdString(plan_.get_degree()));
    std::vector<Semester> semesters = plan_.get_semester();
    for(const auto &semester : semesters)
    {
        ui->comboBox_studyplan->addItem(QString::fromStdString(semester.get_semester_name()));
    }
}

StudyPlanWindow::~StudyPlanWindow()
{
    delete ui;
}


void StudyPlanWindow::on_comboBox_studyplan_currentTextChanged(const QString &arg1)
{
    std::string  text_changed = arg1.toStdString();
    std::vector<Semester> semesters = plan_.get_semester();
    for(const auto& semester : semesters)
    {
        if(semester.get_semester_name() == text_changed)
        {
            ui->listWidget_Semestre->clear();
            for(const auto& subject: semester.get_subjects_semester())
            {
                ui->listWidget_Semestre->addItem(QString::fromStdString(subject.get_subject_name()));
            }
        }
    }
}

void StudyPlanWindow::on_pushButtonMenu_clicked()
{
    emit back_to_menu();
    this->close();
}


#include "schedulewindow.h"
#include "ui_schedulewindow.h"

ScheduleWindow::ScheduleWindow(StudyPlan& plan, std::vector<Teacher>& teachers, std::vector<Assigment>& assigments, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScheduleWindow)
    , plan_(plan)
    , teachers_(teachers)
    , assignments_(assigments)
{
    ui->setupUi(this);
    ui->tableWidget->hide();

    std::vector<Semester> semesters = plan_.get_semester();
    for(const auto &semester : semesters)
    {
        ui->comboBox_schedule->addItem(QString::fromStdString(semester.get_semester_name()));
    }
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
    ui->tableWidget->clear();
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
}

void ScheduleWindow::on_pushButton_clicked()
{
    ui->tableWidget->show();
    ui->tableWidget->clear();

    update_table();

}

void ScheduleWindow::on_pushButton_generate_schedule_clicked()
{

    ui->tableWidget->clear();
    std::vector<Subject> semester_subjects = current_semester.get_subjects_semester();

    NetworkGraph network_graph(teachers_, semester_subjects);
    network_graph.max_flow();

    std::vector<Section> sections = network_graph.get_final_assign_section();




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
}


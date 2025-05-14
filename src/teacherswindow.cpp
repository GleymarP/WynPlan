#include "teacherswindow.h"
#include "ui_teacherswindow.h"

TeachersWindow::TeachersWindow(std::vector<Teacher>& teacher, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TeachersWindow)
    , teachers_(teacher)
{
    ui->setupUi(this);
    ui->tableWidget->hide();
}

TeachersWindow::~TeachersWindow()
{
    delete ui;
}


void TeachersWindow::on_menu_button_clicked()
{
    emit back_to_menu();
    this->close();
}

bool TeachersWindow::validate_id(const QString &id)
{
    static const QRegularExpression format_id("^[VE]-\\d{6,8}$");
    return format_id.match(id).hasMatch();
}

void TeachersWindow::on_search_button_clicked()
{

    QString input_id = ui->line_edit_id->text().trimmed();

    if (input_id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingrese una cédula.");
        return;
    }

    if (!validate_id(input_id)) {
        QMessageBox::warning(this, "Formato inválido", "Ingrese una cédula válida (ej: V-12345678, E-8765432).");
        return;
    }


    bool found_id = false;
    Teacher found_teacher;

    for(const auto& teacher: teachers_)
    {
        if(QString::fromStdString(teacher.get_id()) == input_id)
        {
            found_id = true;
            found_teacher = teacher;
        }
    }

    if(!found_id)
    {
        QMessageBox::information(this, "No encontrado", "No se encontró un profesor con esa cédula.");
        return;
    }
    else
    {
        ui->label_teacher_id->setText("Cédula: " + QString::fromStdString(found_teacher.get_id()));
        ui->label_teacher_name->setText("Nombre: " + QString::fromStdString(found_teacher.get_full_name()));


        ui->tableWidget->show();
        QStringList days = { "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo" };
        QStringList hours;

        for(int i = 0; i < 12; ++i)
        {
            hours << QString("%1:00").arg(7 + i);
        }

        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(12);
        ui->tableWidget->setColumnCount(7);
        ui->tableWidget->setHorizontalHeaderLabels(days);
        ui->tableWidget->setVerticalHeaderLabels(hours);



        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for(int day = 0; day < 7; ++day)
        {
            for(int hour = 0; hour < 12; ++hour)
            {
                const TimeBlock& block = found_teacher.get_timeblock(day, hour);

                QString text;

                switch (block.state)
                {
                    case NO_DISPONIBLE:
                        text = "X";
                        break;
                    case DISPONIBLE:
                        text = "";
                        break;
                    case OCUPADO:
                        text = QString::fromStdString(block.id_subject);
                        break;
                }

                QTableWidgetItem* item = new QTableWidgetItem(text);
                if (block.state == NO_DISPONIBLE)
                {
                    item->setBackground(Qt::gray);
                }
                else if(block.state == OCUPADO)
                {
                    item->setBackground(Qt::red);
                }
                else
                {
                    item->setBackground(Qt::green);
                }

                    ui->tableWidget->setItem(hour, day, item);
            }
        }



    }






    ui->line_edit_id->clear();


}


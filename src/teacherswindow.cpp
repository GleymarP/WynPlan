#include "teacherswindow.h"
#include "ui_teacherswindow.h"

TeachersWindow::TeachersWindow(std::vector<Teacher>& teacher, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TeachersWindow)
    , teachers_(teacher)
{
    ui->setupUi(this);
    ui->tableWidget->hide();
    ui->button_save_changes->hide();
    ui->button_modify_states->hide();
    connect(ui->line_edit_id, &QLineEdit::returnPressed, this, &TeachersWindow::on_search_button_clicked);
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
        current_teacher = found_teacher;
        ui->label_teacher_id->setText("Cédula: " + QString::fromStdString(found_teacher.get_id()));
        ui->label_teacher_name->setText("Nombre: " + QString::fromStdString(found_teacher.get_full_name()));


        ui->tableWidget->show();
        ui->button_modify_states->show();
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
                    item->setBackground(QColor(217, 217, 217));
                }
                else if(block.state == OCUPADO)
                {
                    item->setBackground(QColor(255, 107, 108));
                }
                else
                {
                    item->setBackground(QColor(179, 240, 174));
                }

                    ui->tableWidget->setItem(hour, day, item);
            }
        }

    }

    ui->line_edit_id->clear();

}


void TeachersWindow::on_tableWidget_cellClicked(int row, int column)
{

    if (!edit_mode_enabled)
    {
        return;
    }

    int hour = row;
    int day = column;

    const TimeBlock block = current_teacher.get_timeblock(day, hour);
    QString current_state;

    if(block.state == OCUPADO)
    {
        QString subject_id = QString::fromStdString(block.id_subject);
        QString subject_name;

        for(const auto& subject : current_teacher.get_subjects())
        {
            if(QString::fromStdString(subject.get_id()) == subject_id)
            {
                subject_name = QString::fromStdString(subject.get_subject_name());
                break;
            }
        }

        QMessageBox::information(this, "Bloque ocupado", QString("Este bloque está ocupado por la materia:\n%1 (%2)") .arg(subject_name) .arg(subject_id));
        return;
    }
    else if(block.state == DISPONIBLE)
    {
        current_state = "DISPONIBLE";
    }
    else
    {
        current_state = "NO DISPONIBLE";
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Cambiar el estado del bloque", QString("El bloque actual está marcado como %1. \n ¿Desea cambiar su estado?").arg(current_state), QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        BlockState update_state;
        QTableWidgetItem* item = ui->tableWidget->item(hour, day);

        item->setText("");
        if(current_state == "DISPONIBLE")
        {
            update_state = NO_DISPONIBLE;
            item->setText("X");
            item->setBackground(QColor(217, 217, 217));
        }
        else if (current_state == "NO DISPONIBLE")
        {
            update_state = DISPONIBLE;
            item->setBackground(QColor(179, 240, 174));
        }
        current_teacher.set_state_block(day, hour, update_state);
    }

}


void TeachersWindow::on_button_modify_states_clicked()
{
    edit_mode_enabled = true;
    ui->button_modify_states->setEnabled(false);
    ui->button_save_changes->show();

    QMessageBox::information(this, "Modo Edición Activado", "Ahora puede hacer clic en los bloques para modificar su estado.");
}


void TeachersWindow::on_button_save_changes_clicked()
{
    for(auto& teacher : teachers_)
    {
        if(teacher.get_id() == current_teacher.get_id())
        {
            teacher = current_teacher;
            break;
        }
    }

    Teacher::save_teachers_json(teachers_, QCoreApplication::applicationDirPath() + "/../../resources/teachers.json");
    edit_mode_enabled = false;
    ui->button_modify_states->setEnabled(true);
    ui->button_save_changes->hide();
}


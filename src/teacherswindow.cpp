#include "teacherswindow.h"
#include "ui_teacherswindow.h"

TeachersWindow::TeachersWindow(std::vector<Teacher>& teacher, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TeachersWindow)
    , teachers_(teacher)
{
    ui->setupUi(this);
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

    auto it = std::find_if(teachers_.begin(), teachers_.end(), [&](const Teacher& t) {
        return QString::fromStdString(t.get_id()) == input_id;
    });

    if (it == teachers_.end()) {
        QMessageBox::information(this, "No encontrado", "No se encontró un profesor con esa cédula.");
        return;
    }

    const Teacher& found_teacher = *it;


    ui->label_teacher_id->setText("Cédula: " + QString::fromStdString(found_teacher.get_id()));
    ui->label_teacher_name->setText("Nombre: " + QString::fromStdString(found_teacher.get_full_name()));

    ui->line_edit_id->clear();


}


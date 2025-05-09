#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "menuwindow.h"
#include <QHeaderView>
#include <QMessageBox>

#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    setupUI();
    populateTable();

    setWindowTitle("Horario Académico");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    tableWidget = new QTableWidget(this);
    mainLayout->addWidget(tableWidget);

    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels({"Materia", "Horario", "Sección", "Profesor"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setVisible(false);

    returnButton = new QPushButton("Volver al Menú", this);
    returnButton->setStyleSheet("QPushButton {"
                                "font-size: 14px;"
                                "padding: 10px;"
                                "background-color: #2196F3;"
                                "color: white;"
                                "border: none;"
                                "border-radius: 5px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: #0b7dda;"
                                "}");
    mainLayout->addWidget(returnButton, 0, Qt::AlignRight);

    connect(returnButton, &QPushButton::clicked, this, &MainWindow::returnToMenu);
}

void MainWindow::populateTable()
{
    QStringList materias = {"Cálculo 10", "Física 11", "Sistemas  de Representación", "Inglés I", "IPQ"};
    QStringList horarios = {"Lunes 8:00-10:00", "Martes 10:00-12:00", "Miércoles 14:00-16:00",
                            "Jueves 16:00-18:00", "Viernes 8:00-10:00"};
    QStringList secciones = {"1", "2", "3", "4", "5"};
    QStringList profesores = {"Dr. Pérez", "Dra. Gómez", "Dr. Rodríguez", "Dra. Martínez", "Dr. López"};

    tableWidget->setRowCount(materias.size());

    for (int row = 0; row < materias.size(); ++row) {
        tableWidget->setItem(row, 0, new QTableWidgetItem(materias[row]));
        tableWidget->setItem(row, 1, new QTableWidgetItem(horarios[row]));
        tableWidget->setItem(row, 2, new QTableWidgetItem(secciones[row]));
        tableWidget->setItem(row, 3, new QTableWidgetItem(profesores[row]));
    }
}
void MainWindow::returnToMenu()
{
    this->hide();
    emit returnToMenuRequested();
}

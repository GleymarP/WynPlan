#include "menuwindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Menú");
    resize(400, 300);

    // Widget central y layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Menú", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 30px;");
    layout->addWidget(titleLabel);

    scheduleButton = new QPushButton("Ver Horarios", this);
    scheduleButton->setStyleSheet("QPushButton {"
                                  "font-size: 16px;"
                                  "padding: 10px;"
                                  "min-width: 200px;"
                                  "background-color: #75024c;"
                                  "color: white;"
                                  "border: none;"
                                  "border-radius: 5px;"
                                  "}"
                                  "QPushButton:hover {"
                                  "background-color: #4d0232;"
                                  "}");
    layout->addWidget(scheduleButton, 0, Qt::AlignCenter);

    layout->addSpacing(20);

    exitButton = new QPushButton("Salir", this);
    exitButton->setStyleSheet("QPushButton {"
                              "    font-size: 18px;"
                              "    padding: 10px;"
                              "    min-width: 200px;"
                              "    background-color: #590194;"
                              "    color: white;"
                              "    border: none;"
                              "    border-radius: 5px;"
                              "}"
                              "QPushButton:hover {"
                              "    background-color: #2f024d;"
                              "}");
    layout->addWidget(exitButton, 0, Qt::AlignCenter);

    mainWindow = new MainWindow(this);
    connect(mainWindow, &MainWindow::returnToMenuRequested, this, [this]()
    {
        mainWindow->hide();
        this->show();
    });

    mainWindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(scheduleButton, &QPushButton::clicked, this, &MenuWindow::openScheduleWindow);
    connect(exitButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(mainWindow, &MainWindow::destroyed, this, [this]()
    {
        this->show();
    });

}

MenuWindow::~MenuWindow()
{
}

void MenuWindow::openScheduleWindow()
{
    mainWindow->show();
    this->hide();
}

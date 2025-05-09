#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class MainWindow; // Declaración adelantada

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();


private slots:
    void openScheduleWindow();

private:
    QPushButton *scheduleButton;
    QPushButton *exitButton;
    MainWindow *mainWindow; // Ventana de horarios
};

#endif // MENUWINDOW_H

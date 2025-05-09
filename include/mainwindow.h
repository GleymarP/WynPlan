#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void returnToMenu();

signals:
    void returnToMenuRequested();

private:
    Ui::MainWindow *ui;
    void setupUI();
    void populateTable();

    QTableWidget *tableWidget;
    QPushButton *returnButton;
};
#endif // MAINWINDOW_H

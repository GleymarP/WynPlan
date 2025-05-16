#ifndef SECTIONWINDOW_H
#define SECTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class SectionWindow;
}

class SectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SectionWindow(QWidget *parent = nullptr);
    ~SectionWindow();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_menu_clicked();

private:
    Ui::SectionWindow *ui;
};



#endif // SECTIONWINDOW_H

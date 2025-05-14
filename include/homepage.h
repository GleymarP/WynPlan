#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <studyplanwindow.h>
#include <teacherswindow.h>

namespace Ui {
class HomePage;
}

class HomePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:

    void on_teacher_button_clicked();
    void on_studyplan_button_clicked();;

private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H

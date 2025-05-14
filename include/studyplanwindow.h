#ifndef STUDYPLANWINDOW_H
#define STUDYPLANWINDOW_H

#include <QMainWindow>
#include "definitions.h"

namespace Ui {
class StudyPlanWindow;
}

class StudyPlanWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudyPlanWindow(StudyPlan &plan, QWidget *parent = nullptr);
    ~StudyPlanWindow();

signals:
    void back_to_menu();

private slots:
    void on_comboBox_studyplan_currentTextChanged(const QString &arg1);



    void on_pushButtonMenu_clicked();

private:
    StudyPlan plan_;
    Ui::StudyPlanWindow *ui;
};

#endif // STUDYPLANWINDOW_H

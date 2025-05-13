#ifndef STUDYPLANWINDOW_H
#define STUDYPLANWINDOW_H

#include <QMainWindow>

namespace Ui {
class StudyPlanWindow;
}

class StudyPlanWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudyPlanWindow(QWidget *parent = nullptr);
    ~StudyPlanWindow();

private:
    Ui::StudyPlanWindow *ui;
};

#endif // STUDYPLANWINDOW_H

#ifndef INICIALCONFIG_H
#define INICIALCONFIG_H

#include <QMainWindow>
#include <addsubjectdialog.h>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
class InicialConfig;
}

class InicialConfig : public QMainWindow
{
    Q_OBJECT

public:
    explicit InicialConfig(StudyPlan& plan, QWidget *parent = nullptr);
    ~InicialConfig();

    void update_window();

    void update_next();

signals:
    void back_to_menu();

private slots:
    void on_pushButton_add_subject_clicked();

    void on_pushButton_save_n_semesters_clicked();

    void on_pushButton_save_semester_clicked();

    void on_pushButton_delete_subject_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::InicialConfig *ui;
    StudyPlan plan_;
    std::vector<Subject> subjects_vector;
    std::vector<std::string> semester_vector_name;
    std::vector<Semester> semesters_vector;
    int var = 0;
};

#endif // INICIALCONFIG_H

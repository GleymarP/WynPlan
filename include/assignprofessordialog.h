#ifndef ASSIGNPROFESSORDIALOG_H
#define ASSIGNPROFESSORDIALOG_H

#include <QDialog>
#include <definitions.h>
#include <unordered_set>
#include <QListWidgetItem>

namespace Ui {
class AssignProfessorDialog;
}

class AssignProfessorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignProfessorDialog(Assigment current_assigment, std::vector<Professor> professors_, StudyPlan& plan_, QWidget *parent = nullptr);
    ~AssignProfessorDialog();

    void set_data();

    std::vector<Professor> get_professors_subject(Subject& subject);

    Assigment get_update_assigment() const;

private slots:
    void on_comboBox_subjects_currentTextChanged(const QString &arg1);

    void on_pushButton_cancel_clicked();

    void on_pushButton_save_clicked();

    void on_listWidget_professors_itemClicked(QListWidgetItem *item);


private:
    Ui::AssignProfessorDialog *ui;
    Assigment current_assign;
    std::vector<Professor> professors;
    StudyPlan plan;
    Subject current_subj;

};

#endif // ASSIGNPROFESSORDIALOG_H

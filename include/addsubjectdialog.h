#ifndef ADDSUBJECTDIALOG_H
#define ADDSUBJECTDIALOG_H

#include <QDialog>
#include <definitions.h>
#include <QMessageBox>


namespace Ui {
class AddSubjectDialog;
}

class AddSubjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSubjectDialog(std::vector<Semester>& semestres, std::vector<Subject>& subjects, QWidget *parent = nullptr);
    ~AddSubjectDialog();

    bool is_creation_confirmed() const;
    Subject get_subject();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddSubjectDialog *ui;
    bool confirm_create = false;
    std::vector<Semester> semestres_;
    std::vector<Subject> subjects_;
    Subject subject;
};

#endif // ADDSUBJECTDIALOG_H

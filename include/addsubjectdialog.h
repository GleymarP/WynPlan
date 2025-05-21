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
    explicit AddSubjectDialog(const StudyPlan& plan, QWidget *parent = nullptr);
    ~AddSubjectDialog();

    void set_info(Subject& subject);
    bool is_creation_confirmed() const;
    Subject get_subject();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_textEdit_id_subject_textChanged();

private:
    Ui::AddSubjectDialog *ui;
    bool confirm_create = false;
    StudyPlan plan_;
    Subject subject;
};

#endif // ADDSUBJECTDIALOG_H

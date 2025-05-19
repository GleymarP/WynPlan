#ifndef TEACHEREDITORDIALOG_H
#define TEACHEREDITORDIALOG_H

#include <QDialog>
#include <definitions.h>

namespace Ui {
class TeacherEditorDialog;
}

class TeacherEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherEditorDialog(QWidget *parent = nullptr);
    ~TeacherEditorDialog();

    void set_teacher_info(const Teacher& teacher);

    bool is_deletion_confirmed() const;

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();




private:
    Ui::TeacherEditorDialog *ui;
    bool confirm_delete = false;
};

#endif // TEACHEREDITORDIALOG_H

#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>
#include <definitions.h>
#include <QMessageBox>

namespace Ui {
class TeacherDialog;
}

class TeacherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherDialog(QWidget *parent = nullptr);
    ~TeacherDialog();

    void set_teacher(const Teacher& teacher);

    void set_edit_mode (bool editing);

    Teacher get_update_teacher() const;



private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::TeacherDialog *ui;
    Teacher editable_teacher;
    bool is_editing = false;
};

#endif // TEACHERDIALOG_H

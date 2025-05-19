#ifndef TEACHEREDITORDIALOG_H
#define TEACHEREDITORDIALOG_H

#include <QDialog>

namespace Ui {
class TeacherEditorDialog;
}

class TeacherEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherEditorDialog(QWidget *parent = nullptr);
    ~TeacherEditorDialog();

private:
    Ui::TeacherEditorDialog *ui;
};

#endif // TEACHEREDITORDIALOG_H

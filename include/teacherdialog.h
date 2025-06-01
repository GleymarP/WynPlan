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
    explicit TeacherDialog(std::vector<Professor>& professors, std::string current_id, QWidget *parent = nullptr);
    ~TeacherDialog();

    void set_professor(const Professor& professor);

    void set_edit_mode (bool editing);

    Professor get_update_professor() const;

    void set_available_subjects(const std::vector<Subject>& all_subjects);

    QString get_uppercase(QString input);

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::TeacherDialog *ui;
    Professor editable_professor;
    bool is_editing = false;
    std::vector<Subject> available_subjects;
    std::vector<Professor>& professors_;
    std::string current_id_;
};

#endif // TEACHERDIALOG_H

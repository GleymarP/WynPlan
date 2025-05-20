#ifndef SECTIONDIALOG_H
#define SECTIONDIALOG_H

#include <QDialog>
#include <definitions.h>
#include <QMessageBox>

namespace Ui {
class sectiondialog;
}

class sectiondialog : public QDialog
{
    Q_OBJECT

public:
    explicit sectiondialog(QWidget *parent = nullptr);
    ~sectiondialog();

    void set_option_info(Assigment& assigment);
    bool is_deletion_confirmed() const;

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::sectiondialog *ui;
    bool confirm_delete = false;
};

#endif // SECTIONDIALOG_H

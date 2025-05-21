#ifndef TEACHERSWINDOW_H
#define TEACHERSWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <definitions.h>
#include <teachereditordialog.h>
#include <teacherdialog.h>
#include <set>

namespace Ui {
class TeachersWindow;
}

class TeachersWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeachersWindow(std::vector<Teacher>& teacher, StudyPlan& plan, QWidget *parent = nullptr);
    ~TeachersWindow();


signals:
    void back_to_menu();

private slots:
    void on_menu_button_clicked();

    void on_search_button_clicked();

    bool validate_id(const QString& id);

    void on_tableWidget_cellClicked(int row, int column);

    void on_button_modify_states_clicked();

    void on_button_save_changes_clicked();

    void on_button_delete_clicked();

    void on_button_modify_clicked();

    void update_window();

    void on_pushButton_clicked();

    std::vector<Subject> get_all_subjects();



private:
    Ui::TeachersWindow *ui;
    std::vector<Teacher> teachers_;
    StudyPlan plan_;
    bool edit_mode_enabled = false;
    Teacher current_teacher;
};

#endif // TEACHERSWINDOW_H

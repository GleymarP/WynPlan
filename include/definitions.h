#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
#include <vector>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

class Subject
{
private:
    std::string id;
    std::string name;
    int required_hours;

public:
    void set_id(std::string id_);
    void set_name(std::string name_);
    void set_hours(int hours);

    std::string get_subject_name() const;
    std::string get_id() const;
    int get_hours() const;
};


class Semester
{
private:
    std::string semester_name;
    std::vector<Subject> subject;

public:
    void set_semester_name(std::string name);
    void set_subjects(std::vector<Subject> subjects);

    std::string get_semester_name() const;
    std::vector<Subject> get_subjects_semester() const;
};

class StudyPlan
{
private:
    std::string degree;
    std::vector<Semester> semester;

public:
    void set_degree(std::string name);
    void set_semester(std::vector<Semester> semester_);

    std::string get_degree() const;
    std::vector<Semester> get_semester() const;

    static StudyPlan load_from_json(const QString &filePath);
};


//borrar esto luego
class Schedule
{
private:
    std::string day;
    std::string start_time;
    std::string end_time;

public:
    void set_day(std::string day_);
    void set_start_time(std::string start_time_);
    void set_end_time(std::string end_time_);

    std::string get_day() const;
    std::string get_start_time() const;
    std::string get_end_time() const;

};

// borrar esto luego
class TimeTable
{
private:
    std::string teacher_id;
    int day;
    int hour;

public:
    void set_teacher_id(std::string id);
    void set_day(int day_);
    void set_hour(int hour_);

    std::string get_teacher_id() const;
    int get_day() const;
    int get_hour() const;
};


enum BlockState
{
    NO_DISPONIBLE,
    DISPONIBLE,
    OCUPADO
};

struct TimeBlock
{
    BlockState state = DISPONIBLE;
    std::string id_subject = "";
};

class Teacher
{
private:
    std::string id;
    std::string  full_name;
    std::vector<Subject> subjects;

    std::vector<Schedule> availability;

    TimeBlock weekly_schedule [7][12];

public:

    void set_id(std::string id_);
    void set_full_name(std::string full_name_);
    void set_subjects(std::vector<Subject> subjects_);
    void set_availability(std::vector<Schedule> availability_);

    std::string get_id() const;
    std::string get_full_name() const;
    std::vector<Subject> get_subjects() const;
    std::vector<Schedule> get_availability() const;

    static std::vector<Teacher> load_from_json (const QString &file_path, const StudyPlan &study_plan);

    void assign_block(int day, int hour, const std::string& subject_id);
    void set_state_block(int day, int hour, BlockState state);
    bool available_block(int day, int hour) const;
    const TimeBlock& get_timeblock(int day, int hour) const;

};

class Section
{
private:
    Teacher teacher;
    Subject subject;
    std::vector<std::pair<int, int>> assigned_blocks;
    size_t id_section;

public:
    void set_teacher_section(Teacher teacher_);
    void set_subject_section(Subject subject_);
    void set_id_section(size_t id_section_);

    Teacher get_teacher_section();
    Subject get_subject_section();
    size_t get_id_section();

    void add_timeblock(int day, int hour);
    const std::vector<std::pair<int, int>>& get_assigned_blocks() const;
};

#endif // DEFINITIONS_H


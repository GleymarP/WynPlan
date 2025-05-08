#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include<string>
#include<vector>
#include<tuple>
#include<QString>

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
    size_t credit_units;
public:

    void set_id(std::string id_);
    void set_name(std::string name_);
    void set_credit_units(size_t credit_units_);

    std::string get_subject_name() const;
    std::string get_id() const;
    size_t get_credit_units() const;
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

class Teacher
{
private:
    std::string  full_name;
    std::vector<Subject> subjects;
    std::vector<Schedule> availability;
public:
    void set_full_name(std::string full_name_);
    void set_subjects(std::vector<Subject> subjects_);
    void set_availability(std::vector<Schedule> availability_);

    std::string get_full_name() const;
    std::vector<Subject> get_subjects() const;
    std::vector<Schedule> get_availability() const;

    static std::vector<Teacher> load_from_json (const QString &file_path, const StudyPlan &study_plan);
};


#endif // DEFINITIONS_H


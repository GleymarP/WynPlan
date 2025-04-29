#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include<string>
#include<vector>
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
    //Setters
    void set_id(std::string id_);
    void set_name(std::string name_);
    void set_credit_units(size_t credit_units_);
    //Getters
    std::string get_subject_name() const;
    std::string get_id() const;
    size_t get_credit_units() const;
};

struct Schedule
{
    size_t day;
    std::string start_time;
    std::string end_time;
};

struct Professor
{
    std::string  full_name;
    std::vector<Subject> subjects;
    std::vector<Schedule> available_schedule;
};

class Semester
{
private:
    std::string semester_name;
    std::vector<Subject> subject;
public:
    //Setters
    void set_semester_name(std::string name);
    void set_subjects(std::vector<Subject> subjects);
    //Getters
    std::string get_semester_name() const;
    std::vector<Subject> get_subjects_semester() const;
};

class StudyPlan
{
private:
    std::string degree;
    std::vector<Semester> semester;
public:
    //Setters
    void set_degree(std::string name);
    void set_semester(std::vector<Semester> semester_);
    //Getters
    std::string get_degree() const;
    std::vector<Semester> get_semester() const;
    //Métodos
    static StudyPlan load_from_json(const QString &filePath);
};

#endif // DEFINITIONS_H


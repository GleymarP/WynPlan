#include "definitions.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

void Subject::set_id(std::string id_)
{
    id = id_;
}
void Subject::set_name(std::string name_)
{
    name = name_;
}
void Subject::set_credit_units(size_t credit_units_)
{
    credit_units = credit_units_;
}

std::string Subject::get_subject_name() const
{
    return name;
}
std::string Subject::get_id() const
{
    return id;
}
size_t Subject::get_credit_units() const
{
    return credit_units;
}


void Semester::set_semester_name(std::string name)
{
    semester_name = name;
}

void Semester::set_subjects(std::vector<Subject> subjects)
{
    subject = subjects;
}

std::string Semester::get_semester_name() const
{
    return semester_name;
}

std::vector<Subject> Semester::get_subjects_semester() const
{
    return subject;
}



void StudyPlan::set_degree(std::string name)
{
    degree = name;
}

void StudyPlan::set_semester(std::vector<Semester> semester_)
{
    semester = semester_;
}

std::string StudyPlan::get_degree() const
{
    return degree;
}

std::vector<Semester> StudyPlan::get_semester() const
{
    return semester;
}

StudyPlan StudyPlan::load_from_json(const QString &filePath)
{
    StudyPlan plan;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        return plan;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull())
    {
        return plan;
    }
    QJsonObject root = doc.object();
    QJsonObject plan_object = root["plan_de_estudio"].toObject();

    plan.set_degree(plan_object["nombre"].toString().toStdString());

    QJsonObject semesters_object = plan_object["semestres"].toObject();
    std::vector<Semester> semesters;

    for (auto &semesterKey : semesters_object.keys())
    {
        Semester semester;
        semester.set_semester_name(semesterKey.toStdString());

        QJsonArray subjects_array = semesters_object[semesterKey].toArray();
        std::vector<Subject> subjects;

        for (auto it = subjects_array.begin(); it != subjects_array.end(); ++it)
        {
            QJsonObject subject_object = it->toObject();
            Subject subject;
            subject.set_name(subject_object["nombre"].toString().toStdString());
            subject.set_id(subject_object["codigo"].toString().toStdString());
            subject.set_credit_units(subject_object["creditos"].toInt());
            subjects.push_back(subject);
        }

        semester.set_subjects(subjects);
        semesters.push_back(semester);
    }

    plan.set_semester(semesters);
    return plan;
}


void Schedule::set_day(std::string day_)
{
    day = day_;
}

void Schedule::set_start_time(std::string start_time_)
{
    start_time = start_time_;
}

void Schedule::set_end_time(std::string end_time_)
{
    end_time = end_time_;
}

std::string Schedule::get_day() const
{
    return day;
}

std::string Schedule::get_start_time() const
{
    return start_time;
}

std::string Schedule::get_end_time() const
{
    return end_time;
}


void Teacher::set_full_name(std::string full_name_)
{
    full_name = full_name_;
}

void Teacher::set_subjects(std::vector<Subject> subjects_)
{
    subjects = subjects_;
}

void Teacher::set_available_schedule(std::vector<std::tuple<Subject, std::vector<Schedule>>> available_schedule_)
{
    available_schedule = available_schedule_;
}

std::string Teacher::get_full_name() const
{
    return full_name;
}

std::vector<Subject> Teacher::get_subjects() const
{
    return subjects;
}

std::vector<std::tuple<Subject, std::vector<Schedule>>> Teacher::get_available_schedule() const
{
    return available_schedule;
}

std::vector<Teacher> Teacher::load_from_json(const QString &file_path, const StudyPlan &study_plan)
{
    std::vector<Teacher> teachers;

    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly))
    {
        return teachers;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isArray())
    {
        return teachers;
    }

    QJsonArray teachers_array = doc.array();

    for(const QJsonValue& teacher_value : teachers_array)
    {
        QJsonObject teacher_obj = teacher_value.toObject();
        Teacher teacher;

        teacher.set_full_name(teacher_obj["nombre"].toString().toStdString());

        QJsonArray subjects_array = teacher_obj["materias"].toArray();
        std::vector<Subject> teacher_subjects;
        std::vector<std::tuple<Subject, std::vector<Schedule>>> teacher_schedules;

        for(const QJsonValue& subject_value : subjects_array)
        {
            QJsonObject subject_obj = subject_value.toObject();

            Subject subject_info;
            bool subject_found = false;

            for(const Semester& semester : study_plan.get_semester())
            {
                for(const Subject& plan_subject : semester.get_subjects_semester())
                {
                    if(plan_subject.get_id() == std::to_string(subject_obj["codigo"].toInt()))
                    {
                        subject_info = plan_subject;
                        subject_found = true;
                        break;
                    }
                }
                if(subject_found)
                {
                    break;
                }
            }

            if(subject_found)
            {
                Subject teacher_subject;
                teacher_subject.set_name(subject_info.get_subject_name());
                teacher_subject.set_id(std::to_string(subject_obj["codigo"].toInt()));
                teacher_subject.set_credit_units((subject_info.get_credit_units()));

                std::vector<Schedule> subject_schelude;

                QJsonArray schedules_array = subject_obj["horarios"].toArray();

                for(const QJsonValue& schedule_value : schedules_array)
                {
                    QJsonObject schedule_obj = schedule_value.toObject();
                    Schedule schedule;

                    QJsonArray days_array = schedule_obj["dias"].toArray();

                    for(const QJsonValue& day_value : days_array)
                    {
                        schedule.set_day(day_value.toString().toStdString());
                        schedule.set_start_time(schedule_obj["hora_inicio"].toString().toStdString());
                        schedule.set_end_time(schedule_obj["hora_fin"].toString().toStdString());
                        subject_schelude.push_back(schedule);
                    }

                }

                teacher_subjects.push_back(teacher_subject);
                teacher_schedules.push_back(std::make_tuple(teacher_subject, subject_schelude));
            }

        }

        teacher.set_subjects(teacher_subjects);
        teacher.set_available_schedule(teacher_schedules);
        teachers.push_back(teacher);

    }

    return teachers;
}

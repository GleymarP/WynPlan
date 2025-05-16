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
void Subject::set_hours(int hours)
{
    required_hours = hours;
}

std::string Subject::get_subject_name() const
{
    return name;
}
std::string Subject::get_id() const
{
    return id;
}
int Subject::get_hours() const
{
    return required_hours;
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
            subject.set_hours(subject_object["horas"].toInt());
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





void Teacher::set_id(std::string id_)
{
    id = id_;
}

void Teacher::set_full_name(std::string full_name_)
{
    full_name = full_name_;
}

void Teacher::set_subjects(std::vector<Subject> subjects_)
{
    subjects = subjects_;
}

void Teacher::set_availability(std::vector<Schedule> available_schedule_)
{
    availability = available_schedule_;
}

std::string Teacher::get_id() const
{
    return id;
}

std::string Teacher::get_full_name() const
{
    return full_name;
}

std::vector<Subject> Teacher::get_subjects() const
{
    return subjects;
}

std::vector<Schedule> Teacher::get_availability() const
{
    return availability;
}
void Teacher::set_weekly_schedule(const TimeBlock schedule[7][12])
{
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            weekly_schedule[i][j] = schedule[i][j];
        }
    }
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
        teacher.set_id(teacher_obj["cedula"].toString().toStdString());

        std::vector<Subject> subjects;
        QJsonArray subjects_array = teacher_obj["materias"].toArray();

        for(const QJsonValue& subject_value : subjects_array)
        {
            std::string subject_id = QString::number(subject_value.toInt()).toStdString();
            Subject subject_info;
            bool subject_found = false;

            for(const Semester& semester : study_plan.get_semester())
            {
                for(const Subject& plan_subject : semester.get_subjects_semester())
                {
                    if(plan_subject.get_id() == subject_id)
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
                subjects.push_back(subject_info);
            }
        }
        teacher.set_subjects(subjects);


        if(teacher_obj.contains("weekly_schedule"))
        {
            TimeBlock weekly_schedule[7][12];
            QJsonArray days_array = teacher_obj["weekly_schedule"].toArray();

            for(int day = 0; day < days_array.size() && day < 7; day++)
            {
                QJsonArray hours_array = days_array[day].toArray();
                for(int hour = 0; hour < hours_array.size() && hour < 12; hour++)
                {
                    QJsonObject block_obj = hours_array[hour].toObject();
                    TimeBlock block;
                    std::string state = block_obj["state"].toString().toStdString();
                    if(state == "DISPONIBLE")
                    {
                        block.state = BlockState::DISPONIBLE;
                    }
                    else if(state == "OCUPADO")
                    {
                        std::string id_sub = block_obj["id_subject"].toString().toStdString();
                        block.state = BlockState::OCUPADO;
                        block.id_subject = id_sub;
                    }
                    else block.state = BlockState::NO_DISPONIBLE;

                    weekly_schedule[day][hour] = block;
                }
            }
            teacher.set_weekly_schedule(weekly_schedule);
        }
        teachers.push_back(teacher);
    }
    return teachers;
}

void Teacher::set_state_block(int day, int hour, BlockState state)
{
    if(day >= 0 && day < 7 && hour >= 0 && hour < 12)
    {
        weekly_schedule[day][hour].state = state;
        if(state != OCUPADO)
        {
            weekly_schedule[day][hour].id_subject = "";
        }
    }
}


void Teacher::assign_block(int day, int hour, const std::string &subject_id)
{
    if(day >= 0 && day < 7 && hour >= 0 && hour < 12)
    {
        weekly_schedule[day][hour].state = OCUPADO;
        weekly_schedule[day][hour].id_subject = subject_id;
    }
}

bool Teacher::available_block(int day, int hour) const
{
    if(day >= 0 && day < 7 && hour >= 0 && hour < 12)
    {
        if(weekly_schedule[day][hour].state == DISPONIBLE)
        {
            return true;
        }
    }
    return false;
}


const TimeBlock& Teacher::get_timeblock(int day, int hour) const
{
    return weekly_schedule[day][hour];
}



void Section::set_teacher_section(Teacher teacher_)
{
    teacher = teacher_;
}

void Section::set_subject_section(Subject subject_)
{
    subject = subject_;
}


void Section::set_id_section(size_t id_section_)
{
    id_section = id_section_;
}

Teacher Section::get_teacher_section()
{
    return teacher;
}


Subject Section::get_subject_section()
{
    return subject;
}

size_t Section::get_id_section()
{
    return id_section;
}

void Section::add_timeblock(int day, int hour)
{
    assigned_blocks.emplace_back(day, hour);
}

const std::vector<std::pair<int, int>>& Section::get_assigned_blocks() const
{
    return assigned_blocks;
}


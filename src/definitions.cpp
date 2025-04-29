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



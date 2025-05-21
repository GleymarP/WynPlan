#include "definitions.h"


//Subjects
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
int Subject::get_required_hours() const
{
    return required_hours;
}

//Semester
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
bool Semester::has_subject(const std::string& subject_id, std::vector<Subject>& subjects)
{
    for (const auto& subject : subjects)
    {
        if (subject.get_id() == subject_id)
        {
            return true;
        }
    }
    return false;
}

//StudyPlan
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
void StudyPlan::save_studyplan_json(const StudyPlan& plan, const QString& file_path)
{
    QJsonObject root;
    QJsonObject plan_obj;

    plan_obj["nombre"] = QString::fromStdString(plan.get_degree());

    QJsonObject semesters_obj;

    for (Semester& semester : plan.get_semester())
    {
        QJsonArray subjects_array;

        for (Subject& subject : semester.get_subjects_semester())
        {
            QJsonObject subject_obj;
            subject_obj["nombre"] = QString::fromStdString(subject.get_subject_name());
            subject_obj["codigo"] = QString::fromStdString(subject.get_id());
            subject_obj["horas"] = subject.get_required_hours();

            subjects_array.append(subject_obj);
        }

        semesters_obj[QString::fromStdString(semester.get_semester_name())] = subjects_array;
    }

    plan_obj["semestres"] = semesters_obj;

    root["plan_de_estudio"] = plan_obj;

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "No se pudo abrir el archivo para escritura:" << file_path;
        return;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
}

//Teacher
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
const TimeBlock& Teacher::get_timeblock(int day, int hour) const
{
    return weekly_schedule[day][hour];
}
const Teacher::WeeklySchedule& Teacher::get_weekly_schedule() const
{
    return weekly_schedule;
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
void Teacher::set_time_block(int day, int hour, const TimeBlock& block)
{
    if (day >= 0 && day < 7 && hour >= 0 && hour < 12)
    {
        weekly_schedule[day][hour] = block;
    }
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
            std::string subject_id = subject_value.toString().toStdString();
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
void Teacher::save_teachers_json(const std::vector<Teacher>& teachers, const QString& file_path)
{
    QJsonArray teachers_array;
    for(const auto& teacher : teachers)
    {
        QJsonObject teacher_obj;
        teacher_obj["nombre"] = QString::fromStdString(teacher.get_full_name());
        teacher_obj["cedula"] = QString::fromStdString(teacher.get_id());

        QJsonArray materias_array;
        for (const auto& subject : teacher.get_subjects())
        {
            materias_array.append(QString::fromStdString(subject.get_id()));
        }
        teacher_obj["materias"] = materias_array;
        QJsonArray weekly_schedule_array;
        for (int day = 0; day < 7; day++)
        {
            QJsonArray day_array;
            for (int hour = 0; hour < 12; hour++)
            {
                TimeBlock block = teacher.get_timeblock(day, hour);
                QJsonObject block_obj;
                QString state;

                if(block.state == BlockState::DISPONIBLE)
                {
                    state = "DISPONIBLE";
                }
                else if(block.state == BlockState::OCUPADO)
                {
                    state = "OCUPADO";
                }
                else
                {
                    state = "NO_DISPONIBLE";
                }

                block_obj["state"] = state;
                block_obj["id_subject"] = QString::fromStdString(block.id_subject);
                day_array.append(block_obj);
            }
            weekly_schedule_array.append(day_array);
        }
        teacher_obj["weekly_schedule"] = weekly_schedule_array;
        teachers_array.append(teacher_obj);
    }

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "No se pudo abrir el archivo para escritura:" << file_path;
    }

    QJsonDocument doc(teachers_array);
    file.write(doc.toJson());
    file.close();
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
void Teacher::assign_block(int day, int hour, const std::string &subject_id)
{
    if(day >= 0 && day < 7 && hour >= 0 && hour < 12)
    {
        weekly_schedule[day][hour].state = OCUPADO;
        weekly_schedule[day][hour].id_subject = subject_id;
    }
}

//Section
void Section::set_teacher_section(std::string teacher)
{
    teacher_id = teacher;
}
void Section::set_subject_section(std::string subject)
{
    subject_id = subject;
}
void Section::set_id_section(size_t id_section_)
{
    id_section = id_section_;
}
void Section::set_blocks(std::vector<std::pair<int, int>> blocks)
{
    assigned_blocks =  blocks;
}

std::string Section::get_teacher_section()
{
    return teacher_id;
}
std::string Section::get_subject_section()
{
    return subject_id;
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

//Assigment
void Assigment::set_semester_name(std::string name)
{
    semester_name = name;
}
void Assigment::set_sections_vector(std::vector<Section> vector)
{
    sections_vector = vector;
}
void Assigment::set_option(std::string option_)
{
    option = option_;
}

std::string Assigment::get_semester_name()
{
    return semester_name;
}
std::string Assigment::get_option()
{
    return option;
}
std::vector<Section> Assigment::get_sections_vector()
{
    return sections_vector;
}

std::vector<Assigment> Assigment::load_from_json_assing(const QString &file_path, StudyPlan& study_plan, std::vector<Teacher>& teachers)
{
    std::vector<Assigment> assignments;
    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "No se pudo abrir el archivo:" << file_path;
        return assignments;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull() || !doc.isArray())
    {
        qWarning() << "Archivo JSON inválido o no es un array:" << file_path;
        return assignments;
    }

    QJsonArray root_array = doc.array();
    size_t section_counter = 0;

    for(const QJsonValue& assignment_value : root_array)
    {
        if (!assignment_value.isObject()) continue;

        QJsonObject assignment_obj = assignment_value.toObject();
        Assigment assignment;

        assignment.set_semester_name(assignment_obj["Semester"].toString().toStdString());
        assignment.set_option(assignment_obj["Option"].toString().toStdString());

        QJsonArray sections_array = assignment_obj["sections"].toArray();
        std::vector<Section> sections;

        for(const QJsonValue& section_value : sections_array)
        {
            if (!section_value.isObject()) continue;

            QJsonObject section_obj = section_value.toObject();
            Section section;

            section.set_teacher_section(section_obj["teacher_id"].toString().toStdString());
            section.set_subject_section(section_obj["subject_id"].toString().toStdString());
            section.set_id_section(section_obj["section_id"].toInt());

            QJsonArray time_blocks = section_obj["time_blocks"].toArray();
            for(const QJsonValue& block_value : time_blocks)
            {
                if (!block_value.isObject()) continue;

                QJsonObject block_obj = block_value.toObject();
                int day = block_obj["day"].toInt();
                int hour = block_obj["hour"].toInt();

                section.add_timeblock(day, hour);

                for (Teacher& teacher : teachers)
                {
                    if (teacher.get_id() == section.get_teacher_section())
                    {
                        TimeBlock block;
                        block.state = BlockState::OCUPADO;
                        block.id_subject = section.get_subject_section();
                        teacher.set_time_block(day, hour, block);
                        break;
                    }
                }
            }

            sections.push_back(section);
            section_counter++;
        }

        assignment.set_sections_vector(sections);
        assignments.push_back(assignment);
    }

    return assignments;
}
void Assigment::save_assigments_json(std::vector<Assigment>& assignments, const QString& file_path, StudyPlan&  plan_)
{
    QFile file(file_path);
    QJsonArray root_array;

    for(Assigment& assignment : assignments)
    {
        bool semester_exists = false;
        for(Semester& semester : plan_.get_semester())
        {
            if(semester.get_semester_name() == assignment.get_semester_name())
            {
                semester_exists = true;
                break;
            }
        }

        if(!semester_exists)
        {
            qWarning() << "Semestre" << QString::fromStdString(assignment.get_semester_name())
            << "no encontrado en el plan de estudios";
            continue;
        }

        QJsonObject assignment_obj;
        assignment_obj["Semester"] = QString::fromStdString(assignment.get_semester_name());
        assignment_obj["Option"] = QString::fromStdString(assignment.get_option());

        QJsonArray sections_array;
        for(Section& section : assignment.get_sections_vector())
        {
            QJsonObject section_obj;
            section_obj["teacher_id"] = QString::fromStdString(section.get_teacher_section());
            section_obj["subject_id"] = QString::fromStdString(section.get_subject_section());
            section_obj["section_id"] = static_cast<int>(section.get_id_section());

            QJsonArray blocks_array;
            for(const auto& block : section.get_assigned_blocks())
            {
                QJsonObject block_obj;
                block_obj["day"] = block.first;
                block_obj["hour"] = block.second;
                blocks_array.append(block_obj);
            }
            section_obj["time_blocks"] = blocks_array;
            sections_array.append(section_obj);
        }
        assignment_obj["sections"] = sections_array;
        root_array.append(assignment_obj);
    }

    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.write(QJsonDocument(root_array).toJson());
        file.close();
    }
    else
    {
        qWarning() << "No se pudo abrir el archivo para escritura:" << file_path;
    }
}

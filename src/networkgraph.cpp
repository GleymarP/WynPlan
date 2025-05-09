#include "networkgraph.h"

void ScheduleManager::buildNetwork()
{
    addTeachersToNetwork();
    addSubjectsToNetwork();
    addSchedulesToNetwork();
    connectTeachersToSubjects();
    connectSubjectsToSchedules();
    addCollisionConstraints();
}

void ScheduleManager::addTeachersToNetwork()
{
    for (const auto& teacher : teachers)
    {
        auto node = graph.insert_node({teacher.get_full_name(),"teacher", teacher.get_full_name()});
        teacher_nodes[teacher.get_full_name()] = node;
        graph.insert_arc(source, node, {8, 0, "teacher_capacity_" + teacher.get_full_name()});
    }
}

void ScheduleManager::addSubjectsToNetwork()
{
    for (const auto& semester : study_plan.get_semester())
    {
        for (const auto& subject : semester.get_subjects_semester())
        {
            auto node = graph.insert_node({subject.get_id(),"subject",subject.get_subject_name() + " (" + semester.get_semester_name() + ")"});
            subject_nodes[subject.get_id()] = node;
            graph.insert_arc(node,sink,{1, 0, "subject_requirement_" + subject.get_id()});
        }
    }
}

void ScheduleManager::addSchedulesToNetwork()
{
    std::set<Schedule> all_schedules;
    for (auto& teacher : teachers)
    {
        for (auto& schedule : teacher.get_availability())
        {
            all_schedules.insert(schedule);
        }
    }

    for (const auto& schedule : all_schedules)
    {
        std::string schedule_id = schedule.get_day() + "_" + schedule.get_start_time() + "_" + schedule.get_end_time();

        auto node = graph.insert_node({schedule_id, "schedule", schedule.get_day() + " " + schedule.get_start_time() + "-" + schedule.get_end_time()});
        schedule_nodes[schedule_id] = node;
    }
}

void ScheduleManager::connectTeachersToSubjects()
{
    for (const auto& teacher : teachers)
    {
        auto teacher_node = teacher_nodes[teacher.get_full_name()];

        for (const auto& subject : teacher.get_subjects())
        {
            if (subject_nodes.count(subject.get_id()))
            {
                auto subject_node = subject_nodes[subject.get_id()];
                graph.insert_arc(teacher_node,subject_node,{1, 0, "can_teach_" + teacher.get_full_name() + "_" + subject.get_id()});
            }
        }
    }
}

void ScheduleManager::connectSubjectsToSchedules()
{
    for (const auto& [subject_id, subject_node] : subject_nodes)
    {
        auto intermediate_node = graph.insert_node({"inter_" + subject_id,"subject_schedule", "Scheduling for " + subject_id
        });

        graph.insert_arc(subject_node,intermediate_node, {1, 0, "scheduling_" + subject_id} );


        for (const auto& [schedule_id, schedule_node] : schedule_nodes) {
            graph.insert_arc(
                intermediate_node,
                schedule_node,
                {1, 0, "possible_time_" + subject_id + "_" + schedule_id}
                );
        }

        // Conexión horario -> sumidero
        for (const auto& [schedule_id, schedule_node] : schedule_nodes) {
            graph.insert_arc(
                schedule_node,
                sink,
                {1, 0, "time_slot_" + schedule_id}
                );
        }
    }
}

void ScheduleManager::addCollisionConstraints() {
    for (const auto& semester : study_plan.get_semester()) {
        const auto& subjects = semester.get_subjects_semester();

        for (size_t i = 0; i < subjects.size(); ++i) {
            for (size_t j = i + 1; j < subjects.size(); ++j) {
                std::string subj1_id = subjects[i].get_id();
                std::string subj2_id = subjects[j].get_id();

                if (!subject_nodes.count(subj1_id) || !subject_nodes.count(subj2_id)) {
                    continue;
                }
                auto constraint_node = graph.insert_node({
                    "constraint_" + subj1_id + "_" + subj2_id,
                    "constraint",
                    "No overlap " + subj1_id + " and " + subj2_id
                });
            }
        }
    }
}




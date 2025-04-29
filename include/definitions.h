#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include<string>
#include<vector>

struct Subject
{
    std::string id;
    std::string name;
    size_t credit_units;
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



#endif // DEFINITIONS_H


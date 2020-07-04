//
// VE280 Lab 7, SU2020
// Created by cyx on 20-5-22.
//

#include <iostream>
#include "course.h"

typedef struct task {
    std::string type;
    int index{}; // Task index of type
    int due_month{};
    int due_day{};
} Task;
/*
 * Task is used to represent tasks in a course.
 * type could be "Lab", "Project" and etc., which specifies the type of the task,
 * index is the number of task of this type,
 * due_month and due_day specifies its due date
 */

const int MAXTASKS = 4; // default maximum number of tasks in a course at a time
class TechnicalCourse : public Course {
protected:
    Task *tasks{};
    int numTasks{};
    int sizeTasks{};
    std::string course_code;
public:
    TechnicalCourse() = default;

    explicit TechnicalCourse(const std::string &course_code, int size = MAXTASKS) {
        this->tasks = new Task[size];
        this->numTasks = 0;
        this->sizeTasks = size;
        this->course_code = course_code;
    }

    ~TechnicalCourse() override {
        delete[] tasks;
    };

    void updateTask(const std::string &type, int index, int due_month, int due_day) override;

    void finishTask(const std::string &type, int index, int finish_month, int finish_day) override;

    void print() override;
};

void TechnicalCourse::updateTask(const std::string &type, int index, int due_month, int due_day)
// REQUIRES: due_month and due_day are in normal range.
// MODIFIES: this
// EFFECTS: if Task index of type already exists in tasks, updates its due_month & due_day ;
//          otherwise, adds Task i of type at the end of tasks if tasks is not full,
//          throw an exception if tasks is full
{
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i].type == type && tasks[i].index == index) {
            tasks[i].due_month = due_month;
            tasks[i].due_day = due_day;
            return;
        } else {
            continue;
        }
    }

    if (numTasks == sizeTasks) throw tooManyTasks{};

    tasks[numTasks].type = type;
    tasks[numTasks].index = index;
    tasks[numTasks].due_month = due_month;
    tasks[numTasks].due_day = due_day;
    numTasks++;

    if (type == "Lab" || type == "Project") {
        std::cout << course_code << " " << type << " " << index << " is released! Submit it via oj!" << std::endl;
    } else {
        std::cout << course_code << " " << type << " " << index << " is released! Submit it via canvas!" << std::endl;
    }
}

void TechnicalCourse::finishTask(const std::string &type, int index, int finish_month, int finish_day)
// REQUIRES: Task index of type is in tasks. finish_month and finish_day are in normal range.
// MODIFIES: this
// EFFECTS: removes Task index of type
{
    int temp = -1;
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i].type == type && tasks[i].index == index) {
            temp = i;
            if (finish_month < tasks[i].due_month ||
                (finish_month == tasks[i].due_month && finish_day <= tasks[i].due_day)) {
                std::cout << course_code << " " << type << " " << index << " is finished!" << std::endl;
            } else {
                std::cout << course_code << " " << type << " " << index << " is overdue!" << std::endl;
            }
        } else {
            continue;
        }
    }
    if (temp == -1) {
        return;
    }

    for (int i = temp; i < numTasks; i++) {
        tasks[i] = tasks[i + 1];
    }
    tasks[numTasks - 1] = Task{};
    numTasks--;
}

void TechnicalCourse::print() {
    std::cout << course_code << std::endl;
    for (int i = 0; i < numTasks; ++i) {
        std::cout << tasks[i].type << " " << tasks[i].index << ": "
                  << tasks[i].due_month << "/" << tasks[i].due_day << std::endl;
    }
}

class UpperlevelTechnicalCourse : public TechnicalCourse {
public:
    explicit UpperlevelTechnicalCourse(const std::string &course_code, int size = MAXTASKS) {
        this->tasks = new Task[size];
        this->numTasks = 0;
        this->sizeTasks = size;
        this->course_code = course_code;
    };

    ~UpperlevelTechnicalCourse() override = default;

    void updateTask(const std::string &type, int index, int due_month, int due_day) override;
};

void UpperlevelTechnicalCourse::updateTask(const std::string &type, int index, int due_month, int due_day)
// REQUIRES: due_month and due_day are in normal range.
// MODIFIES: this
// EFFECTS: If Task index of type does not exist in tasks,
//          adds Task i of type according to its due date if tasks is not full, throw an exception if tasks is full.
//          If Task index of type already exists in tasks,
//          updates its position in tasks & due_month & due_day if its due_month/due_day is changed,
//          do nothing if its due_month/due_day is unchanged.
{
    bool flag = false;
    int ptr;
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i].type == type && tasks[i].index == index) {
            if (tasks[i].due_day == due_day && tasks[i].due_month == due_month) {
                return;
            } else {
                ptr = i;
                flag = true;
            }
        }
    }
    if (!flag && numTasks == sizeTasks) {
        throw tooManyTasks{};
    }

    if (flag) {
        tasks[ptr].due_month = due_month;
        tasks[ptr].due_day = due_day;
        for (int i = 0; i < numTasks - 1; i++) {
            if (tasks[i].due_month < tasks[i + 1].due_month ||
                (tasks[i].due_month == tasks[i + 1].due_month && tasks[i].due_day < tasks[i + 1].due_day)) {
                continue;
            } else if (tasks[i].due_month == tasks[i + 1].due_month && tasks[i].due_day == tasks[i + 1].due_day) {
                if (ptr == i) {
                    Task task;
                    task = tasks[i];
                    tasks[i] = tasks[i + 1];
                    tasks[i + 1] = task;
                    ptr++;
                } else {
                    continue;
                }
            } else {
                Task task;
                task = tasks[i];
                tasks[i] = tasks[i + 1];
                tasks[i + 1] = task;
                ptr++;
            }
        }
    }

    ptr = 0;
    while (ptr < numTasks) {
        if (due_month < tasks[ptr].due_month ||
            (tasks[ptr].due_month == due_month && due_day < tasks[ptr].due_day)) {
            break;
        } else {
            ptr++;
        }
    }

    for (int i = numTasks; i > ptr; i--) {
        tasks[i] = tasks[i - 1];
    }

    tasks[ptr].type = type;
    tasks[ptr].index = index;
    tasks[ptr].due_day = due_day;
    tasks[ptr].due_month = due_month;

    if (!flag) {
        numTasks++;
        std::cout << course_code << " " << type << " " << index << " is released! ";
        if (type == "Team Project") {
            std::cout << "Submit it via github!" << std::endl;
        } else if (type == "Lab" || type == "Project") {
            std::cout << "Submit it via oj!" << std::endl;
        } else {
            std::cout << "Submit it via canvas!" << std::endl;
        }
    }
}

Course *create(const std::string &class_type, const std::string &course_code, bool assign_size, int tasks_size) {
    if (class_type == "Technical") {
        if (assign_size) {
            return new TechnicalCourse(course_code, tasks_size);
        } else {
            return new TechnicalCourse(course_code);
        }
    } else if (class_type == "Upper Level Technical") {
        if (assign_size) {
            return new UpperlevelTechnicalCourse(course_code, tasks_size);
        } else {
            return new UpperlevelTechnicalCourse(course_code);
        }
    } else {
        return nullptr;
    }
}

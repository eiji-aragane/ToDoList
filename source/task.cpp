// task.cpp

#include "task.h"

#include <iostream>

Task::Task(int id, std::string taskName)
    : taskId(id), taskName(taskName), taskStatus(TaskStatus::Incomplete) {}
Task::~Task() {}

TaskResult Task::doneTask() {
    taskStatus = TaskStatus::Complete;
    return TaskResult::SUCCESS;
}
void Task::showTask() const {
    std::string status{};
    if (taskStatus == TaskStatus::Complete) {
        status = "完了";
    } else {
        status = "未完了";
    }
    std::cout << "[" << taskId << "]" << " " << taskName << " ステータス："
              << status << std::endl;
}
int Task::getId() const { return taskId; }
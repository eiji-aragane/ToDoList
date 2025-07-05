#include "taskManager.h"

#include <algorithm>

TaskManager::TaskManager() {}
TaskManager::~TaskManager() {}

TaskResult TaskManager::addTask(const std::string& taskName) {
    TaskResult result{TaskResult::SUCCESS};
    taskList.emplace_back(std::make_shared<Task>(nextId++, taskName));
    return result;
}

TaskResult TaskManager::deleteTask(int taskId) {
    auto oldSize = taskList.size();

    auto it = std::remove_if(taskList.begin(), taskList.end(),
                             [taskId](const std::shared_ptr<Task>& task) {
                                 return task->getId() == taskId;
                             });
    taskList.erase(it, taskList.end());

    return (taskList.size() < oldSize) ? TaskResult::SUCCESS : TaskResult::FAIL;
}

TaskResult TaskManager::doneTask(int taskId) {
    TaskResult result{TaskResult::FAIL};
    for (const std::shared_ptr<Task>& task : taskList) {
        if (task && (task->getId() == taskId)) {
            result = task->doneTask();
        }
    }
    return result;
}

void TaskManager::showTask() const {
    for (const std::shared_ptr<Task>& task : taskList) {
        if (task) {
            task->showTask();
        }
    }
}

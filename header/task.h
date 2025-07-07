// タスククラス
#ifndef TASK_H
#define TASK_H
#include <string>

enum TaskStatus { Incomplete, Complete };

enum TaskResult { SUCCESS, FAIL };

class Task {
   public:
    Task(int id, std::string taskName);
    ~Task();

    TaskResult doneTask();
    void showTask() const;
    int getId() const;
    std::string getName() const;
    TaskStatus getTaskStatus() const;
    void setStatus(TaskStatus taskStatus);

   private:
    int taskId;
    std::string taskName;
    TaskStatus taskStatus;
};

#endif
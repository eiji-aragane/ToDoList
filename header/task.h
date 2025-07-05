// タスククラス
#ifndef TASK_H
#define TASK_H
#include <string>

enum TaskStatus { Incomplete, Complete };

// struct task {
//     int id;
//     std::string taskName;
//     TaskStatus taskStatus;
// };

enum TaskResult { SUCCESS, FAIL };

class Task {
   public:
    Task(int id, std::string taskName);
    ~Task();

    TaskResult doneTask();
    void showTask() const;
    int getId() const;

   private:
    int taskId;
    std::string taskName;
    TaskStatus taskStatus;
};

#endif
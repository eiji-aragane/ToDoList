// タスクマネージャクラス
// taskManager.h

#include <memory>
#include <string>
#include <vector>

#include "task.h"

class TaskManager {
   public:
    TaskManager();
    ~TaskManager();

    TaskResult addTask(const std::string& taskName);
    TaskResult deleteTask(int taskId);
    TaskResult doneTask(int taskId);
    void showTask() const;

   private:
    std::vector<std::shared_ptr<Task>> taskList;
    int nextId = 1;
};
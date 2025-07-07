// タスクマネージャクラス
// taskManager.h

#include <memory>
#include <string>
#include <vector>

#include "json.hpp"
#include "task.h"

using json = nlohmann::json;

class TaskManager {
   public:
    TaskManager();
    ~TaskManager();

    TaskResult addTask(const std::string& taskName);
    TaskResult deleteTask(int taskId);
    TaskResult doneTask(int taskId);
    void showTask() const;
    TaskResult saveToFile() const;
    TaskResult readFromFile();

   private:
    std::string findBackUpFile() const;
    void deleteAllBackUp();
    std::vector<std::shared_ptr<Task>> taskList;
    int nextId = 1;
};
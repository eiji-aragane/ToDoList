#include "taskManager.h"

#include <direct.h>
#include <sys/stat.h>
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string FOLDER{"backup/"};

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

TaskResult TaskManager::saveToFile() const {
    TaskResult result{TaskResult::FAIL};
    auto nowTime{std::chrono::system_clock::now()};
    std::time_t time_now = std::chrono::system_clock::to_time_t(nowTime);
    std::tm local_tm;

    // フォルダが無ければ作成する
    struct stat info;
    if (stat(FOLDER.c_str(), &info) != 0) {
        _mkdir(FOLDER.c_str());
    }

#if defined(_WIN32)
    // MinGW では localtime_r がないことがある
    std::tm* tmp = std::localtime(&time_now);
    if (tmp != nullptr) {
        local_tm = *tmp;
    }
#else
    localtime_r(&time_now, &local_tm);
#endif

    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y-%m-%d_%H-%M-%S") << "_backup.json";

    json jsonData;
    for (const std::shared_ptr<Task>& task : taskList) {
        if (task) {
            jsonData.push_back(
                {{"id", task->getId()},
                 {"name", task->getName()},
                 {"status", task->getTaskStatus() == TaskStatus::Complete
                                ? "Complete"
                                : "Incomplete"}});
        }
    }

    std::ofstream out(FOLDER + ss.str());
    if (out) {
        out << jsonData.dump(4);
        result = TaskResult::SUCCESS;
    }
    return result;
}

TaskResult TaskManager::readFromFile() {
    if (!findBackUpFile().empty()) {
        std::ifstream in(findBackUpFile());
        if (!in) return TaskResult::FAIL;

        json j;
        in >> j;

        taskList.clear();

        int maxId{0};

        for (const auto& item : j) {
            int id = item["id"];
            std::string name = item["name"];
            std::string statusStr = item["status"];

            TaskStatus status = (statusStr == "Complete")
                                    ? TaskStatus::Complete
                                    : TaskStatus::Incomplete;

            auto task = std::make_shared<Task>(id, name);
            task->setStatus(status);  // setStatus を用意しておく
            taskList.push_back(task);

            if (id > maxId) {
                maxId = id;
            }
        }
        nextId = maxId + 1;
        in.close();
        deleteAllBackUp();

    } else {
        std::cout << "バックアップファイルが見つかりません\n";
        return TaskResult::FAIL;
    }

    return TaskResult::SUCCESS;
}

std::string TaskManager::findBackUpFile() const {
    std::vector<std::string> files;

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;

    std::string searchPath = FOLDER + "\\*.json";
    hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return "";
    }

    do {
        files.push_back(findFileData.cFileName);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (files.empty()) return "";

    // ソート（ファイル名にタイムスタンプがある前提）
    std::sort(files.begin(), files.end());

    return FOLDER + "\\" + files.back();
}

void TaskManager::deleteAllBackUp() {
    std::string searchPath = FOLDER + "\\*.json";

    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "バックアップファイルが存在しません\n";
        return;
    }

    do {
        std::string filePath = FOLDER + "\\" + findFileData.cFileName;
        if (!DeleteFileA(filePath.c_str())) {
            std::cerr << "削除に失敗しました: " << filePath << "\n";
        }
    } while (FindNextFileA(hFind, &findFileData));

    FindClose(hFind);
}
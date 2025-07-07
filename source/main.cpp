#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include "taskManager.h"

int main() {
    // taskManager生成
    TaskManager taskManager{};
    taskManager.readFromFile();

    // task生成
    std::cout << "===ToDoアプリへようこそ===" << std::endl;
    std::string command{};

    while (true) {
        std::cout << "===コマンドを入力してください===" << std::endl;
        std::cout << "add:タスクを追加する" << std::endl;
        std::cout << "delete:タスクを削除する" << std::endl;
        std::cout << "done:タスクを完了させる" << std::endl;
        std::cout << "list:タスクの一覧を表示する" << std::endl;
        std::cout << "finish:アプリを終了させる" << std::endl;

        std::getline(std::cin, command);
        if (command == "add") {
            std::cout << "タスク名を入力してください" << std::endl;
            std::string taskName{};
            std::getline(std::cin, taskName);
            // タスクを追加する関数
            TaskResult result{taskManager.addTask(taskName)};
            if (result == TaskResult::SUCCESS) {
                std::cout << "タスク追加が成功しました" << std::endl;
            } else {
                std::cout << "タスク追加に失敗しました" << std::endl;
            }
        } else if (command == "delete") {
            std::cout << "削除するタスクIDを入力してください" << std::endl;
            int taskId{};
            std::cin >> taskId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "タスクID:" << taskId << "を削除します" << std::endl;
            TaskResult result{taskManager.deleteTask(taskId)};
            if (result == TaskResult::SUCCESS) {
                std::cout << "タスク削除が成功しました" << std::endl;
            } else {
                std::cout << "タスク削除に失敗しました" << std::endl;
            }
        } else if (command == "done") {
            std::cout << "完了するタスクIDを入力してください" << std::endl;
            int taskId{};
            std::cin >> taskId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "タスクID:" << taskId << "を完了します" << std::endl;
            TaskResult result{taskManager.doneTask(taskId)};
            if (result == TaskResult::SUCCESS) {
                std::cout << "タスク完了が成功しました" << std::endl;
            } else {
                std::cout << "タスク完了に失敗しました" << std::endl;
            }
        } else if (command == "list") {
            std::cout << "タスクを表示します" << std::endl;
            taskManager.showTask();
        } else if (command == "finish") {
            TaskResult result{taskManager.saveToFile()};
            std::cout << "終了します" << std::endl;
            break;
        } else {
            std::cout << "不明なコマンドです" << std::endl;
        }
    }
    return 0;
}

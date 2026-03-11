#include "TodoManager.h"
#include <iostream>
#include <limits>
#include <fstream>

TodoManager::TodoManager() : todoList(std::make_unique<TodoList>()) {}

void TodoManager::run() {
    std::cout << "=== 面向对象版待办事项列表 ===" << std::endl;
    std::cout << "当前文件: " << todoList->getFilename() << std::endl;

    handleMainMenu();
}

void TodoManager::printMainMenu() const {
    std::cout << "\n--- 主菜单 ---" << std::endl;
    std::cout << "1. 添加任务" << std::endl;
    std::cout << "2. 查看所有任务" << std::endl;
    std::cout << "3. 删除任务" << std::endl;
    std::cout << "4. 文件操作" << std::endl;
    std::cout << "5. 统计信息" << std::endl;
    std::cout << "6. 清空所有任务" << std::endl;
    std::cout << "7. 退出程序" << std::endl;
    std::cout << "请选择操作 (1-7): ";
}

void TodoManager::printFileMenu() const {
    std::cout << "\n--- 文件操作 ---" << std::endl;
    std::cout << "1. 保存任务到文件" << std::endl;
    std::cout << "2. 从文件加载任务" << std::endl;
    std::cout << "3. 查看文件内容" << std::endl;
    std::cout << "4. 更改文件名" << std::endl;
    std::cout << "5. 返回主菜单" << std::endl;
    std::cout << "请选择操作 (1-5): ";
}

void TodoManager::handleMainMenu() {
    int choice = 0;

    do {
        printMainMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            handleAddTask();
            break;
        case 2:
            todoList->viewTasks();
            break;
        case 3:
            handleDeleteTask();
            break;
        case 4:
            handleFileMenu();
            break;
        case 5:
            todoList->displayStatistics();
            break;
        case 6:
            todoList->clearAllTasks();
            break;
        case 7:
            std::cout << "感谢使用待办事项列表，再见！" << std::endl;
            break;
        default:
            std::cout << "无效选择，请重试。" << std::endl;
        }
    } while (choice != 7);
}

void TodoManager::handleFileMenu() {
    int fileChoice = 0;

    do {
        printFileMenu();
        std::cin >> fileChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (fileChoice) {
        case 1:
            handleSaveToFile();
            break;
        case 2:
            handleLoadFromFile();
            break;
        case 3:
            handleViewFileContent();
            break;
        case 4:
        {
            std::string newFilename;
            std::cout << "请输入新的文件名: ";
            std::getline(std::cin, newFilename);
            todoList->setFilename(newFilename);
        }
        break;
        case 5:
            std::cout << "返回主菜单..." << std::endl;
            break;
        default:
            std::cout << "无效选择，请重试。" << std::endl;
        }
    } while (fileChoice != 5);
}

void TodoManager::handleAddTask() {
    std::string task;
    std::cout << "请输入新任务: ";
    std::getline(std::cin, task);
    todoList->addTask(task);

    // 询问是否立即保存
    if (!task.empty()) {
        char autoSave;
        std::cout << "是否立即保存到文件？(y/n): ";
        std::cin >> autoSave;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (autoSave == 'y' || autoSave == 'Y') {
            todoList->saveToFile();
        }
    }
}

void TodoManager::handleDeleteTask() {
    todoList->viewTasks();
    if (!todoList->isEmpty()) {
        int taskNumber;
        std::cout << "请输入要删除的任务编号: ";
        std::cin >> taskNumber;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "请输入有效的数字！" << std::endl;
            return;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        bool is_ok = todoList->deleteTask(taskNumber);
        if (is_ok == true) {

        }
    }
}

void TodoManager::handleSaveToFile() {
    char choice;
    std::cout << "保存到默认文件 " << todoList->getFilename() << "？(y/n): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        todoList->saveToFile();
    }
    else {
        std::string customFilename;
        std::cout << "请输入自定义文件名: ";
        std::getline(std::cin, customFilename);
        todoList->saveToFile(customFilename);
    }
}

void TodoManager::handleLoadFromFile() {
    char choice;
    std::cout << "从默认文件 " << todoList->getFilename() << " 加载？(y/n): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'y' || choice == 'Y') {
        todoList->loadFromFile();
    }
    else {
        std::string customFilename;
        std::cout << "请输入要加载的文件名: ";
        std::getline(std::cin, customFilename);
        todoList->loadFromFile(customFilename);
    }
}

void TodoManager::handleViewFileContent() const {
    std::string filename = todoList->getFilename();
    std::ifstream file(filename);

    if (file.is_open()) {
        std::cout << "\n--- 文件内容 (" << filename << ") ---" << std::endl;
        std::string line;
        int lineNum = 1;
        while (std::getline(file, line)) {
            std::cout << lineNum << ". " << line << std::endl;
            lineNum++;
        }
        file.close();
    }
    else {
        std::cout << "文件不存在或无法打开！" << std::endl;
    }
}
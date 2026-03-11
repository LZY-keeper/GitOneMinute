#include "TodoList.h"
#include <iostream>
#include <limits>
#include <algorithm>

// 默认构造函数
TodoList::TodoList() : filename("todo_list.txt") {
    loadFromFile(); // 自动加载之前的任务
}

// 带文件名参数的构造函数
TodoList::TodoList(const std::string& file) : filename(file) {
    loadFromFile(); // 自动加载之前的任务
}

// 添加任务
void TodoList::addTask(const std::string& task) {
    if (!task.empty()) {
        tasks.push_back(task);
        std::cout << "任务添加成功！" << std::endl;
    }
    else {
        std::cout << "任务内容不能为空！" << std::endl;
    }
}

// 查看所有任务
void TodoList::viewTasks() const {
    if (tasks.empty()) {
        std::cout << "当前没有待办任务。" << std::endl;
        return;
    }

    std::cout << "\n--- 当前待办事项 ---" << std::endl;
    displayTasksInternal();
    std::cout << "总计: " << tasks.size() << " 个任务" << std::endl;
}

// 删除任务
bool TodoList::deleteTask(int taskNumber) {
    if (tasks.empty()) {
        std::cout << "当前没有可删除的任务。" << std::endl;
        return false;
    }

    if (isValidIndex(taskNumber)) {
        std::string removedTask = tasks[taskNumber - 1];
        tasks.erase(tasks.begin() + taskNumber - 1);
        std::cout << "任务 \"" << removedTask << "\" 已删除！" << std::endl;
    }
    else {
        std::cout << "无效的任务编号！" << std::endl;
    }
    return true;
}

// 清空所有任务
void TodoList::clearAllTasks() {
    tasks.clear();
    std::cout << "所有任务已清空！" << std::endl;
}

// 保存到默认文件
bool TodoList::saveToFile() {
    return saveToFile(filename);
}

// 保存到指定文件
bool TodoList::saveToFile(const std::string& customFilename)  {
    std::ofstream outputFile(customFilename);

    if (!outputFile.is_open()) {
        std::cout << "错误：无法创建或打开文件 " << customFilename << "！" << std::endl;
        return false;
    }

    for (const auto& task : tasks) {
        outputFile << task << std::endl;
    }

    outputFile.close();
    std::cout << "成功保存 " << tasks.size() << " 个任务到文件: " << customFilename << std::endl;
    return true;
}

// 从默认文件加载
bool TodoList::loadFromFile() {
    return loadFromFile(filename);
}

// 从指定文件加载
bool TodoList::loadFromFile(const std::string& customFilename) {
    std::ifstream inputFile(customFilename);

    if (!inputFile.is_open()) {
        std::cout << "文件 " << customFilename << " 不存在或无法打开，将创建新文件。" << std::endl;
        return false;
    }

    std::vector<std::string> loadedTasks;
    std::string line;
    int loadedCount = 0;

    while (std::getline(inputFile, line)) {
        if (!line.empty()) {
            loadedTasks.push_back(line);
            loadedCount++;
        }
    }

    inputFile.close();

    if (loadedCount > 0) {
        tasks = loadedTasks; // 替换当前任务列表
        filename = customFilename; // 更新文件名
        std::cout << "从文件加载了 " << loadedCount << " 个任务。" << std::endl;
        return true;
    }

    return false;
}

// 获取任务数量
int TodoList::getTaskCount() const {
    return tasks.size();
}

// 检查是否为空
bool TodoList::isEmpty() const {
    return tasks.empty();
}

// 获取特定任务
std::string TodoList::getTask(int index) const {
    if (isValidIndex(index)) {
        return tasks[index - 1];
    }
    return "无效的任务索引";
}

// 获取文件名
std::string TodoList::getFilename() const {
    return filename;
}

// 设置文件名
void TodoList::setFilename(const std::string& newFilename) {
    filename = newFilename;
    std::cout << "文件名已更新为: " << filename << std::endl;
}

// 显示统计信息
void TodoList::displayStatistics() const {
    std::cout << "\n--- 统计信息 ---" << std::endl;
    std::cout << "任务总数: " << getTaskCount() << std::endl;
    std::cout << "存储文件: " << getFilename() << std::endl;
    std::cout << "是否为空: " << (isEmpty() ? "是" : "否") << std::endl;

    if (!isEmpty()) {
        std::cout << "前3个任务:" << std::endl;
        int displayCount = std::min(3, getTaskCount());
        for (int i = 1; i <= displayCount; ++i) {
            std::cout << "  " << i << ". " << getTask(i) << std::endl;
        }
    }
}

// 私有方法：检查索引是否有效
bool TodoList::isValidIndex(int index) const {
    return (index >= 1 && index <= static_cast<int>(tasks.size()));
}

// 私有方法：内部显示任务
void TodoList::displayTasksInternal() const {
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << i + 1 << ". " << tasks[i] << std::endl;
    }
}
#ifndef TODOLIST_H
#define TODOLIST_H

#include <vector>
#include <string>
#include <fstream>

class TodoList {
private:

    // 私有辅助方法
    bool isValidIndex(int index) const;
    void displayTasksInternal() const;

private:

    std::vector<std::string> tasks;
    std::string filename;


public:
    // 构造函数
    TodoList();
    explicit TodoList(const std::string& file);

    // 核心功能方法
    void addTask(const std::string& task);
    void viewTasks() const;
    bool deleteTask(int taskNumber);
    void clearAllTasks();

    // 文件操作方法
    bool saveToFile() ;
    bool loadFromFile();
    bool saveToFile(const std::string& customFilename);
    bool loadFromFile(const std::string& customFilename);

    // 查询方法
    int getTaskCount() const;
    bool isEmpty() const;
    std::string getTask(int index) const;

    // 状态信息
    std::string getFilename() const;
    void setFilename(const std::string& newFilename);

    // 工具方法
    void displayStatistics() const;
};

#endif // TODOLIST_H
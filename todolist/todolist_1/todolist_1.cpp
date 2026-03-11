#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

// todolist_1是没有本地存储的功能

// 函数声明
void printMenu();
void addTask(std::vector<std::string>& tasks);
void viewTasks(const std::vector<std::string>& tasks);
void deleteTask(std::vector<std::string>& tasks);



int main() {
    std::vector<std::string> tasks; // 存储任务的向量
    int choice = 0;

    std::cout << "=== 简易待办事项列表 ===" << std::endl;

    do {
        printMenu();
        std::cin >> choice;

        // 清除输入缓冲区，防止后续getline读取到换行符
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            addTask(tasks);
            break;
        case 2:
            viewTasks(tasks);
            break;
        case 3:
            deleteTask(tasks);
            break;
        case 4:
            std::cout << "感谢使用待办事项列表，再见！" << std::endl;
            break;
        default:
            std::cout << "无效选择，请重试。" << std::endl;
        }
    } while (choice != 4);

    return 0;
}

void printMenu() {
    std::cout << "\n--- 主菜单 ---" << std::endl;
    std::cout << "1. 添加任务" << std::endl;
    std::cout << "2. 查看所有任务" << std::endl;
    std::cout << "3. 删除任务" << std::endl;
    std::cout << "4. 退出程序" << std::endl;
    std::cout << "请选择操作 (1-4): ";
}

void addTask(std::vector<std::string>& tasks) {
    std::string task;
    std::cout << "请输入新任务: ";
    std::getline(std::cin, task);

    if (!task.empty()) {
        tasks.push_back(task);
        std::cout << "任务添加成功！" << std::endl;
    }
    else {
        std::cout << "任务内容不能为空！" << std::endl;
    }
}

void viewTasks(const std::vector<std::string>& tasks) {
    if (tasks.empty()) {
        std::cout << "当前没有待办任务。" << std::endl;
        return;
    }

    std::cout << "\n--- 当前待办事项 ---" << std::endl;
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::cout << i + 1 << ". " << tasks[i] << std::endl;
    }
    std::cout << "总计: " << tasks.size() << " 个任务" << std::endl;
}

void deleteTask(std::vector<std::string>& tasks) {
    if (tasks.empty()) {
        std::cout << "当前没有可删除的任务。" << std::endl;
        return;
    }

    viewTasks(tasks); // 先显示所有任务

    int taskNumber;
    std::cout << "请输入要删除的任务编号: ";
    std::cin >> taskNumber;

    // 检查输入是否为数字且有效
    if (std::cin.fail()) {
        std::cin.clear(); // 清除错误状态
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "请输入有效的数字！" << std::endl;
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除缓冲区

    if (taskNumber >= 1 && taskNumber <= static_cast<int>(tasks.size())) {
        std::string removedTask = tasks[taskNumber - 1];
        tasks.erase(tasks.begin() + taskNumber - 1);
        std::cout << "任务 \"" << removedTask << "\" 已删除！" << std::endl;
    }
    else {
        std::cout << "无效的任务编号！" << std::endl;
    }
}

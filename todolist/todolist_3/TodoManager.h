#ifndef TODOMANAGER_H
#define TODOMANAGER_H

#include "TodoList.h"
#include <memory>

class TodoManager {
private:

    void printMainMenu() const;
    void printFileMenu() const;
    void handleMainMenu();
    void handleFileMenu();
    void handleAddTask();
    void handleDeleteTask();
    void handleSaveToFile();
    void handleLoadFromFile();
    void handleViewFileContent() const;

private:

        std::unique_ptr<TodoList> todoList;

public:
    TodoManager();
    void run();
};

#endif // TODOMANAGER_H
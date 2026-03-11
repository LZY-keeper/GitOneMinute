#include <iostream>
#include "TodoManager.h"

int main() {
    try {
        TodoManager manager;
        manager.run();

        // 智能指针
        // std::unique_ptr<TodoManager> pmanager = std::make_unique<TodoManager>()
        //pmanager->run();
    }
    catch (const std::exception& e) {
        std::cerr << "程序发生异常: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
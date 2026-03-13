#include <memory>
#include <iostream>

class MyClass {
public:
    MyClass(int value) : data(value) {
        std::cout << "MyClass constructed: " << data << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destroyed: " << data << std::endl;
    }
private:
    int data;
};

int main() {
    // 1. 创建 shared_ptr
    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>(100); // 引用计数 = 1
    {
        // 内部就是其生命周期
        std::shared_ptr<MyClass> ptr2 = ptr1; // 引用计数 = 2
        std::shared_ptr<MyClass> ptr3 = ptr1; // 引用计数 = 3
        std::cout << "Inside block - use_count: " << ptr1.use_count() << std::endl;
    } // ptr2 和 ptr3 析构，引用计数 = 1

    std::cout << "Outside block - use_count: " << ptr1.use_count() << std::endl;

    // 2. 自定义删除器
    auto customDeleter = [](MyClass* p) {
        std::cout << "Custom deleter called!" << std::endl;
        delete p;
        };
    std::shared_ptr<MyClass> ptr4(new MyClass(200), customDeleter);

    // 3. 循环引用问题（需要注意！）
    struct Node {
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> prev;
        ~Node() { std::cout << "Node destroyed" << std::endl; }
    };

    auto node1 = std::make_shared<Node>();// 引用计数：node1=1
    auto node2 = std::make_shared<Node>();// 引用计数：node2=1
    node1->next = node2; // 引用计数：node1=1, node2=2
    node2->prev = node1; // 引用计数：node1=2, node2=2
    // 离开作用域时，引用计数都减为1，无法归零，导致内存泄漏！

    return 0;
    // ptr1 析构，引用计数归零，对象被销毁
    // ptr4 析构，调用自定义删除器
}



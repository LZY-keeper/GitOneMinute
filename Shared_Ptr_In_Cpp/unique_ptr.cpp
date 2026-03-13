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
    void doSomething() {
        std::cout << "Doing something with: " << data << std::endl;
    }
private:
    int data;
};

int main() {
    // 1. 创建 unique_ptr  注意，独占指针禁止赋值/拷贝
    std::unique_ptr<MyClass> ptr1(new MyClass(10));
    // 推荐使用 std::make_unique (C++14)
    auto ptr2 = std::make_unique<MyClass>(20);

    // 2. 使用指针
    ptr1->doSomething();
    (*ptr2).doSomething();

    // 3. 所有权转移
    std::unique_ptr<MyClass> ptr3 = std::move(ptr1); // ptr1 现在为 nullptr
    if (!ptr1) {
        std::cout << "ptr1 is now empty" << std::endl;
    }

    // 4. 重置和释放
    ptr2.reset(); // 主动释放对象，ptr2 变为 nullptr
    MyClass* rawPtr = ptr3.release(); // 释放所有权，返回原生指针，但不销毁对象
    // 现在必须手动管理 rawPtr
    delete rawPtr;
    //所以使用release是危险的，我们强烈推荐使用reset

    // 离开作用域时，剩余的所有智能指针会自动管理其生命周期
    return 0;
}


// unique_ptr内部实现原理:
// 通过删除拷贝构造函数和拷贝赋值运算符（= delete），只保留移动语义来确保唯一所有权。
template<typename T>
class unique_ptr {
private:
    T* ptr;
public:
    // 构造函数
    explicit unique_ptr(T* p = nullptr) : ptr(p) {}

    // 禁止拷贝
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // 允许移动
    unique_ptr(unique_ptr&& other) : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& other) {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // 析构函数
    ~unique_ptr() {
        delete ptr;
    }

    // 操作符重载
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    // 释放所有权
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;              // 注意release和reset的区别，一个依旧返回空，一个直接delete
                                  // 所以尽量使用reset
    }

    // 重置指针
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }
};
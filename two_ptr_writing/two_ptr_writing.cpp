#include <iostream>



// 手写UniquePtr（简化）
template<typename T>
class UniquePtr {
private:
    T* ptr;

public:
    // 显式构造函数
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // 析构函数
    ~UniquePtr() {
        delete ptr;
    }

    // 删除拷贝构造和拷贝赋值
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 移动构造函数
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // 移动赋值运算符
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;          // 释放当前资源
            ptr = other.ptr;     // 接管资源
            other.ptr = nullptr; // 置空原指针
        }
        return *this;
    }

    // 操作符重载
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // 实用函数
    T* get() const { return ptr; }
    explicit operator bool() const { return ptr != nullptr; }

    // 释放所有权
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // 重置
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }
};

// 测试手写 UniquePtr
class TestClass {
public:
    TestClass(int val) : value(val) {
        std::cout << "TestClass(" << value << ") constructed\n";
    }
    ~TestClass() {
        std::cout << "TestClass(" << value << ") destroyed\n";
    }
    void print() const {
        std::cout << "Value: " << value << std::endl;
    }
private:
    int value;
};

int main() {
    UniquePtr<TestClass> ptr1(new TestClass(100));
    ptr1->print();

    // UniquePtr<TestClass> ptr2 = ptr1; // 错误：拷贝构造被删除

    UniquePtr<TestClass> ptr3 = std::move(ptr1); // 正确：移动构造
    if (!ptr1) {
        std::cout << "ptr1 is now empty after move" << std::endl;
    }
    ptr3->print();

    return 0;
}


// 手写shared_ptr（简化版）
#include <iostream>

// 引用计数控制器
template<typename T>
class ControlBlock {
public:
    T* ptr;
    int ref_count;

    ControlBlock(T* p = nullptr) : ptr(p), ref_count(1) {}

    ~ControlBlock() {
        delete ptr;
    }
};

template<typename T>
class SharedPtr {
private:
    ControlBlock<T>* cb;

    // 清理函数
    void cleanup() {
        if (cb) {
            --cb->ref_count;
            if (cb->ref_count == 0) {
                delete cb;
            }
            cb = nullptr;
        }
    }

public:
    // 构造函数
    explicit SharedPtr(T* p = nullptr) {
        if (p) {
            cb = new ControlBlock<T>(p);
        }
        else {
            cb = nullptr;
        }
    }

    // 拷贝构造函数
    SharedPtr(const SharedPtr& other) : cb(other.cb) {
        if (cb) {
            ++cb->ref_count;
        }
    }

    // 拷贝赋值运算符
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            cleanup();  // 清理当前资源
            cb = other.cb;
            if (cb) {
                ++cb->ref_count;
            }
        }
        return *this;
    }

    // 移动构造函数
    SharedPtr(SharedPtr&& other) noexcept : cb(other.cb) {
        other.cb = nullptr;
    }

    // 移动赋值运算符
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            cb = other.cb;
            other.cb = nullptr;
        }
        return *this;
    }

    // 析构函数
    ~SharedPtr() {
        cleanup();
    }

    // 操作符重载
    T& operator*() const { return *(cb->ptr); }
    T* operator->() const { return cb->ptr; }

    // 实用函数
    T* get() const { return cb ? cb->ptr : nullptr; }
    int use_count() const { return cb ? cb->ref_count : 0; }
    explicit operator bool() const { return cb && cb->ptr; }
};

// 测试手写 SharedPtr
class SharedTest {
public:
    SharedTest(int val) : value(val) {
        std::cout << "SharedTest(" << value << ") constructed\n";
    }
    ~SharedTest() {
        std::cout << "SharedTest(" << value << ") destroyed\n";
    }
    void show() const {
        std::cout << "SharedTest value: " << value << std::endl;
    }
private:
    int value;
};

int main() {
    SharedPtr<SharedTest> ptr1(new SharedTest(42));
    std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl; // 1

    {
        SharedPtr<SharedTest> ptr2 = ptr1; // 拷贝构造
        std::cout << "ptr1 use_count after ptr2 copy: " << ptr1.use_count() << std::endl; // 2
        ptr2->show();

        SharedPtr<SharedTest> ptr3 = ptr1; // 另一个拷贝
        std::cout << "ptr1 use_count after ptr3 copy: " << ptr1.use_count() << std::endl; // 3
    } // ptr2 和 ptr3 析构

    std::cout << "ptr1 use_count after block: " << ptr1.use_count() << std::endl; // 1
    ptr1->show();

    return 0;
    // ptr1 析构，引用计数归零，对象被销毁
}
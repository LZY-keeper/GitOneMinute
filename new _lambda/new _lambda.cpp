#include <iostream>
#include<string>

int main() {
    int base = 100;
    std::string name = "Hello";

    // 一个捕获了 base（值）和 name（引用）的Lambda
    auto myLambda = [base, &name](int multiplier) mutable -> std::string {
        base *= multiplier; // 修改按值捕获的base的副本
        name += " World";   // 修改按引用捕获的name
        return name + " : " + std::to_string(base);
        };

    auto result = myLambda(2);
    std::cout << "Result: " << result << std::endl;
    std::cout << "External name: " << name << std::endl; // 外部name被修改了
    std::cout << "External base: " << base << std::endl; // 外部base未被修改

    return 0;
}


// 等价编译器生成的匿名类

#include <iostream>
#include <string>

// 这是我们手动模拟编译器生成的“匿名类”
class MyFunctor {
private:
    // 1. 捕获列表的变量成为成员变量
    int base_;          // 按值捕获 -> 普通成员
    std::string& name_; // 按引用捕获 -> 成员引用

public:
    // 2. 构造函数：初始化捕获来的变量
    MyFunctor(int base, std::string& name)
        : base_(base), name_(name) // 注意：name_直接绑定到外部的name
    {
    }

    // 3. 重载 operator()
    // 因为Lambda使用了 'mutable'，所以这个函数不是 const 的！
    // 返回类型也与Lambda声明一致
    auto operator()(int multiplier) -> std::string {
        base_ *= multiplier; // 修改的是成员变量 base_，不影响外部的base
        name_ += " World";   // 修改的是绑定的外部 name
        return name_ + " : " + std::to_string(base_);
    }
};

int main() {
    int base = 100;
    std::string name = "Hello";

    // 4. 创建我们手写的“闭包对象”
    // 这完全等价于 `auto myLambda = [base, &name]...`
    MyFunctor myLambda(base, name); // 将外部变量传入构造函数

    // 下面的调用和原始Lambda代码完全一致
    auto result = myLambda(2); // 调用 myLambda.operator()(2)
    std::cout << "Result: " << result << std::endl;
    std::cout << "External name: " << name << std::endl; // 输出：External name: Hello World
    std::cout << "External base: " << base << std::endl; // 输出：External base: 100

    return 0;
}
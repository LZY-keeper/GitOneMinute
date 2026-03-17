#include <iostream>
#include <thread>
#include <vector>

class UnsafeCounter {
private:
    int count = 0;

public:
    void increment() {
        // 这里存在数据竞争！
        count = count + 1;
    }

    int get_count() const { return count; }
};

void demonstrate_data_race() {
    UnsafeCounter counter;
    std::vector<std::thread> threads;

    // 创建10个线程，每个增加计数器1000次
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < 1000; ++j) {
                counter.increment();
            }
            });
    }

    for (auto& t : threads) {
        t.join();
    }

    // 结果可能小于10000！
    std::cout << "Final count: " << counter.get_count() << std::endl;
    // 可能输出: Final count: 8763 (而不是10000)
}

int main() {

    demonstrate_data_race();
    std::cout << "hello" << std::endl;
}

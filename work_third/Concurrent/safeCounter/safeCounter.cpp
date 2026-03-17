#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

class safeCounter {
private:
    std::mutex _mutex;  //普通的互斥锁
    int _count = 0;

public:

    void writeImage(){} // 使用互斥锁进行锁定
    void readImage(){} //使用读写锁进行锁定

    void increment() {

        //其他操作

        // _mutex.lock();      //阻塞
        /*if (!_mutex.try_lock())
            return; */          //不阻塞

        // std::unique_lock 灵活锁 在lock_guard 基础上增添了更多功能，支持中途解锁、支持 条件变量（重要）等
        //std::shared_mutex 读写锁 可同时在多线程锁定的内容进行读操作，不包含写操作

        {
            //这个锁出现的意义是  自动开关锁+控制作用域范围
            std::lock_guard<std::mutex> lock(_mutex);//构造函数会lock，析构函数会unlock（生命周期在操作结束后就没有了）

            // 这里存在数据竞争！
            _count = _count + 1;  //锁的是此范围内的操作，防止多线程一块锁效率不高

        }  //确定最小范围的锁 即 锁粒度控制


        //其他操作
        //_mutex.unlock();

        //其他操作
    }

    int get_count() const { return _count; }
};

void demonstrate_data_race() {
    safeCounter counter;
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

    // 结果将永远是10000！
    std::cout << "Final count: " << counter.get_count() << std::endl;
}


int main()
{
    std::cout << "Hello World!\n";
}



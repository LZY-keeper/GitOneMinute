// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ThreadSafeImageQueue.h"

int main()
{
    //该锁纯粹是为了让cout可以完整，实际生产者和消费者
    //不需要使用锁
    std::mutex coutMutex;
    ThreadSafeImageQueue<int> queue(10);//是一个int队列，而不是相片参数
    // 生产者线程
    std::thread producer([&]() {
        for (int i = 0; i < 100; ++i) {
            std::lock_guard<std::mutex> lock(coutMutex);
            bool ret = queue.push(i, 10);; //10ms  是超时时间
            std::cout << "Produced push:" << i <<
                (ret ? " successed" : " failed") << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }});

        // 消费者线程
        std::thread consumer([&]() {
            for (int i = 0; i < 230; ++i)
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                int value;
                bool ret = queue.pop(value, 5);//5ms之内 是超时时间
                if (ret == true)
                    std::cout << "Consumer pop successed:" << value << std::endl;
                else
                    std::cout << "Consumer pop failed!" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }
            });

        producer.join();
        consumer.join();

        std::cout << "Hello World!\n";
}

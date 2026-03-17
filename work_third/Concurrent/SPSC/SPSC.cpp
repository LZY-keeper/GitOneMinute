#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>


std::mutex mutex_;
std::condition_variable condition_;
bool data_ready_ = false;//这个是标志位
int shared_data_ = 0;

//如果是队列  标志位应该判断  队列是否为空，队列是不是满的

// 生产者函数
void producer() {
    std::cout << "Producer: starting work..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::lock_guard<std::mutex> lock(mutex_);
        shared_data_ = 42;
        data_ready_ = true;
        std::cout << "Producer: data prepared, notifying..." << std::endl;
    }

    condition_.notify_one();  // 通知一个等待的线程
}

//消费者函数
void consumer() {
    std::cout << "Consumer: waiting for data..." << std::endl;

    std::unique_lock<std::mutex> lock(mutex_);

    // 等待条件满足
    condition_.wait(lock, []() {
        std::cout << "Consumer: checking condition..." << std::endl;
        return data_ready_; //一般来说这里的判断不该是标志位，而是判断图像是否为空
        });

    // 条件满足，处理数据
    std::cout << "Consumer: received data = " << shared_data_ << std::endl;
}


int main()
{

    //
    std::thread consumer_thread(consumer);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::thread producer_thread(producer);

    producer_thread.join();
    consumer_thread.join();

    std::cout << "Hello World!\n";
}


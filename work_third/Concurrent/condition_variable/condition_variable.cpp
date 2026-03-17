#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable condition_;  //这只是一个条件变量
bool ready = false; //标志位，数据是否准备好了

void waiting_thread() {
    std::unique_lock<std::mutex> lock(mtx);  // 1. 获取锁

    // condition_.wait() 的内部操作：
    //函数，等待为true，卡在这里等待
    condition_.wait(lock, [] { return ready; });  //[] { return ready; }一定得带这样的条件，不然会虚假唤醒
    // 等效于：
    // while (![] { return ready; }) {
    //     lock.unlock();       // 2. 暂时释放锁，// ⚠️ lock_guard 没有这个方法
    //     // 进入等待队列（操作系统级）
    //	   internal_wait();  //其实是卡在这里
    //     lock.lock();         // 3. 被唤醒后重新获取锁// ⚠️ lock_guard 没有这个方法
    // }

    //需要执行的操作
    std::cout << "waiting_thread! Thread ID: "
        << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void trigger_thread() {
    //这里能不能获取成功？  
    // 其实可以，因为wait的用法可以使这里获取锁的操作不受waiting_thread里获取锁操作的影响
    std::unique_lock<std::mutex> lock(mtx);  // 1. 获取锁
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //等待这里为true
    ready = true; //数据修改
    // 调用notify_all唤醒internal_wait();  //其实是卡在这里
    condition_.notify_all();
    std::cout << "trigger_thread! Thread ID: "
        << std::this_thread::get_id() << std::endl;
}

int main()
{
    std::thread waitingthread(waiting_thread);//等待线程
    std::thread triggerthread(trigger_thread);//触发现场

    waitingthread.join();
    triggerthread.join();
    std::cout << "Hello World!\n";
}





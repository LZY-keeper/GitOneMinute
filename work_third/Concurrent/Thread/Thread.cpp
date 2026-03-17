#include <iostream>
#include<thread>


// 简单的线程函数
void hello_thread() {
    std::cout << "Hello from thread! Thread ID: "
        << std::this_thread::get_id() << std::endl;
    
}

// 带循环的线程函数
void worker_thread(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::cout << "Worker iteration: " << i
            << " Thread ID: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

class ThreadManager {
public:
    // 1. 使用普通函数
    static void static_function(int id) {
        std::cout << "Static function, thread " << id << std::endl;
    }
    
    // 2. 使用成员函数
    //默认有一个隐性参数：this指针--即对象指针
    void member_function(int id) {
        std::cout << "Member function, thread " << id
            << " this: " << this << std::endl;
    }

};

// 3. 使用函数对象
struct Functor {
    //operator 操作符重载，重载的是（）这个操作符
    void operator()(int id) const {
        std::cout << "Functor, thread " << id << std::endl;
    }
};


int main()
{
    //Functor f;
    //f(5);
    ThreadManager manager;

    // 方式1: 静态函数
    std::thread t1(ThreadManager::static_function,1);

    // 方式2: 成员函数
    std::thread t2(&ThreadManager::member_function, &manager,2);// &manager == this指针参数，调一个引用

    // 方式3: lambda表达式--随地随用
    std::thread t3([](int id) {
        std::cout << "Inline lambda, thread " << id << std::endl;
        }, 3);

    // 方式4: 函数对象
    std::thread t4(Functor(), 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "hello!\n";
    
}




//int main()
//{
//    //线程是什么时候运行的？ --构造完毕后。即刻就是就绪状态，马上进入运行状态
//    std::thread t2(worker_thread,5);
//
//    //std::thread t1(hello_thread);//{}--空的构造，不可执行的线程
//    //bool ret = t1.joinable();
//    //这里异常是指：主线程先退出了，没有理会子线程
//
//   // t1.join();//这个可以阻塞当前线程，直到线程内部实现结束（也就是说主线程会等子线程执行完毕才会结束)
//
//    t2.detach(); //主子线程分离，让系统去管理，异步。这里执行后控制台打印的是主线程，只要主线程结束了，就结束打印
//
//    std::cout << "hello!\n";
//    std::this_thread::sleep_for(std::chrono::milliseconds(200));//打印完后等待200ms
//}


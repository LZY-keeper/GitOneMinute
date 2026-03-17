#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

//线程池等于公司   有多少个人-》子线程
class ThreadPool {
public:
    //构造函数，传入size_t，是明确这个线程池，里面一共有几个线程在工作，
    //一般看你CPU核数
    ThreadPool(size_t);
    //添加任务，线程池的工作原理就是，我先开假设10个线程，然后这是个线程就
    //跑起来了，就是空跑，然后加一个任务呢，其中一个线程就把这个任务拿去执行
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    //在线程池里面呢，有两个关键变量，就是workers和tasks，我们构造的时候，
    //构造的10个空线程，就存在workers里，添加的任务，则在tasks里
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;

    //所以呢，我们有工作者，和任务两个概念，就和我们的生产者和消费者类似。

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    : stop(false)
{
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(//thread对象：隐形构造：thread
            //每一个worker在做什么呢，就是在做下面的逻辑：
            [this]
            {
                for (;;)// == while (true) ：代表打工人永无止境的打工
                {
                    std::function<void()> task;

                    {
                        //这里就是经典的条件变量和unique_lock配合使用
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        //条件变量在等什么呢？等tasks任务队列不为空，如果是空的，
                        //线程执行者就会一直wait等待下去
                        //如果不为空，workers工作者就会取任务去执行，执行完这个任务，
                        //就又会继续等待，重复上面步骤
                        this->condition.wait(lock,
                            [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();//执行任务
                }
            }
        );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        //添加任务呢，其实很简单，就是将任务添加到tasks队列中，并notify_one，
        //唤醒一个工作者去工作
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}

#endif


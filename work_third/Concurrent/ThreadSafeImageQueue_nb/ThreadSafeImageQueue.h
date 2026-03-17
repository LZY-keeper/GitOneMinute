#pragma once
/**
 * @file ThreadSafeImageQueue.h
 * @brief 线程安全的图像队列完整实现
 */
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>
#include <atomic>
#include <stdexcept>

 /**
 * @brief 线程安全的图像队列
 * @tparam ImageType 图像类型，默认为cv::Mat
 */
 //template<typename ImageType = cv::Mat>
template<typename ImageType>
class ThreadSafeImageQueue {
public:
    /**
    * @brief 构造函数
    * @param max_size 队列最大容量，0表示无限制
    * @param shutdown_on_destructor 析构时是否自动关闭
    */
    explicit ThreadSafeImageQueue(size_t max_size = 10,
        bool shutdown_on_destructor = true)
        : max_size_(max_size)
        , shutdown_flag_(false)
        , shutdown_on_destructor_(shutdown_on_destructor) {
        if (max_size == 0) {
            max_size_ = std::numeric_limits<size_t>::max();
        }
    }

    /**
    * @brief 析构函数
    */
    ~ThreadSafeImageQueue() {
        if (shutdown_on_destructor_) {
            shutdown();
        }
    }

    // 禁用拷贝和赋值
    ThreadSafeImageQueue(const ThreadSafeImageQueue&) = delete;
    ThreadSafeImageQueue& operator=(const ThreadSafeImageQueue&) = delete;

    /**
    * @brief 移动构造函数
    */
    ThreadSafeImageQueue(ThreadSafeImageQueue&& other) noexcept {
        std::lock_guard<std::mutex> lock(other.mutex_);
        queue_ = std::move(other.queue_);
        max_size_ = other.max_size_;
        shutdown_flag_ = other.shutdown_flag_.load();
        shutdown_on_destructor_ = other.shutdown_on_destructor_;
    }

    /**
    * @brief 移动赋值运算符
    */
    ThreadSafeImageQueue& operator=(ThreadSafeImageQueue&& other) noexcept {
        if (this != &other) {
            std::unique_lock<std::mutex> lock1(mutex_, std::defer_lock);
            std::unique_lock<std::mutex> lock2(other.mutex_, std::defer_lock);
            std::lock(lock1, lock2);

            queue_ = std::move(other.queue_);
            max_size_ = other.max_size_;
            shutdown_flag_ = other.shutdown_flag_.load();
            shutdown_on_destructor_ = other.shutdown_on_destructor_;

            // 唤醒所有等待的线程
            not_empty_cond_.notify_all();
            not_full_cond_.notify_all();
        }
        return *this;
    }

    /**
    * @brief 推入图像（阻塞）
    * @param img 待推入的图像
    * @param timeout_ms 超时时间（毫秒），0表示一直等待
    * @return 是否成功推入
    */


    bool push(const ImageType& img, int timeout_ms = 0) {
        std::unique_lock<std::mutex> lock(mutex_);

        // 等待队列非满
        if (!wait_for_not_full(lock, timeout_ms)) {
            return false;
        }

        if (shutdown_flag_) {
            return false;
        }

        // 推入图像
        bool was_empty = queue_.empty();
        queue_.push(img);

        lock.unlock();

        // 通知消费者（如果队列从空变为非空）
        if (was_empty) {
            not_empty_cond_.notify_one();
        }

        return true;
    }

    //生产者调用push，如果队列是满的 就会让非满条件not_full_cond等待
    //成功推入数据就会调用非空条件not_empty_cond进行唤醒

    //消费者调用pop，如果队列是空的，就会让非空条件not_empty_cond等待 
    //成功取出数据，就会强调非满条件not_full_cond进行唤醒触发


    /**
    * @brief 取出图像（阻塞）
    * @param img 接收图像的引用
    * @param timeout_ms 超时时间（毫秒）
    * @return 是否成功取出
    */
    bool pop(ImageType& img, int timeout_ms = 0) {
        std::unique_lock<std::mutex> lock(mutex_);

        // 等待队列非空
        if (!wait_for_not_empty(lock, timeout_ms)) {
            return false;
        }

        if (queue_.empty() || shutdown_flag_) {
            return false;
        }

        // 取出图像
        bool was_full = (queue_.size() == max_size_);

        // 使用交换避免拷贝（如果ImageType支持移动）
        if constexpr (std::is_move_assignable_v<ImageType>) {
            img = std::move(queue_.front());
        }
        else {
            img = queue_.front();
        }
        queue_.pop();

        lock.unlock();

        // 通知生产者（如果队列从满变为非满）
        if (was_full) {
            not_full_cond_.notify_one();
        }

        return true;
    }

    /**
    * @brief 清空队列
    */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);

        // 清空队列
        while (!queue_.empty()) {
            queue_.pop();
        }

        // 通知所有等待的生产者
        not_full_cond_.notify_all();
    }

    /**
    * @brief 获取队列当前大小
    * @return 队列大小
    */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    /**
    * @brief 队列是否为空
    * @return 是否为空
    */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    /**
    * @brief 队列是否已满
    * @return 是否已满
    */
    bool full() const {
        if (max_size_ == 0) return false;

        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size() >= max_size_;
    }

    /**
    * @brief 获取队列最大容量
    * @return 最大容量
    */
    size_t max_size() const {
        return max_size_;
    }

    /**
    * @brief 设置队列容量
    * @param max_size 新的最大容量
    */
    void set_max_size(size_t max_size) {
        std::lock_guard<std::mutex> lock(mutex_);

        max_size_ = (max_size == 0) ? std::numeric_limits<size_t>::max() : max_size;

        // 如果新容量比当前队列大小大，通知等待的生产者
        if (queue_.size() < max_size_) {
            not_full_cond_.notify_all();
        }
    }

    /**
    * @brief 关闭队列
    * @note 关闭后所有阻塞操作将立即返回false
    */
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_flag_ = true;
        }

        // 唤醒所有等待的线程
        not_empty_cond_.notify_all();
        not_full_cond_.notify_all();
    }

private:
    mutable std::mutex mutex_;                         // 互斥锁
    std::condition_variable not_empty_cond_;          // 非空条件变量
    std::condition_variable not_full_cond_;           // 非满条件变量

    std::queue<ImageType> queue_;                     // 底层队列
    size_t max_size_;                                 // 队列最大容量
    std::atomic<bool> shutdown_flag_;                 // 关闭标志
    bool shutdown_on_destructor_;                     // 析构时自动关闭

    /**
    * @brief 等待队列非满（内部辅助函数）
    * @param lock 已锁定的互斥锁
    * @param timeout_ms 超时时间
    * @return 是否等待成功
    */
    bool wait_for_not_full(std::unique_lock<std::mutex>& lock, int timeout_ms) {
        if (shutdown_flag_) {
            return false;
        }
        //标志队列还有空间，非满
        if (max_size_ == 0 || queue_.size() < max_size_) {
            return true;
        }

        if (timeout_ms <= 0) {
            // 无限等待
            not_full_cond_.wait(lock, [this]() {
                return queue_.size() < max_size_ || shutdown_flag_;
                });
            return !shutdown_flag_;
        }
        else {
            // 带超时的等待
            auto timeout_reached = not_full_cond_.wait_for(lock,
                std::chrono::milliseconds(timeout_ms), [this]() {
                    return queue_.size() < max_size_ || shutdown_flag_;
                });

            return timeout_reached && !shutdown_flag_;
        }
    }

    /**
        * @brief 等待队列非空（内部辅助函数）
        * @param lock 已锁定的互斥锁
        * @param timeout_ms 超时时间
        * @return 是否等待成功
        */
    bool wait_for_not_empty(std::unique_lock<std::mutex>& lock, int timeout_ms) {
        if (shutdown_flag_) {
            return false;
        }

        if (!queue_.empty()) {
            return true;
        }

        if (timeout_ms <= 0) {
            // 无限等待
            not_empty_cond_.wait(lock, [this]() {
                return !queue_.empty() || shutdown_flag_;
                });
            return !shutdown_flag_ && !queue_.empty();
        }
        else {
            // 带超时的等待
            auto timeout_reached = not_empty_cond_.wait_for(lock,
                std::chrono::milliseconds(timeout_ms), [this]() {
                    return !queue_.empty() || shutdown_flag_;
                });

            return timeout_reached && !shutdown_flag_ && !queue_.empty();
        }
    }
};


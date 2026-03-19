#include<iostream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <algorithm>

/**
 * @brief 事件类型枚举
 */
enum class EventType {
    NONE = 0,           ///< 无事件
    DATA_CHANGED = 1,   ///< 数据变化事件
    USER_LOGIN = 2,     ///< 用户登录事件
    SYSTEM_ERROR = 3,   ///< 系统错误事件
    UI_UPDATE = 4,      ///< 界面更新事件
    NETWORK_CHANGED = 5 ///< 网络状态变化事件
};

/**
 * @brief 抽象监听者基类
 * 所有具体监听者必须继承此类并实现onEvent方法
 */
class Listener {
public:
    virtual ~Listener() = default;

    /**
     * @brief 事件响应函数
     * @param event 事件类型
     */
    virtual void onRespond(EventType event) = 0;//纯虚函数--要想用Listener，必须实现onRespond

protected:
    Listener() = default;
    Listener(const Listener&) = delete;
    Listener& operator=(const Listener&) = delete;
};


/**
 * @brief 观察者单例类
 * 用于管理监听者并分发事件
 */
class Observer {
public:
    /**
     * @brief 获取单例实例
     * @return Observer& 单例引用
     */
    /*static Observer& getInstance() {
        static std::once_flag once_flag;
        std::call_once(once_flag, []() {
            instance_ = new Observer();
            });
        return *instance_;        
    }*/
    //在此改为标准的Meyers 单例    另外此单例不允许外部构造，须在private里创建
    static Observer& getInstance() {
        static Observer instance;   // 线程安全的局部静态对象
        return instance;
    }

    /**
     * @brief 注册监听者
     * @param event 事件类型
     * @param listener 监听者指针
     */
    void registerListener(EventType event, Listener* listener) {
        if (listener == nullptr) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        // 检查是否已经注册
        auto it = listeners_.find(event);
        if (it != listeners_.end()) {
            auto& listeners = it->second;
            if (std::find(listeners.begin(), listeners.end(), listener) != listeners.end()) {
                return; // 已注册
            }
        }

        listeners_[event].push_back(listener);
    }

    /**
     * @brief 注销监听者
     * @param event 事件类型
     * @param listener 监听者指针
     */
    void unregisterListener(EventType event, Listener* listener) {
        if (listener == nullptr) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        auto it = listeners_.find(event);
        if (it != listeners_.end()) {
            auto& listeners = it->second;
            auto pos = std::remove(listeners.begin(), listeners.end(), listener);
            if (pos != listeners.end()) {
                listeners.erase(pos, listeners.end());

                // 如果该事件的监听者列表为空，删除整个条目
                if (listeners.empty()) {
                    listeners_.erase(it);
                }
            }
        }
    }

    /**
     * @brief 通知所有监听者
     * @param event 事件类型
     */
    void notify(EventType event) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = listeners_.find(event);
        if (it != listeners_.end()) {
            const auto& listeners = it->second;
            // 通知所有监听者
            for (auto* listener : listeners) {
                if (listener != nullptr) {
                    listener->onRespond(event);
                }
            }
        }
    }

    /**
     * @brief 清空所有监听者
     */
    void clearAllListeners() {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners_.clear();
    }

private:
    /**
     * @brief 私有构造函数
     */
    Observer() = default;

    /**
     * @brief 析构函数
     */
    ~Observer() = default;

    /**
     * @brief 禁止拷贝构造
     */
    Observer(const Observer&) = delete;

    /**
     * @brief 禁止赋值操作
     */
    Observer& operator=(const Observer&) = delete;

    /**
     * @brief 事件监听者映射表
     */
    std::unordered_map<EventType, std::vector<Listener*>> listeners_; //第一个参数是事件，第二个参数是监听者关注事件后的列表

    /**
     * @brief 互斥锁，保证线程安全
     */
    mutable std::mutex mutex_;

    /**
     * @brief 单例实例指针
     */
    static Observer* instance_;
};

// 初始化静态成员
Observer* Observer::instance_ = nullptr;

//设置面板
class SettingsPanel :  public Listener {
public:
    SettingsPanel() {//在构造函数内部注册
        Observer::getInstance().registerListener(EventType::UI_UPDATE, this);
        Observer::getInstance().registerListener(EventType::NETWORK_CHANGED, this);
    };
    //还有很多其它的接口处理
    //。。。

    void onRespond(EventType event) override {
        switch (event) {
        case EventType::UI_UPDATE:
            std::cout << "SettingsPanel: Handling UI update event" << std::endl;
            refreshSettings();
            break;
        case EventType::NETWORK_CHANGED:
            std::cout << "SettingsPanel: Handling network changed event" << std::endl;
            updateNetworkStatus();
            break;
        default:
            // 不处理其他事件
            break;
        }
    }

private:
    void refreshSettings() {
        // 实际的设置刷新逻辑
    }

    void updateNetworkStatus() {
        // 实际的网络状态更新逻辑
    }
};

int main()
{
    // 获取观察者单例
    Observer& observer = Observer::getInstance();

    // 创建具体面板，他继承了监听者
    SettingsPanel settingsPanel;

    // 注册监听者    这一块在构造函数里注册，这里就不用注册了
   /* observer.registerListener(EventType::UI_UPDATE, &settingsPanel);
    observer.registerListener(EventType::NETWORK_CHANGED, &settingsPanel);*/


    // 发送通知
    std::cout << "=== Sending DATA_CHANGED event ===" << std::endl;
    observer.notify(EventType::DATA_CHANGED);

    std::cout << "\n=== Sending UI_UPDATE event ===" << std::endl;
    observer.notify(EventType::UI_UPDATE);

    std::cout << "\n=== Sending NETWORK_CHANGED event ===" << std::endl;
    observer.notify(EventType::NETWORK_CHANGED);


    std::cout << "Hello World!\n";
}


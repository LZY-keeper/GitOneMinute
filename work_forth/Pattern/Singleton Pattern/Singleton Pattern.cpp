#include <iostream>
#include <string>
#include <map>
#include<mutex>

//单例模式的变种--懒汉式单例（注意多线程问题）
class LazySingleton {
private:
    static LazySingleton* instance;
    static std::mutex mutex;

    LazySingleton() = default;

public:
    static LazySingleton* getInstance() {
        if (instance == nullptr) {  // 第一次检查，避免不必要的锁
            std::lock_guard<std::mutex> lock(mutex);
            if (instance == nullptr) {  // 第二次检查，确保线程安全,防止其它线程new了一下而导致的内存泄漏
                instance = new LazySingleton();
            }
        }
        return instance;
    }
};

//单例模式的变种--饿汉式单例，不推荐
class EagerSingleton {
private:
    static EagerSingleton instance;

    EagerSingleton() = default;

public:
    static EagerSingleton& getInstance() {
        return instance;
    }
};

// 静态成员初始化
EagerSingleton EagerSingleton::instance;


// Meyers' Singleton（现代C++推荐）
class MeyersSingleton {
private:
    MeyersSingleton() = default;

public:
    static MeyersSingleton& getInstance() {
        static MeyersSingleton instance;//都是静态，全局只有一份
        return instance;
    }

    //// 业务方法示例
    //void processImage(const cv::Mat& image) {
    //    // 图像处理逻辑
    //}
};



//单例模式

class camera {};//简单定义一下类对象

class CameraManager {
public:
    // 全局访问点
    static CameraManager* getInstance() {
        if (instance == nullptr) {
            instance = new CameraManager();//唯一的类对象，这就是为什么设置static
        }
        return instance;
    }

    void init() {
        //想办法调用接口去枚举相机
        //_cameraIdMap
    }

    // 业务方法:其实有很多个业务，这里只展示以一个
    void openCamera(std::string cameraId) {
        auto it = _cameraIdMap.find(cameraId);
        if (it != _cameraIdMap.end()) {
            // 找到了，使用 it->second 访问 camera 对象
            camera& cam = it->second;
            // 使用 cam...
            //调用相机的打开接口open
            std::cout << cameraId << "Camera open" << std::endl;
        }
        else {
            // 没有找到
            std::cout << "Camera not found!" << std::endl;
        }
    }



private:
    static CameraManager* instance; //static 无论类对象创建多少个，内存中静态指针都只会有一个
                                    //例：CameraManager a；CameraManager b；->同一个

    std::map<std::string, camera> _cameraIdMap;  //存相机序列号->相机对象（映射）
                                                 //camera可以不用在意，一般不会存类对象，而是智能指针

    // 私有构造函数，防止外部实例化
    CameraManager() {}

    // 防止拷贝和赋值
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;
};

// 静态成员初始化，一般放在类的外部5
CameraManager* CameraManager::instance = nullptr;

//怎么初始化的？比如
//int a = 5; //其中CameraManager::instance相当于a， CameraManager*相当于int



int main()
{
    std::string id = "555";
    CameraManager::getInstance()->openCamera(id);
    std::cout << "Hello World!\n";
}


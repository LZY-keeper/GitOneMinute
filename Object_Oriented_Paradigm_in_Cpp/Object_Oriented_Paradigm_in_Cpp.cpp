#include<iostream>

// 基础相机类 - 定义所有相机的共性
class BaseCamera {
protected:
    std::string name_;
    bool connected_;

public:
    BaseCamera(const std::string& name) : name_(name), connected_(false) {}

    virtual bool connect() {  //接口
        std::cout << "Base camera connecting..." << std::endl;
        connected_ = true;
        return true;
    }

  //  virtual bool disconnect() = 0; //纯虚接口,必须被子类复写实现

    virtual void capture() {  //接口
        if (!connected_) throw std::runtime_error("Not connected");
       // return cv::Mat();
        return;
    }

    virtual ~BaseCamera() = default;
};

// USB相机 - 继承基础功能，添加特有功能
class USBCamera : public BaseCamera {
private:
    int device_id_;

public:
    USBCamera(const std::string& name) : BaseCamera(name)
       {
    }

    // 重写连接方法，提供USB相机的具体实现
    bool connect() override {
        std::cout << "USB camera " << device_id_ << " connecting..." << std::endl;
        // USB特有的连接逻辑
        connected_ = true;
        return true;
    }

     void capture()override {  //接口
        if (!connected_) throw std::runtime_error("Not connected");
        // return cv::Mat();
        std::cout << "USBCamera capture One Frame";
        return;
    }

    // 添加USB相机特有功能
    void setUSBSpeed(int speed) {
        std::cout << "Setting USB speed to " << speed << std::endl;
    }
};

// 网络相机 - 同样的接口，不同的实现
class NetworkCamera : public BaseCamera {
private:
    std::string ip_address_;

public:
    NetworkCamera(const std::string& name)
        : BaseCamera(name) {
    }

    bool connect() override {
        std::cout << "Network camera " << ip_address_ << " connecting..." << std::endl;
        // 网络连接逻辑
        connected_ = true;
        return true;
    }

    void capture()override {  //接口
        if (!connected_) throw std::runtime_error("Not connected");
        // return cv::Mat();
        std::cout << "NetworkCamera capture One Frame";
        return;
    }

};

// 好的耦合：通过接口依赖
class VisionSystem {
    std::unique_ptr<BaseCamera> _camera;  // 只依赖接口

public:
    VisionSystem(std::unique_ptr<BaseCamera> detector)
        : _camera(std::move(detector)) {
    }
    void run() {

        // 多态的实现，只依赖一个抽象的接口类，就实现了不同的子类行为
        _camera->connect();
        _camera->capture();
    }
};


int main() {


  //  BaseCamera camera("hikcamera");  错误，加了纯虚接口，无法被实例化
    
   

    std::unique_ptr<BaseCamera>camera;
    int cameraType;
    std::cout << "选择相机类型（1=USB,2=Network):";
    std::cin >> cameraType;
    if (cameraType == 1) {
        camera = std::make_unique<USBCamera>("usbCam");
    }
    else {
        camera = std::make_unique<NetworkCamera>("netWorkCam");
    }

    // 我们要创建一个VisionSystem
    VisionSystem system(std::move(camera));
    system.run();

    return 0;
}
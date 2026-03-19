#pragma once
#include"aloginterface.h"
#include <functional>
#include <mutex>
#include <vector>


//// 简单工厂
//class AlgorithmFactory {
//public:
//    // 获取单例实例
//    static AlgorithmFactory& getInstance() {
//        static AlgorithmFactory instance;
//        return instance;
//    }
//    std::unique_ptr<AlgorithmInterface> createAlgorithm(const std::string& name) {
//        if (name == "CannyEdgeDetector") return std::make_unique<CannyEdgeDetector>();
//        if (name == "SIFTFeatureExtractor") return std::make_unique<SIFTFeatureExtractor>();
//        if (name == "HoughLineDetector") return std::make_unique<HoughLineDetector>();
//        if (name == "ORBFeatureExtractor") return std::make_unique<ORBFeatureExtractor>();
//        return nullptr;
//    }
//
//private:
//    // 私有构造函数
//    AlgorithmFactory() {}
//};



// 单例算法工厂  --给简单工厂增加注册接口
class AlgorithmFactory {
public:
    // 自动注册宏（可选，简化注册代码）
#define REGISTER_ALGORITHM(AlgorithmClass, algorithmName) \
        namespace { \
            struct AlgorithmClass##Registrar { \
                AlgorithmClass##Registrar() { \
                    AlgorithmFactory::getInstance().registerAlgorithm<AlgorithmClass>(algorithmName); \
                } \
            } AlgorithmClass##RegistrarInstance; \
        }

    // 算法创建函数类型
    using AlgorithmCreator = std::function<std::unique_ptr<AlgorithmInterface>()>;
    // 获取单例实例
    static AlgorithmFactory& getInstance() {
        static AlgorithmFactory instance;
        return instance;
    }

    // 通用注册方法
    void registerAlgorithm(const std::string& name, AlgorithmCreator creator) {
        std::lock_guard<std::mutex> lock(mutex_);

        if (registry_.find(name) != registry_.end()) {
            std::cerr << "Warning: Algorithm '" << name << "' is already registered.\n";
        }

        registry_[name] = std::move(creator);
        std::cout << "Algorithm '" << name << "' registered successfully.\n";
    }

    // 模板注册方法：直接通过算法类型注册
    template<typename AlgorithmType>
    void registerAlgorithm(const std::string& name) {
        registerAlgorithm(name, []() -> std::unique_ptr<AlgorithmInterface> {
            return std::make_unique<AlgorithmType>();
            });
    }

    // 创建算法实例
    std::unique_ptr<AlgorithmInterface> createAlgorithm(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = registry_.find(name);
        if (it != registry_.end()) {
            return it->second();  // 调用创建函数
        }

        std::cerr << "Error: Algorithm '" << name << "' not found.\n";
        return nullptr;
    }

    // 打印所有可用算法
    void printAvailableAlgorithms() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::string> names;
        names.reserve(registry_.size());

        for (const auto& pair : registry_) {
            names.push_back(pair.first);
        }

        std::cout << "\n=== Available Algorithms (" << names.size() << ") ===\n";
        for (size_t i = 0; i < names.size(); ++i) {
            std::cout << i + 1 << ". " << names[i] << "\n";
        }
        std::cout << "===========================\n\n";
    }
private:
    // 私有构造函数
    AlgorithmFactory() {
        // 可以在这里注册内置算法
        // registerBuiltInAlgorithms();
    }

    // 注册表：算法名称 -> 创建函数
    std::map<std::string, AlgorithmCreator> registry_;

    // 互斥锁保证线程安全
    mutable std::mutex mutex_;

    // 删除拷贝构造函数和赋值运算符
    AlgorithmFactory(const AlgorithmFactory&) = delete;
    AlgorithmFactory& operator=(const AlgorithmFactory&) = delete;
};


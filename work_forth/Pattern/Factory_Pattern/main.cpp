#include <iostream>
#include"aloginterface.h"
#include"algorithmFactory.h"


// 传统方式的问题 - 紧耦合，所以我们需要做一个算法工厂
//这里要做成一个单例
AlgorithmInterface* creatAlgo(const std::string &algorithmType) {
    if (algorithmType == "CannyEdge") {
        return new CannyEdgeDetector();
    }
    else if (algorithmType == "SIFTFeature") {
        return new SIFTFeatureExtractor();
    }
    else if (algorithmType == "HoughLine") {
        return new HoughLineDetector();
    }
    // 每增加新算法都需要修改此处代码
}

// 使用REGISTER_ALGORITHM宏进行自动注册
REGISTER_ALGORITHM(CannyEdgeDetector, "CannyEdgeDetector")
REGISTER_ALGORITHM(SIFTFeatureExtractor, "SIFTFeatureExtractor")
REGISTER_ALGORITHM(HoughLineDetector, "HoughLineDetector")
REGISTER_ALGORITHM(ORBFeatureExtractor, "ORBFeatureExtractor")

// 这个就是以上宏展开的样子--可以去看宏定义
//namespace {
//    struct CannyEdgeDetectorRegistrar {
//        CannyEdgeDetectorRegistrar() {
//            AlgorithmFactory::getInstance().registerAlgorithm<CannyEdgeDetector>("CannyEdgeDetector");
//        }
//    } CannyEdgeDetectorRegistrarInstance;
//}
//
//namespace {
//    struct SIFTFeatureExtractorRegistrar {
//        SIFTFeatureExtractorRegistrar() {
//            AlgorithmFactory::getInstance().registerAlgorithm<SIFTFeatureExtractor>("SIFTFeatureExtractor");
//        }
//    } SIFTFeatureExtractorRegistrarInstance;
//}

//可当成你代码里的某个函数内
int main()
{
    // 手动注册一些算法--和宏的自动没什么区别
    auto& factory = AlgorithmFactory::getInstance();

    //// 方法1：使用模板方法注册
    //factory.registerAlgorithm<CannyEdgeDetector>("Canny");

    //// 方法2：使用通用注册方法
    //factory.registerAlgorithm("HoughLineDetector",
    //    []() -> std::unique_ptr<HoughLineDetector> {
    //        auto algo = std::make_unique<HoughLineDetector>();
    //        // 自定义创建逻辑
    //        return algo;
    //    });

    // 演示工厂使用
    factory.printAvailableAlgorithms();// 打印可用算法

    // 创建算法实例
    std::cout << "\nCreating algorithms:\n";

    // 使用宏注册的算法
    auto canny1 = factory.createAlgorithm("CannyEdgeDetector");
    if (canny1) {
        std::cout << "Created: " << canny1->getName() << "\n";
        canny1->process(cv::Mat());
    }


    std::cout << "\nHello World!";

   

}


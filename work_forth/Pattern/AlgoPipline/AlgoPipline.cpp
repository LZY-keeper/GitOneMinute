#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <sstream>
#include "AlogInterface.h"
#include "parseAlgoJson.h"
#include "AlgorithmPipeline.h"

// 使用REGISTER_ALGORITHM宏进行自动注册
REGISTER_ALGORITHM(CannyEdgeDetector, "CannyEdgeDetector")
REGISTER_ALGORITHM(SIFTFeatureExtractor, "SIFTFeatureExtractor")
REGISTER_ALGORITHM(HoughLineDetector, "HoughLineDetector")
REGISTER_ALGORITHM(ORBFeatureExtractor, "ORBFeatureExtractor")

int main()
{
    std::cout << "Hello World!\n";

    std::ifstream file("F:/text/GitOneMinute/work_forth/pipline1.json");
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file.");
    }
    // 使用字符串流读取整个文件
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string strJson = buffer.str();
    std::cout << strJson << std::endl;

    auto algos = parseAlgoJson::instance().parseFromJson(strJson);
    auto pipeline = AlgorithmPipeline("pipeline", std::move(algos));
    cv::Mat src, dst;
    pipeline.process(src, dst);
    return 0;
}
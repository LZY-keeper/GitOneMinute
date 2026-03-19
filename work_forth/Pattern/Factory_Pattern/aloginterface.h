#pragma once
#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <memory>

//我们还没讲到opencv，暂时使用假的替代
namespace cv {
    class Mat {};
}

// 参数值类型定义
using ParameterValue = std::variant<int, double, bool, std::string>;

// 算法参数映射类型
using ParameterMap = std::map<std::string, ParameterValue>;//map里是 参数名称——>参数值

// 算法统一接口
class AlgorithmInterface {
public:
    virtual ~AlgorithmInterface() = default;

    virtual cv::Mat process(const cv::Mat& input) = 0;
    virtual ParameterMap getParameters() const = 0;//获取参数
    virtual void setParameters(const ParameterMap& params) = 0;//设置参数
    virtual std::string getName() const = 0;//类型名称：假如是 CannyEdgeDetector边缘检测
};

// 可选：使用函数模板进行类型安全的参数设置
template<typename T>
bool tryGetParameter(const ParameterMap& params, const std::string& key, T& value) {
    auto it = params.find(key);
    if (it != params.end()) {
        try {
            value = std::get<T>(it->second);
            return true;
        }
        catch (const std::bad_variant_access&) {
            return false;
        }
    }
    return false;
}

// 具体算法实现：Canny边缘检测
class CannyEdgeDetector : public AlgorithmInterface {
private:
    double threshold1 = 50.0;
    double threshold2 = 150.0;
    int apertureSize = 3;
    bool L2gradient = false;

public:
    cv::Mat process(const cv::Mat& input) override {
        cv::Mat edges;
        //cv::Canny(input, edges, threshold1, threshold2, apertureSize, L2gradient);
        std::cout << "CannyEdgeDetector process!";
        return edges;
    }

    ParameterMap getParameters() const override {
        ParameterMap params;
        params["threshold1"] = threshold1;
        params["threshold2"] = threshold2;
        params["apertureSize"] = apertureSize;
        params["L2gradient"] = L2gradient;
        return params;
    }

    void setParameters(const ParameterMap& params) override {
        for (const auto& [key, value] : params) {
            if (key == "threshold1") {
                threshold1 = std::get<double>(value);
            }
            else if (key == "threshold2") {
                threshold2 = std::get<double>(value);
            }
            else if (key == "apertureSize") {
                apertureSize = std::get<int>(value);
            }
            else if (key == "L2gradient") {
                L2gradient = std::get<bool>(value);
            }
        }
        //tryGetParameter(params, "threshold1", threshold1);
        //tryGetParameter(params, "threshold2", threshold2);
        //tryGetParameter(params, "apertureSize", apertureSize);
        //tryGetParameter(params, "L2gradient", L2gradient);  
        //如果设置了大量的参数，就不可能实现多个for循环，所以可以用以上模板代替实现，更简洁
    }

    std::string getName() const override {
        return "CannyEdgeDetector";
    }
};

class SIFTFeatureExtractor : public AlgorithmInterface {
public:
    cv::Mat process(const cv::Mat& input) override { return cv::Mat{}; }
    ParameterMap getParameters() const override { return ParameterMap{}; }
    void setParameters(const ParameterMap& params) override {}
    std::string getName() const override { return "SIFTFeatureExtractor"; }
};

class HoughLineDetector : public AlgorithmInterface {
public:
    cv::Mat process(const cv::Mat& input) override { return cv::Mat{}; }
    ParameterMap getParameters() const override { return ParameterMap{}; }
    void setParameters(const ParameterMap& params) override {}
    std::string getName() const override { return "HoughLineDetector"; }
};

class ORBFeatureExtractor : public AlgorithmInterface {
public:
    cv::Mat process(const cv::Mat& input) override { return cv::Mat{}; }
    ParameterMap getParameters() const override { return ParameterMap{}; }
    void setParameters(const ParameterMap& params) override {}
    std::string getName() const override { return "ORBFeatureExtractor"; }
};

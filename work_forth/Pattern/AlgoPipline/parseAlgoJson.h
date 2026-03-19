#pragma once
#include"aloginterface.h"
#include"algorithmFactory.h"
#include<vector>
#include"json.hpp"


using Algos = std::vector<std::unique_ptr<AlgorithmInterface>>;
class parseAlgoJson {
private:
    parseAlgoJson() = default;
public:
    static parseAlgoJson& instance() {
        static parseAlgoJson instance;
        return instance;
    }
    //反序列化
    Algos parseFromJson(const std::string& jsonStr);
    //序列化
    std::string parseToJson(const Algos& algos);
};


//以下这2个实现一般放在cpp里，这里是为了图省事
//反序列化如何实现
Algos parseAlgoJson::parseFromJson(const std::string& jsonStr) {
    Algos algorithms;

    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        
        /*"pipeline_name": "边缘检测与直线检测管道",
        "version": "1.0",
        "description" : "用于工业零件检测的视觉算法管道",*/  //就是实现这些。。。
        auto name = j["pipeline_name"].get<std::string>();
        std::cout << name << std::endl;
        
        auto pipeline = j["processing_pipeline"];

        for (const auto& algoConfig : pipeline) {
            std::string algorithmName = algoConfig["algorithm"];

            //创建实例
            auto algorithm = AlgorithmFactory::getInstance().createAlgorithm(algorithmName);
            if (!algorithm) {
                throw std::runtime_error("Unknown algorithm: " + algorithmName);
            }

            // 通用的参数解析
            ParameterMap params;
            auto jsonParams = algoConfig["parameters"];

            for (auto it = jsonParams.begin(); it != jsonParams.end(); ++it) {
                std::string key = it.key();
                nlohmann::json value = it.value();

                // 根据JSON值的类型转换为ParameterValue
                if (value.is_number_integer()) {
                    params[key] = value.get<int>();
                }
                else if (value.is_number_float()) {
                    params[key] = value.get<double>();
                }
                else if (value.is_boolean()) {
                    params[key] = value.get<bool>();
                }
                else if (value.is_string()) {
                    params[key] = value.get<std::string>();
                }
            }

            algorithm->setParameters(params);
            algorithms.push_back(std::move(algorithm));
        }

    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error parsing JSON: " + std::string(e.what()));
    }

    return algorithms;
}


//序列化如何实现
std::string parseToJson(const Algos& algos) {
    try {
        nlohmann::json j;
        j["pipeline_name"] = "边缘检测与直线检测管道";
        j["version"] = "1.0";
        j["description"] = "用于工业零件检测的视觉算法管道";

        nlohmann::json pipelineArray = nlohmann::json::array();//为什么用数组？
                              // 因为processing_pipeline（文档里的例子）是数组里包含着的json的嵌套关系

        // 遍历所有算法
        for (const auto& algo : algos) {
            if (!algo) {
                continue;  // 跳过空指针
            }

            nlohmann::json algoJson;

            // 算法名称
            algoJson["algorithm"] = algo->getName();

            // 参数
            ParameterMap params = algo->getParameters();
            nlohmann::json paramsJson = nlohmann::json::object();

            // 遍历所有参数
            for (const auto& [key, value] : params) {
                // 根据参数值的类型存储到 JSON
                if (std::holds_alternative<int>(value)) {
                    paramsJson[key] = std::get<int>(value);
                }
                else if (std::holds_alternative<double>(value)) {
                    paramsJson[key] = std::get<double>(value);
                }
                else if (std::holds_alternative<bool>(value)) {
                    paramsJson[key] = std::get<bool>(value);
                }
                else if (std::holds_alternative<std::string>(value)) {
                    paramsJson[key] = std::get<std::string>(value);
                }
            }

            algoJson["parameters"] = paramsJson;
            pipelineArray.push_back(algoJson);
        }



        // 转换为字符串
        std::string jsonStr = j.dump(4);  // 缩进4个空格，便于阅读
        return jsonStr;

    }
    catch (const std::exception& e) {
        std::cerr << "Error serializing to JSON: " << e.what() << std::endl;
        return "";
    }
}


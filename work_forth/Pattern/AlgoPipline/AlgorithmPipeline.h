#pragma once
#include <vector>
#include <memory>
#include "AlogInterface.h"
#include "parseAlgoJson.h"

// 算法管道管理器
class AlgorithmPipeline {
private:
    std::vector<std::unique_ptr<AlgorithmInterface>> m_algorithms;
    std::string m_name;

public:
    AlgorithmPipeline(const std::string& name, std::vector<std::unique_ptr<AlgorithmInterface>> algorithms)
        : m_name(name), m_algorithms(std::move(algorithms)) {
    }

    // 执行管道处理
    bool process(cv::Mat& input, cv::Mat& output) {
        if (m_algorithms.empty()) {
            return false;
        }

        cv::Mat current = input;
        cv::Mat intermediate;
        for (int i = 0; i < m_algorithms.size(); ++i) {
            auto intermediate = m_algorithms[i]->process(current);

            if (i < m_algorithms.size() - 1) {
                current = intermediate;
            }
            else {
                output = intermediate;
            }
        }

        return true;
    }

    // 获取算法数量
    int size() const {
        return m_algorithms.size();
    }
};

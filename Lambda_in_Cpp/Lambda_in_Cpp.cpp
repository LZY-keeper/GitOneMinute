#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = { 3, 1, 4, 1, 5, 9, 2, 6 };

    // 示例1：简单的Lambda，用于排序
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a < b; // 升序排序
        });

    // 示例2：使用捕获列表
    int minVal = 2;
    int maxVal = 5;
    // 按值捕获 minVal 和 maxVal
    auto isInRange = [minVal, maxVal](int x) -> bool {
        return x >= minVal && x <= maxVal;
        };
    auto count = std::count_if(nums.begin(), nums.end(), isInRange);
    std::cout << "Numbers in range: " << count << std::endl;

    // 示例3：使用 mutable
    int counter = 0;
    // 按值捕获 counter，并使用 mutable 来修改它
    auto incrementer = [counter]() mutable {
        counter++;
        std::cout << "Internal counter: " << counter << std::endl;
        };
    incrementer(); // 输出：Internal counter: 1
    incrementer(); // 输出：Internal counter: 2
    std::cout << "External counter: " << counter << std::endl; // 输出：External counter: 0 (未被修改)

    // 示例4：按引用捕获
    int externalVal = 10;
    auto refCapturer = [&externalVal]() {
        externalVal *= 2; // 修改外部变量
        };
    refCapturer();
    std::cout << "externalVal: " << externalVal << std::endl; // 输出：externalVal: 20

    return 0;
}
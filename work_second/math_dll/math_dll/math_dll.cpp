#include "pch.h"
#include "math_dll.h"


// 类的实现
int MathLibrary::Add(int a, int b) {
    
    return a + b;
}

int MathLibrary::Multiply(int a, int b) {
    return a * b;
}

double MathLibrary::Divide(double a, double b) {
    if (b == 0) return 0.0;
    return a / b;
}

//全局对象map
// map存的是句柄到类对象的映射
// map存的是handle->math
// C风格函数实现
//extern "C" {
//    // 句柄
//    void CreateHandle(void** handle) {
//        MathLibrary math;
//    }
//
//    MATH_API int AddNumbers(void** handle,int a, int b) {
//
//
//        return math.Add(a,b);
//    }
//
//    MATH_API double DivideNumbers(double a, double b) {
//        if (b == 0) return 0.0;
//        return a / b;
//    }
//}

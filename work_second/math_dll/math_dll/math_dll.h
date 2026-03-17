#pragma once


// C接口导出宏的最佳实践  方法一
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C 
#endif

// 导出宏定义
#ifdef MATHDLL_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif


// 导出类
class MATH_API MathLibrary {
public:
	 int Add(int a, int b);
	 int Multiply(int a, int b);
	 double Divide(double a, double b);


};


//// c接口导出方法二
//EXTERN_C MATH_API int AddNumbers(int a, int b);
//EXTERN_C MATH_API double DivideNumbers(double a, double b);


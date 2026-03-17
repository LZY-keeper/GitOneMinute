// math_handle_api.h
#pragma once

//句柄模式 c接口
#if defined(_WIN32)
#ifdef MATHDLL_EXPORTS
#define MATH_HANDLE_API __declspec(dllexport)
#else
#define MATH_HANDLE_API __declspec(dllimport)
#endif
#else
#ifdef MATHDLL_EXPORTS
#define MATH_HANDLE_API __attribute__((visibility("default")))
#else
#define MATH_HANDLE_API
#endif
#endif

// ==================== C语言兼容性 ====================
#ifdef __cplusplus
extern "C" {
#endif

// ==================== 不透明句柄类型 ====================
typedef void* MathHandle;  //给void取了一个别名：MathHandle*

// ==================== 错误码定义 ====================
typedef enum {
    MATH_HANDLE_SUCCESS = 0,
    MATH_HANDLE_ERROR_NULL_PTR = -1,
    MATH_HANDLE_ERROR_INVALID_HANDLE = -2,
    MATH_HANDLE_ERROR_INVALID_ARG = -3,
    MATH_HANDLE_ERROR_DIVIDE_BY_ZERO = -4,
    MATH_HANDLE_ERROR_OUT_OF_MEMORY = -5,
    MATH_HANDLE_ERROR_INTERNAL = -6
} MathHandleErrorCode;

// ==================== 句柄生命周期管理 ====================
MATH_HANDLE_API MathHandleErrorCode math_handle_create(MathHandle* handle);
MATH_HANDLE_API MathHandleErrorCode math_handle_destroy(MathHandle handle);

// ==================== 数学操作接口 ====================
MATH_HANDLE_API MathHandleErrorCode math_handle_add(MathHandle handle, int a, int b, int* result);



#ifdef __cplusplus
} // extern "C"
#endif


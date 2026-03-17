// text.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include"include/math_dll.h"
#include "include/math_dll_c.h"

//c接口使用
int main()
{
    MathHandle handle = NULL;
    int result_int;

    //创建句柄
    MathHandleErrorCode error = math_handle_create(&handle);
    error = math_handle_add(handle, 10, 20, &result_int);
    
    std::cout << "MathLibrary::Add(10, 20):" << result_int;

    error = math_handle_destroy(handle);

}


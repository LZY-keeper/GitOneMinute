// math_lib.cpp
#include"pch.h"
#include"framework.h"
#include "math_lib.h"


int MathUtils::Add(int a, int b) {
    return a + b;
}

int MathUtils::Multiply(int a, int b) {
    return a * b;
}

double MathUtils::Divide(double a, double b) {
    if (b == 0) return 0.0;
    return a / b;
}
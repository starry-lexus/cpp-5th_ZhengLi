/**
 *@author 岛中客、眼中你
 *@date 2023/1/8
 */

#include "eg5_13.h"

void Y::g(X *x) {   //类外定义
    x->i += 1;
}

void Z::f(X *x) {
    x->i += 5;
}

void h(X *x) {
    x->i += 10;
}
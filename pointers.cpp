#include <iostream>
#include <string>
#include "pointers.h"
using namespace std;

void print(string str, int val){
    cout << str << val << endl;
}

void f1(int n ){    ++n;   }
void f2(int &n){    ++n;   }
void f3(int *pi){   ++*pi;   pi = nullptr;  }
void f4(int *&rpi){   ++*rpi;   rpi = nullptr;  }

void DemoPointers1(){
    int a = 10, &c = a; // b=20,
    int *p = nullptr; //, *q = nullptr, *r;
    // r = nullptr;

    print("Before f1, a = ", a);
    f1(a);
    f1(5);
    f1(a+7);
    f1(c);
    print("After  f1, a = ", a);
    
    a = 10;
    print("Before f2, a = ", a);
    f2(a);
    // f2(35);
    // f2(a+4);
    f2(c);
    print("After  f2, a = ", a);


    a = 10; p = &a;
    // b = 20; //q = &b;
    print("Before f3, a = ", a);
    f3(&a);
    f3(p);
    print("After  f3, a = ", a);


}
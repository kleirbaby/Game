// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <cstdlib>
#include <iostream>

using namespace std;

unsigned getUnsigned(const char* p)
{
    const unsigned char* up;
    up = reinterpret_cast<const unsigned char*>(p);

    unsigned ret = up[0];
    ret |= (up[1] << 8);
    ret |= (up[2] << 16);
    ret |= (up[3] << 24);
    return ret;
}

struct PP
{
    unsigned len;
    char ch;
};

struct PU
{
    int id;
    unsigned short len;
    int td;
};

int main()
{
    PP p = { 1234,'c' };
    const char* ptr = reinterpret_cast<const char*>(&p);
    
    unsigned u = getUnsigned(ptr);
    cout << u << endl;

    unsigned n1 = 1 << 4;
    unsigned n2 = 1 << 3;

    cout << (n1 | n2) << endl;

    PU pu;
    cout << sizeof(PU) << endl;
    cout << &pu << endl;
    cout << &pu.id << endl;
    cout << &pu.len << endl;
    cout << &pu.td << endl;

    char* pc = new char[100];

    cout << "pc------>" << endl;

    printf("%p\n", &pc[0]);
    printf("%p\n", &pc[1]);
    printf("%p\n", &pc[0 + 2]);
    printf("%p\n", &pc[0] + 2);

    delete pc;

    unsigned* pi = new unsigned[100];

    cout << "pi------>" << endl;

    printf("%p\n", &pi[0]);
    printf("%p\n", &pi[1]);
    printf("%p\n", &pi[0 + 2 * 4]);
    printf("%p\n", &pi[0] + 2 * 4);

    delete pi;
}
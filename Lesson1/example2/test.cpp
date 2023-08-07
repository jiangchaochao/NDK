#include <iostream>
#include "test.h"
using namespace std;
int test::add(int a, int b) {

	std::cout << "a: " << a << "   b: " << b << std::endl;
    return a + b;
}

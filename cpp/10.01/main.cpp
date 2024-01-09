#include <stdio.h>

class A {
    int n;
public:
    A(int n) {
        this->n = n;
    }
    int operator[](int i) {
        return i + n;
    }
};

int main()
{
    A first = 1;
    A second(10);
    printf("first: %d %d\n", first[100], first[200]);
    printf("second: %d %d\n", second[100], second[200]);

    return 0;
}
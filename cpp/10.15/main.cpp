#include <stdio.h>

template <class T>
void swap3(T &x, T &y, T &z) 
{
    T t;
    t = x;
    x = y;
    y = z;
    z = t;
}

int main()
{
    char x, y, z;
    x = 1;
    y = 2;
    z = 3;

    printf("%d %d %d\n", x, y, z);
    swap3(x, y, z);
    printf("%d %d %d\n", x, y, z);

    return 0;
}

#include <stdio.h>

template <class T>
T get_and_zero(T &v)
{
    T t(v);
    v = 0;
    
    return t;
}

int main()
{
    char a, b = 100;

    printf("%d\n", b);
    a = get_and_zero(b);
    printf("%d %d\n", a, b);

    return 0;
}

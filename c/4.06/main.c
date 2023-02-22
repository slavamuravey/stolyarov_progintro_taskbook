#include <stdio.h>

int put_sum(int *p1, int *p2, int *p3)
{
    int result = *p1 + *p2 + *p3;
    *p1 = *p2 = *p3 = result;

    return result;
}

int main()
{
    int v1 = 7;
    int v2 = 77;
    int v3 = 777;

    int result = put_sum(&v1, &v2, &v3);

    printf("v1 = %d, v2 = %d, v3 = %d; put_sum result = %d\n", v1, v2, v3, result);

    return 0;
}

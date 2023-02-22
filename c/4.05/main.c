#include <stdio.h>

int get_and_zero(int *p)
{
    int result = *p;
    *p = 0;

    return result;
}

int main()
{
    int variable = 7;

    int result = get_and_zero(&variable);

    printf("Variable value = %d; get_and_zero result = %d\n", variable, result);

    return 0;
}

#include <stdio.h>

class B {
    int v;
public:
    B(int v) {
        this->v = v;
    }

    int operator+=(int addendum) {
        return v += addendum;
    }

    int Get() {
        return v;
    }
};

int main()
{
    B first(1), second = 2;
    first += 10; second += 1000;
    printf("%d %d\n", first.Get(), second.Get());

    return 0;
}
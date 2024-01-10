#include <stdio.h>

class D {
    int v;
public:
    D() {
        v = 0;
    }

    D(int i) {
        v = i;
    }
    
    D(const D &o) {
        v = o.Get() + 1;
    }

    int Get() const {
        return v;
    }
};

int main()
{
    D x;
    D y(x);
    D z = y;
    printf("%d %d %d\n", x.Get(), y.Get(), z.Get());

    return 0;
}
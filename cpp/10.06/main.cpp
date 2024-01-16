#include <stdio.h>
#include <string.h>

class E {
    class Row {
        int idx;
    public:
        Row(const int idx) {
            this->idx = idx;
        }
        int operator[](const int idx) const {
            return this->idx == idx ? 0 : this->idx - idx;
        }
    };
public:
    Row operator[](const int idx) const {
        return Row(idx);
    }
};

int main()
{
    E e;
    printf("%d %d %d\n", e[0][0], e[100][100], e[-10][-10]);
    printf("%d %d %d\n", e[1500][7], e[7][55], e[-8][-16]);

    return 0;
}

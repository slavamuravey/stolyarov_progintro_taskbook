#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SIZE 3

class M {
    mutable int m[SIZE][SIZE];
    class Row {
        int *ptr;
    public:
        Row(int *ptr) {
            this->ptr = ptr;
        }

        int &operator[](const unsigned int idx) const {
            return ptr[idx - 1];
        }
    };
public:
    M() {
        int i;
        
        memset(m, 0, sizeof(m));
        for (i = 0; i < SIZE; i++) {
            m[i][i] = 1;
        }
    }

    Row operator[](const unsigned int idx) {
        return Row(m[idx - 1]);
    }

    M operator+(M &m_add) const {
        M new_m;
        int i, j;
        for (i = 1; i <= SIZE; i++) {
            for (j = 1; j <= SIZE; j++) {
                new_m[i][j] = m[i][j] + m_add[i][j];
            }
        }

        return new_m;
    }
};

int main()
{
    M m1;
    printf("%d %d %d\n", m1[1][1], m1[2][2], m1[2][3]);
    M m2;
    m1[2][3] = 7;
    m2[2][3] = 350;
    M m3(m1 + m2);
    printf("%d %d %d\n", m3[1][1], m3[2][2], m3[2][3]);

    return 0;
}

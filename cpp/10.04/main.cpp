#include <stdio.h>
#include <string.h>

#define SIZE 3

class I {
    unsigned int col;
    unsigned int row;
public:
    I(unsigned int col, unsigned int row) {
        this->col = col;
        this->row = row;
    }

    unsigned int Col() const {
        return col;
    }

    unsigned int Row() const {
        return row;
    }
};

class M {
    int m[SIZE][SIZE];
public:
    M() {
        int i;

        memset(m, 0, sizeof(m));
        for (i = 0; i < SIZE; i++) {
            m[i][i] = 1;
        }
    }

    int &operator[](const I &coord) {
        return m[coord.Col() - 1][coord.Row() - 1];
    }

    M operator+(M &m_add) const {
        M new_m;
        int i, j;
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                const I coord = I(i + 1, j + 1);
                new_m[coord] = m[i][j] + m_add[coord];
            }
        }

        return new_m;
    }
};

int main()
{
    M m1;
    printf("%d %d %d\n", m1[I(1, 1)], m1[I(2, 2)], m1[I(2, 3)]);
    M m2;
    m1[I(2, 3)] = 7;
    m2[I(2, 3)] = 350;
    M m3(m1 + m2);
    printf("%d %d %d\n", m3[I(1, 1)], m3[I(2, 2)], m3[I(2, 3)]);

    return 0;
}

struct item {
};

int main()
{
    int a[245][12];
    double b[100][10][12];
    char c[5][5];
    char *d[5][5];
    struct item *e[20][3];
    struct item *f[20];

    int (*ap)[12] = a;
    double (*bp)[10][12] = b;
    char (*cp)[5] = c;
    char *(*dp)[5] = d;
    struct item *(*ep)[3] = e;
    struct item **fp = f;
    
    return 0;
}

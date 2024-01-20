#include <stdio.h>

class Prism {
    double h;
public:
    Prism(double h): h(h) {}
    virtual double Square() const = 0;
    double Volume() const {
        return h * Square();
    }
};

class Box: public Prism {
    double a;
public:
    Box(double h, double a): Prism(h), a(a) {}
    virtual double Square() const {
        return a * a;
    }
};

class Cube: public Box {
public:
    Cube(double a): Box(a, a) {}
};

int main()
{
    const Prism *p, *q, *r;
    Box a(0.5, 2), b(5, 0.2);
    Cube c(0.5);
    p = &a; q = &b; r = &c;
    printf("Squares: %3.3f %3.3f %3.3f\n", p->Square(), q->Square(), r->Square());
    printf("Volumes: %3.3f %3.3f %3.3f\n", p->Volume(), q->Volume(), r->Volume());

    return 0;
}

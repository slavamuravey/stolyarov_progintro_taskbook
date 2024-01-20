#include <stdio.h>
#include <math.h>

class Body {
    double density;
public:
    Body(double density): density(density) {}
    virtual double Volume() const = 0;
    double Mass() const {
        return density * Volume();
    }
};

class Cube: public Body {
    double rib_len;
public:
    Cube(double rib_len, double density): Body(density), rib_len(rib_len) {}
    virtual double Volume() const {
        return rib_len * rib_len * rib_len;
    }
};

class Tetrahedron: public Body {
    double rib_len;
public:
    Tetrahedron(double rib_len, double density): Body(density), rib_len(rib_len) {}
    virtual double Volume() const {
        return rib_len * rib_len * rib_len * M_SQRT2 / 12;
    }
};

int main()
{
    const Body *p, *q, *r;
    Cube a(2, 10), b(5, 0.1);
    Tetrahedron t(6, 2.5);
    p = &a; q = &b; r = &t;
    printf("Volumes: %3.3f %3.3f %3.3f\n", p->Volume(), q->Volume(), r->Volume());
    printf("Weights: %3.3f %3.3f %3.3f\n", p->Mass(), q->Mass(), r->Mass());

    return 0;
}

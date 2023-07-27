#include <math.h>
#include <stdio.h>
#include <assert.h>

#define PhysConsts_gamma 1.4

int ptoc(double* p, double* c) {
    assert(p != NULL);
    assert(c != NULL);

    double rho, velx, vely, velz, velx2, vely2, velz2, ke, pressure;
    double bx, by, bz, bsquared;
    double gammag = PhysConsts_gamma;
    double gammam1 = gammag - 1;
    double gammam1i = 1 / gammam1;

    rho = p[0];
    velx = p[1];
    vely = p[2];
    velz = p[3];
    pressure = p[4];
    bx = p[5];
    by = p[6];
    bz = p[7];

    velx2 = velx * velx;
    vely2 = vely * vely;
    velz2 = velz * velz;
    ke = 0.5 * rho * (velx2 + vely2 + velz2);
    bsquared = bx * bx + by * by + bz * bz;

    c[0] = rho;
    c[1] = rho * velx;
    c[2] = rho * vely;
    c[3] = rho * velz;
    c[4] = ke + pressure * gammam1i + 0.5 * bsquared;
    c[5] = bx;
    c[6] = by;
    c[7] = bz;

    if (c[4] <= 1e-20) {
        printf("p %f %f %f %f %f %f %f %f\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        printf("c %f %f %f %f %f %f %f %f\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
        printf("ptoc: Mein Gott!\n");
        assert(0);
    }

    if (vely < -400) {
        printf("p %f %f %f %f %f %f %f %f\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        printf("c %f %f %f %f %f %f %f %f\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
    }

    return 0;
}

int ctop(double* c, double* p, double* loc) {
    assert(c != NULL);
    assert(p != NULL);
    assert(loc != NULL);

    double pmin = 1e-20;
    double rho, rhoi;
    double px, py, pz, et, velx, vely, velz, velx2, vely2, velz2, ke, pressure;
    double bx, by, bz, bsquared;
    double gammag = PhysConsts_gamma;
    double gammam1 = gammag - 1;

    rho = c[0];
    px = c[1];
    py = c[2];
    pz = c[3];
    et = c[4];
    bx = c[5];
    by = c[6];
    bz = c[7];
    rhoi = 1.0 / rho;
    velx = px * rhoi;
    vely = py * rhoi;
    velz = pz * rhoi;
    velx2 = velx * velx;
    vely2 = vely * vely;
    velz2 = velz * velz;
    ke = 0.5 * rho * (velx2 + vely2 + velz2);
    bsquared = 0.5 * (bx * bx + by * by + bz * bz);

    double internalenergy = et - ke - bsquared;
    pressure = internalenergy * gammam1;

    p[0] = rho;
    p[1] = velx;
    p[2] = vely;
    p[3] = velz;
    pressure = fmax(pressure, pmin);
    p[4] = pressure;
    p[5] = bx;
    p[6] = by;
    p[7] = bz;

#ifdef VERBOSE_PRESSURE
    if (pressure < 1e-20) {
        printf("cons %f mv= %f %f %f e =  %f b = %f %f %f\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
        printf("prim %f v= %f %f %f p = %f b = %f %f %f\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        printf("ctop: pressure low\n");
        if (pressure < 0) {
            printf("pressure < 0 %f\n", pressure);
            return 1;
        }
    }
#endif

#ifdef CHECK_WEIRD_VELOCITY
    if (vely < -400) {
        printf("p %f %f %f %f %f %f %f %f\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        printf("c %f %f %f %f %f %f %f %f\n", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
        printf("Vel < -400\n");
    }
#endif

    return 0;
}

int main() {
    double p[8] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
    double c[8];

    ptoc(p, c);
    ctop(c, p, NULL);

    return 0;
}

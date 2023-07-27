#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define fabs(x) ((x) >= 0 ? (x) : -(x))

struct unk {
    double array[8];
};

int maxspeed(struct unk** mesh, int nx, int ny, double* maxspeed);

int main() {
    int ii = 0;
    int jj = 0;

    double gammag = 5.0 / 3.0;
    double gammam1 = gammag - 1;

    double ri = 0, px = 0, py = 0, pz = 0, et = 0, ke = 0;
    double rl = 0, ul = 0, vl = 0, wl, pl = 0, al = 0, speed = 0;
    double bu, bv, bw, bsquared;
    double calfven = 0;
    double calfven2 = 0;
    double cfast = 0;
    double cfast2 = 0;
    double csound2 = 0;
    double csound = 0;
    double term = 0;
    double a_star2 = 0;
    double vv2 = 0;
    double speed1 = 0;
    double speed2 = 0;
    double speed3 = 0;

    int nx = 10; // Adjust the value of nx and ny as per your requirements
    int ny = 10;

    struct unk** mesh = (struct unk**)malloc(nx * sizeof(struct unk*));
    for (int i = 0; i < nx; i++) {
        mesh[i] = (struct unk*)malloc(ny * sizeof(struct unk));
    }

    double max_speed = 0.0;

    for (ii = 2; ii < nx - 2; ii++) {
        for (jj = 2; jj < ny - 2; jj++) {

            rl = mesh[ii][jj].array[0];
            px = mesh[ii][jj].array[1];
            py = mesh[ii][jj].array[2];
            pz = mesh[ii][jj].array[3];
            et = mesh[ii][jj].array[4];

            bu = mesh[ii][jj].array[5];
            bv = mesh[ii][jj].array[6];
            bw = mesh[ii][jj].array[7];

            ri = 1.0 / rl;
            ul = px * ri;
            vl = py * ri;
            wl = pz * ri;
            vv2 = (ul * ul + vl * vl + wl * wl);
            ke = 0.5 * rl * vv2;
            bsquared = (bu * bu + bv * bv + bw * bw);
            pl = et - ke - 0.5 * bsquared;
            pl = pl * (gammam1);
            bsquared = bsquared * ri;

            calfven2 = bu * bu * ri;
            calfven = sqrt(calfven2);
            csound2 = gammag * pl / rl;
            csound = sqrt(csound2);
            a_star2 = (csound2 + bsquared);
            term = sqrt(a_star2 * a_star2 - 4 * csound2 * calfven2);
            cfast2 = 0.5 * (a_star2 + term);
            cfast = sqrt(cfast2);

            speed = max(fabs(ul), fabs(vl)) + cfast;
            speed1 = fabs(ul) + cfast;

            if (speed1 > max_speed) {
                max_speed = speed1;
            }

            calfven2 = bv * bv * ri;
            term = sqrt(a_star2 * a_star2 - 4 * csound2 * calfven2);
            cfast2 = 0.5 * (a_star2 + term);
            cfast = sqrt(cfast2);
            speed2 = fabs(vl) + cfast;

            if (speed2 > max_speed) {
                max_speed = speed2;
            }

            calfven2 = bw * bw * ri;
            term = sqrt(a_star2 * a_star2 - 4 * csound2 * calfven2);
            cfast2 = 0.5 * (a_star2 + term);
            cfast = sqrt(cfast2);
            speed3 = fabs(wl) + cfast;

            if (speed3 > max_speed) {
                max_speed = speed3;
            }

            if (isinf(max_speed)) {
                printf("Max speed is infinite at location (%d, %d)\n", ii, jj);
                exit(0);
            }
        }
    }

    printf("Max speed: %lf\n", max_speed);

    // Free allocated memory
    for (int i = 0; i < nx; i++) {
        free(mesh[i]);
    }
    free(mesh);

    return 0;
}

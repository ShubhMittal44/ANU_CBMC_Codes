#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PhysConsts.h

const double PhysConsts_pi = 3.14159;
const double PhysConsts_gamma = 1.4;

typedef struct {
    double _MASS;
    double _MOMX;
    double _MOMY;
    double _MOMZ;
    double _ENER;
    double _B_X;
    double _B_Y;
    double _B_Z;
    int temperature;
} unk;

typedef struct {
    int dim1;
    int dim2;
    unk** data;
} Array2D;

typedef struct {
    double gamma;
    double pi;
} PhysConsts;

typedef struct {
    int nx;
    int ny;
    double gammam1i;
    double pressure;
    double bx;
    double by;
    double sqr4piei;
    double myx;
    double myy;
    double rh;
    double vx;
    double vy;
    double vz;
    int myid;
    int xmax;
    int ymax;
    int coords[2];
    int dims;
    int centre[2];
} Blast;

int initialise_mesh(Blast* blast, Array2D mesh, Array2D faceBx, Array2D faceBy) {
    int nx = mesh.dim1;
    int ny = mesh.dim2;
    double bx = 10;
    double by = 10;
    double sqr4piei = 0.5 / sqrt(PhysConsts::pi);
    int myx = 0;
    int myy = 0;
    double rh = 1.0;
    double vx = 0;
    double vy = 0.0;
    double vz = 0.0;
    int myid = 99;
    int xmax = 0;
    int ymax = 0;

    bx = bx * sqr4piei;
    by = by * sqr4piei;

    printf("Proc %d %d %d\n", myid, xmax, ymax);

    int myaddress[] = { 0, 0 };
    myaddress[0] = (nx - 2 * NGC) * coords[0];
    myaddress[1] = (ny - 2 * NGC) * coords[1];

    // Initialise face-centered B fields
    for (int ii = 0; ii < nx + 1; ii++) {
        for (int jj = 0; jj < ny + 1; jj++) {
            faceBx.data[ii][jj] = bx;
            faceBy.data[ii][jj] = by;
        }
    }

    for (int jj = 0; jj < ny; jj++) {
        for (int ii = 0; ii < nx; ii++) {
            int gg = ii - NGC;
            int hh = jj - NGC;

            bx = 0.5 * (faceBx.data[ii][jj] + faceBx.data[ii + 1][jj]);
            by = 0.5 * (faceBy.data[ii][jj] + faceBy.data[ii][jj + 1]);
            double pressure = 0.1;

            mesh.data[ii][jj]._MASS = 1.0;
            mesh.data[ii][jj]._MOMX = 0.0;
            mesh.data[ii][jj]._MOMY = 0.0;
            mesh.data[ii][jj]._MOMZ = 0.0;
            mesh.data[ii][jj]._ENER = pressure * blast->gammam1i + 0.5 * (bx * bx + by * by);
            mesh.data[ii][jj]._B_X = bx;
            mesh.data[ii][jj]._B_Y = by;
            mesh.data[ii][jj]._B_Z = 0.0;
            mesh.data[ii][jj].temperature = myid;

            blast->centre[0] = blast->xmax * (nx - 2 * NGC) / 2 + 10;
            blast->centre[1] = blast->ymax * (ny - 2 * NGC) / 2 + 10;

            myx = (float)myaddress[0] + gg - blast->centre[0];
            myy = (float)myaddress[1] + hh - blast->centre[1];

            double dist = sqrt((myx * myx) + (myy * myy));

            if (dist < 0.1 * nx) {
                mesh.data[ii][jj]._MASS = rh;
                mesh.data[ii][jj]._MOMX = vx;
                mesh.data[ii][jj]._MOMY = vy;
                mesh.data[ii][jj]._MOMZ = vz;
                mesh.data[ii][jj]._B_X = bx;
                mesh.data[ii][jj]._B_Y = by;
                mesh.data[ii][jj]._B_Z = 0.0;
                mesh.data[ii][jj]._ENER = 10.0 * blast->gammam1i + 0.5 * rh * (vx * vx + vy * vy) + 0.5 * (bx * bx + by * by);
            }
        }
    }

    return 0;
}

int main() {
    Blast blast;
    Array2D mesh;
    Array2D faceBx;
    Array2D faceBy;

    int nx = 10; // Provide appropriate dimensions
    int ny = 10; // Provide appropriate dimensions

    // Allocate memory for the arrays
    mesh.dim1 = nx;
    mesh.dim2 = ny;
    mesh.data = malloc(nx * sizeof(unk*));
    for (int i = 0; i < nx; i++) {
        mesh.data[i] = malloc(ny * sizeof(unk));
    }

    faceBx.dim1 = nx + 1;
    faceBx.dim2 = ny + 1;
    faceBx.data = malloc((nx + 1) * sizeof(double*));
    for (int i = 0; i < nx + 1; i++) {
        faceBx.data[i] = malloc((ny + 1) * sizeof(double));
    }

    faceBy.dim1 = nx + 1;
    faceBy.dim2 = ny + 1;
    faceBy.data = malloc((nx + 1) * sizeof(double*));
    for (int i = 0; i < nx + 1; i++) {
        faceBy.data[i] = malloc((ny + 1) * sizeof(double));
    }

    initialise_mesh(&blast, mesh, faceBx, faceBy);

    // Print the results
    for (int i = 0; i < mesh.dim1; i++) {
        for (int j = 0; j < mesh.dim2; j++) {
            printf("mesh[%d][%d]: _MASS = %lf, _MOMX = %lf, _MOMY = %lf, _MOMZ = %lf, _ENER = %lf, _B_X = %lf, _B_Y = %lf, _B_Z = %lf, temperature = %d\n",
                i, j, mesh.data[i][j]._MASS, mesh.data[i][j]._MOMX, mesh.data[i][j]._MOMY, mesh.data[i][j]._MOMZ,
                mesh.data[i][j]._ENER, mesh.data[i][j]._B_X, mesh.data[i][j]._B_Y, mesh.data[i][j]._B_Z, mesh.data[i][j].temperature);
        }
    }

    // Deallocate memory
    for (int i = 0; i < nx; i++) {
        free(mesh.data[i]);
    }
    free(mesh.data);

    for (int i = 0; i < nx + 1; i++) {
        free(faceBx.data[i]);
    }
    free(faceBx.data);

    for (int i = 0; i < nx + 1; i++) {
        free(faceBy.data[i]);
    }
    free(faceBy.data);

    return 0;
}

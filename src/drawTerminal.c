#include "Particle.h"
#include "checker.h"
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25

// need to do scaling and conversion to int before comparing
int cmpForPrint(Particle *a, Particle *b)
{
    if (a == b) return 0; // both points to same particle or both null
    if (!b) return 1;     // only a
    if (!a) return -1;    // only b
    int arx = (int)(a->rx * WIDTH);
    int brx = (int)(b->rx * WIDTH);
    int ary = (int)(a->ry * HEIGHT);
    int bry = (int)(b->ry * HEIGHT);
    if (ary > bry)
        return 1;
    else if (ary < bry)
        return -1;
    // now ary = bry. compare rx
    else if (arx > brx)
        return 1;
    else if (arx < brx)
        return -1;
    else
        return 0;
}

int cmpWrapperforPrint(const void *a, const void *b)
{
    // a and b are pointer to Particle*
    return cmpForPrint(*(Particle **)a, *(Particle **)b);
}
void sortForPrint(Particle **particles, size_t n)
{
    qsort(particles, n, sizeof(Particle *), cmpWrapperforPrint);
}

int isSortedForPrint(Particle **particles, size_t n)
{
    for (size_t i = 0; i < n - 1; i++) {
        if (cmpForPrint(particles[i], particles[i - 1]) > 0)
            return 0;
    }
    return 1;
}

void draw(Particle **particles, size_t n)
{
    if (!isSortedForPrint(particles, n)) sortForPrint(particles, n);
    int row = 0;
    int col = 0;
    Particle *p;
    for (size_t i = 0; i <= WIDTH; i++) {
        printf("_");
    }
    printf("\n|");
    for (size_t i = 0; i < n; i++) {
        p = particles[i];
        while (row != (int)(HEIGHT * p->ry)) {
            while (col < WIDTH) {
                printf(" ");
                col++;
            }
            printf("|\n|");
            col = 0;
            ++row;
            if (row > HEIGHT) return;
        }
        while (col != (int)(WIDTH * p->rx)) {
            if (col >= WIDTH) {
                printf("|\n|");
                col = 0;
                continue;
            }
            printf(" ");
            col = (col + 1);
        }
        // printf("*%d %d", row, col);
        printf("*");
        col++;
    }
    while (col < WIDTH) {
        printf(" ");
        col++;
    }
    printf("|\n|");
    for (size_t i = 0; i < WIDTH; i++) {
        printf("_");
    }
    printf("|");
}

// int main()
// {
//     Particle *a[200];
//     int n = 200;
//     for (size_t i = 0; i < n; i++) {
//         a[i] = createRandomParticle();
//     }
//     sortForPrint(a, n);
//     // for (size_t i = 0; i < n; i++) {
//     //     printf("%lf %lf\n", a[i]->rx, a[i]->ry);
//     // }
//     draw(a, n);
// }
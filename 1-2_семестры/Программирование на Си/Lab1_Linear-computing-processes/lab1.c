#include <stdio.h>
#include <math.h>

int main(void) {
    double a, x, b, p, y, z;

    printf("x: "); scanf("%lf", &x);
    printf("a: "); scanf("%lf", &a);
    printf("b: "); scanf("%lf", &b);

    y = (14 * fabs(log10(x)) + 2) / (40 + x);
    p = cos(a) + 11 * y * y;
    z = (p * p) / (p + b);

    printf("p = %7.5lf\n", p);
    printf("y = %7.5lf\n", y);
    printf("z = %7.5lf\n", z);
    return 0;
}

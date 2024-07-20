#include <stdio.h>
#include "../include/space.h"

int main(int argc, char** argv){
    printf("Hello world, I'm back for the best and the segfault \n");

    Vector a = {-1, 1, -1};
    Vector b = { 1,  1,  1};
    float alpha = 3;

    printf("The scalar product of a and b is %f  \n", scalar_product(a, b));
    printf("The multiplication of %f by a is %s \n", alpha, VectorToString( mult(alpha, a) ) );

    return 0;
}
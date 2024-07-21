#include <stdio.h>
#include <stdlib.h>
#include "../include/mesh.h"

int main(int argc, char** argv)
{
    Mesh* m = NULL;
    initMesh(m, 3, 3);
    log_info("The number of springs in this network is %d", numberOfSprings(3, 3));
    log_debug("Exiting ");

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "../include/mesh.h"

int main(int argc, char** argv)
{
    Mesh* m = NULL;
    initMesh(m, 5, 5);
    log_error("Exiting ");

    return 0;
}
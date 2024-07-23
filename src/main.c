#include <stdio.h>
#include <stdlib.h>
#include "../include/mesh.h"

int main(int argc, char** argv)
{
    Mesh* m = (Mesh*) malloc(sizeof(Mesh));
    initMesh(m, 10, 10);
    log_info("The number of springs in this network is %d", numberOfSprings(3, 3));
    
    if ( m == NULL )
    {
        log_error("Mesh is still null");
        return 0;
    }
    convert_mesh_to_vtk(m, "mesh_polydata.vtk");
    convert_mesh_to_unstructure_grid_vtk(m, "mesh_unstruct_grid.vtk");

    log_debug("Exiting ");
    return 0;
}
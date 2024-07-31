#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/mesh.h"

int main(int argc, char** argv)
{
    Mesh* m = (Mesh*) malloc(sizeof(Mesh));
    meshType type = TABLE;
    initMesh(m, 50, 50, type);
    log_info("The number of springs in this network is %d", numberOfSprings(m->n, m->m));

    float delta_t = 0.1f;
    unsigned int count = 10000;
    char poly_file_name[256];
    char grid_file_name[256];

    int step = 30;
    log_info("Starting file generation: delta_time=%.3f number_of_file=%d", delta_t, count/step);
    clock_t start_time = clock();  // Start the timer
    for(unsigned int i=0; i< count; i++)
    {
        if( i%step == 0){
            snprintf(poly_file_name, sizeof(poly_file_name), "vtk_poly/mesh_poly_%03u.vtk", i);
            snprintf(grid_file_name, sizeof(grid_file_name), "vtk_grid/mesh_grid_%03u.vtk", i);
            convertMeshToPolyVTK(m, poly_file_name);
            convertMeshToGridVTK(m, grid_file_name);
        }
        
        updatePosition(m, delta_t, type);
    }
    clock_t end_time = clock();  // End the timer
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // Calculate elapsed time in seconds
    log_info("File generation completed in %.3f seconds", elapsed_time);
    
    freeMesh(m);
    log_debug("Exiting ");
    return 0;
}
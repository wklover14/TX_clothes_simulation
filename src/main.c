#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/mesh.h"
#include "../include/utils.h"

int main(int argc, char** argv)
{
    Mesh* m = (Mesh*) malloc(sizeof(Mesh));
    meshType type = parseArguments(argc, argv);


    initMesh(m, 50, 50, type);
    log_info("The number of springs in this network is %d", numberOfSprings(m->n, m->m));

    float delta_t = 0.1f;
    unsigned int count = 10000;
    const char* type_name = getTypeName(type);
    char poly_file_name[256];
    char grid_file_name[256];

    int step = 30;
    log_info("Starting file generation: delta_time=%.3f number_of_file=%d", delta_t, count/step);

    // create directories
    snprintf(poly_file_name, sizeof(poly_file_name), "vtk_poly_%s", type_name);
    snprintf(grid_file_name, sizeof(grid_file_name), "vtk_grid_%s", type_name);
    createDirectory(poly_file_name);
    createDirectory(grid_file_name);
    

    clock_t start_time = clock();  // Start the timer
    for(unsigned int i=0; i< count; i++)
    {
        if( i%step == 0){ // write the mesh to vtk files
            snprintf(poly_file_name, sizeof(poly_file_name), "vtk_poly_%s/mesh_poly_%s_%03u.vtk", type_name, type_name,i);
            snprintf(grid_file_name, sizeof(grid_file_name), "vtk_grid_%s/mesh_grid_%s_%03u.vtk", type_name, type_name,i);
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
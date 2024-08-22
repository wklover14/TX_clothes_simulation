#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "../include/mesh.h"
#include "../include/params.h"
#include "../include/utils.h"

int main(int argc, char **argv) {
  // Allocate memory for the mesh structure
  Mesh *m = (Mesh *)malloc(sizeof(Mesh));

  // Parse command-line arguments to determine the type of mesh
  meshType type = parseArguments(argc, argv);

  // Initialize the mesh with the specified type
  initMesh(m, type);

  // Log the total number of springs in the mesh
  log_info("The number of springs in this network is %d",
           numberOfSprings(m->n, m->m));

  // Get the string representation of the mesh type
  const char *type_name = getTypeName(type);

  // Buffers for storing file paths
  char poly_file_name[256];
  char grid_file_name[256];

  // Log the start of file generation, including the delta time and number of
  // files
  log_info("Starting file generation: delta_time=%.3f number_of_file=%d",
           DELTA_T, NB_UPDATES / STEP);

  // Create directories for storing VTK files
  snprintf(poly_file_name, sizeof(poly_file_name), "vtk_poly_%s", type_name);
  snprintf(grid_file_name, sizeof(grid_file_name), "vtk_grid_%s", type_name);
  createDirectory(poly_file_name);
  createDirectory(grid_file_name);

  // Variables to store the start and end time for performance measurement
  struct timespec start_time, end_time;

  // Start the timer to measure the execution time
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  // Main loop to update the mesh over time
  for (unsigned int i = 0; i < NB_UPDATES; i++) {
    // Every 'STEP' iterations, save the current state of the mesh to VTK files
    if (i % STEP == 0) {
      // Generate file names for the current iteration
      snprintf(poly_file_name, sizeof(poly_file_name),
               "vtk_poly_%s/mesh_poly_%s_%03u.vtk", type_name, type_name, i);
      snprintf(grid_file_name, sizeof(grid_file_name),
               "vtk_grid_%s/mesh_grid_%s_%03u.vtk", type_name, type_name, i);

      // Convert the current mesh state to VTK format and save
      convertMeshToPolyVTK(m, poly_file_name);
      convertMeshToGridVTK(m, grid_file_name);
    }

    // Update the position of the mesh points for the next iteration
    updatePosition(m, DELTA_T, type);
  }

  // End the timer after the main loop has completed
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  // Calculate the total elapsed time in seconds
  double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

  // Log the time taken for file generation
  log_info("File generation completed in %.3f seconds", elapsed_time);

  // Free the allocated memory for the mesh structure
  freeMesh(m);

  // Log that the program has finished generating files and is exiting
  log_debug("Files generated. Exiting ");
  return 0;
}

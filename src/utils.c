#include "utils.h"

int createDirectory(const char *path) {
    struct stat st = {0};

    if (stat(path, &st) == -1) {
        if (MKDIR(path) == 0) {
            log_info("Directory created: %s", path);
            return 0; // Success
        } else {
            log_error("Error creating directory");
            return -1; // Failure
        }
    } else {
        log_debug("Directory already exists: %s", path);
        return 0; // Success (directory already exists)
    }
}

meshType parseArguments(int argc, char *argv[]) {
    if (argc != 2) { // only one argument
        log_error("Usage: %s [curtain] | [table-cloth] | [soft] ", argv[0]);
        exit(EXIT_FAILURE);
    }

    if( strcmp(argv[1], "curtain") == 0 )
    {   
        return CURTAIN;
    } else if (strcmp(argv[1], "table-cloth") == 0)
    {
        return TABLE_CLOTH;
    } else if (strcmp(argv[1], "soft")== 0)
    {
        return SOFT;
    } else if(strcmp(argv[1], "flag")== 0)
    {
        return FLAG;
    } else
    {
        log_error("the requested arguments doesn't exists");
        log_error("Usage: %s [curtain] | [table-cloth] ", argv[0]);
        exit(EXIT_FAILURE);
    }
}


/**
 * Return the string corresponding to a meshType
 */
const char* getTypeName(meshType type)
{
    switch (type)
    {
    case CURTAIN:
        return "curtain";

    case TABLE_CLOTH:
        return "table_cloth";

    case SOFT:
        return "soft";
    
    case FLAG:
        return "flag";

    default:
        log_error("Mesh type not handled");
        return "xxx";
    }
}

        
/** 
 * Convert a mesh into a set of points and lines in a vtk file
 */ 
void convertMeshToPolyVTK(const Mesh *mesh, const char *output_filename)
{
    FILE* file = fopen(output_filename, "w");
    if (file == NULL) {
        log_error("Error: Could not open file %s.\n",output_filename);
        return;
    }

    // Write VTK file header
    fprintf(file, "# vtk DataFile Version 4.2\n");
    fprintf(file, "Mesh Data\n");
    fprintf(file, "ASCII\n");
    fprintf(file, "DATASET POLYDATA\n");

    // Write points
    unsigned int total_points = mesh->n * mesh->m;
    fprintf(file, "POINTS %u float\n", total_points);
    for (unsigned int i = 0; i < mesh->n; i++) {
        for (unsigned int j = 0; j < mesh->m; j++) {
            fprintf(file, "%3f %3f %3f\n", mesh->P[i][j].x, mesh->P[i][j].y, mesh->P[i][j].z);
        }
    }

    // Write lines (springs)
    unsigned int total_number_of_springs = numberOfSprings(mesh->n , mesh->m);

    // print all springs
    fprintf(file, "LINES %u %u\n", mesh->n_springs, 3 * mesh->n_springs);
    for (unsigned int k = 0; k < total_number_of_springs; k++) {
        
        if(mesh->springs[k].isBreak) continue;

        // Convert grid coordinates (i, j) to point indices
        unsigned int id1 = mesh->springs[k].ext_1.i * mesh->m + mesh->springs[k].ext_1.j;
        unsigned int id2 = mesh->springs[k].ext_2.i * mesh->m + mesh->springs[k].ext_2.j;
        fprintf(file, "2 %d %d\n", id1, id2);
    }

    fclose(file);
}

/**
 * Convert a Mesh into a a grid that can be used as surface easily.
 */
void convertMeshToGridVTK(const Mesh *mesh, const char *output_filename)
{
    FILE* file = fopen(output_filename, "w");
    if (file == NULL) {
        log_error("Error: Could not open file %s.\n",output_filename);
        return;
    }
    // Write VTK file header
    fprintf(file, "# vtk DataFile Version 4.2\n"); // Only 4.2 version is correctly supported by Paraview
    fprintf(file, "Unstructured Grid Mesh\n");
    fprintf(file, "ASCII\n");
    fprintf(file, "DATASET UNSTRUCTURED_GRID\n");

    // Write points
    unsigned int total_points = mesh->n * mesh->m;
    fprintf(file, "POINTS %u float\n", total_points);
    for (unsigned int i = 0; i < mesh->n; i++) {
        for (unsigned int j = 0; j < mesh->m; j++) {
            fprintf(file, "%f %f %f\n", mesh->P[i][j].x, mesh->P[i][j].y, mesh->P[i][j].z);
        }
    }

    // Write cells (quadrilateral)
    unsigned int total_cells = (mesh->n - 1) * (mesh->m - 1);
    fprintf(file, "CELLS %u %u\n", total_cells, 5 * total_cells); // Each cell has 4 points + 1 (for the number of points)
    for (unsigned int i = 0; i < mesh->n - 1; i++) {
        for (unsigned int j = 0; j < mesh->m - 1; j++) {
            unsigned int id1 = i * mesh->m + j;
            unsigned int id2 = i * mesh->m + (j + 1);
            unsigned int id3 = (i + 1) * mesh->m + j;
            unsigned int id4 = (i + 1) * mesh->m + (j + 1);
            fprintf(file, "4 %d %d %d %d\n", id1, id2, id4, id3);
        }
    }

    // Write cell types
    // VTK_QUAD = 9
    fprintf(file, "CELL_TYPES %u\n", total_cells);
    for (unsigned int k = 0; k < total_cells; k++) {
        fprintf(file, "7\n");
    }

    fclose(file);
}
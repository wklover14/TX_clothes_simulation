#include "../include/mesh.h"

const float    Mu               = 0.02f;                    // Mass of a point
const float    C_DIS            = 0.04f;                    // Damping coefficient
const float    C_VI             = 0.023f;                   // Viscous coefficient


/**
 * Correctly allocate all attributes of a mesh, it fails if the mesh provided is NULL
 */
void initMesh(Mesh* mesh,unsigned int n,unsigned int m)
{
    if(mesh == NULL)
    {
        log_error("Mesh provided is empty!!");
        return;
    } 

    if(n == 0 || m == 0)
    {
        log_error("Bad parameters, neither n nor m can be negative or null");
        return;
    }

    mesh->n = n;
    mesh->m = m;
    mesh->t = 0.0f; // the initial is zero    

    mesh->P = (Vector**) malloc(n * sizeof(Vector*));
    mesh->V = (Vector**) malloc(n * sizeof(Vector*));
    mesh->A = (Vector**) malloc(n * sizeof(Vector*));

    unsigned int N   = numberOfSprings(n, m); // total number of springs in the mesh
    mesh->springs   = (Spring*) malloc(N * sizeof(Spring));

    unsigned int spring_index = 0; // index to iterate through the springs list

    for(unsigned int i = 0; i < n ; i++) 
    {
        mesh->P[i] = (Vector*) malloc(m * sizeof(Vector));
        mesh->V[i] = (Vector*) malloc(m * sizeof(Vector));
        mesh->A[i] = (Vector*) malloc(m * sizeof(Vector));
        
        for(unsigned int j=0; j < m; j++)
        {
            /** 
             * Initialize the position of the point in the space here.
             * mesh->P[i][j] = newVector(x, y, z);
             * 
             * For now we give them the same position as their coordinate on the grid, with an initial velocity of zero
             * */ 
            mesh->P[i][j] = newVector(i, j, 0.0f);
            mesh->V[i][j] = newVector(0.0f, 0.0f, 0.0f);
            mesh->A[i][j] = newVector(0.0f, 0.0f, 0.0f);

            fillSprings(mesh->springs, &spring_index, i, j, n, m);
        } 
    }

    log_info("Mesh Created!");
}

Vector** computeForce(Mesh* m)
{
    //TODO: test
    return NULL;
}

Vector** computeAcceleration(Mesh* m, float delta_t)
{
    return NULL;
}

Vector** computeVelocity(Mesh* m, float delta_t)
{
    return NULL;
}

/** 
 * Convert a mesh to set point of points and lines in a vtk file
 */ 
void convert_mesh_to_vtk(const Mesh *mesh, const char *output_filename)
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
    unsigned int springs_count = numberOfSprings(mesh->n , mesh->m);

    // print all springs
    fprintf(file, "LINES %u %u\n", springs_count, 3 * springs_count);
    for (unsigned int k = 0; k < springs_count; k++) {
        // Convert grid coordinates (i, j) to point indices
        unsigned int id1 = mesh->springs[k].ext_1.i * mesh->m + mesh->springs[k].ext_1.j;
        unsigned int id2 = mesh->springs[k].ext_2.i * mesh->m + mesh->springs[k].ext_2.j;
        fprintf(file, "2 %u %u\n", id1, id2);
    }

    fclose(file);
    log_info("Output file %s created", output_filename);
}

/**
 * Convert a Mesh into a a grid that can be used as surface easily.
 */
void convert_mesh_to_unstructure_grid_vtk(const Mesh *mesh, const char *output_filename)
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
            fprintf(file, "4 %u %u %u %u\n", id1, id2, id4, id3);
        }
    }

    // Write cell types
    // VTK_QUAD = 9
    fprintf(file, "CELL_TYPES %u\n", total_cells);
    for (unsigned int k = 0; k < total_cells; k++) {
        fprintf(file, "9\n");
    }

    fclose(file);
}
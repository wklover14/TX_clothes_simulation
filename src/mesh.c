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

    mesh->P     = (Vector**) malloc(n * sizeof(Vector*));
    mesh->P0    = (Vector**) malloc(n * sizeof(Vector*));
    mesh->V     = (Vector**) malloc(n * sizeof(Vector*));

    unsigned int N  = numberOfSprings(n, m); // total number of springs in the mesh
    mesh->springs   = (Spring*) malloc(N * sizeof(Spring));
    
    Vector origin = {0.0f, 0.0f, 0.0f};
    unsigned int spring_count = 0;

    for(unsigned int i = 0; i < n ; i++) 
    {
        mesh->P[i]  = (Vector*) malloc(m * sizeof(Vector));
        mesh->P0[i] = (Vector*) malloc(m * sizeof(Vector));
        mesh->V[i]  = (Vector*) malloc(m * sizeof(Vector));
        
        for(unsigned int j=0; j < m; j++)
        {
            /** 
             * Initialize the position of the point in the space here.
             * mesh->P[i][j] = newVector(x, y, z);
             * 
             * For now we give them the same position as their coordinate on the grid, with an initial velocity of zero
             * */ 
            mesh->P[i][j]   = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
            mesh->P0[i][j]  = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
            mesh->V[i][j]   = newVector(0.0f, 0.0f, 0.0f);

            fillSprings(mesh->springs, &spring_count, i, j, n, m);
        } 
    }

    log_info("Mesh Created!");
}


/**
 * Compute the next position of the mesh point.
 * For now, we ignore the fluid forces
 */
void updatePosition(Mesh* mesh, float delta_t)
{
    //TODO: test
    if (mesh == NULL || mesh->P == NULL)
    {
        log_error("Mesh or position empty! Exiting");
        return;
    }
    
    // The result shoul be but inside a specific Acceleration matrix, and after the computation of every forces, update other matrices
    Vector** acceleration = (Vector**) malloc(mesh->n * sizeof(Vector*));
    for(unsigned int i = 0; i < mesh->m; i++ )
    {
        acceleration[i] = (Vector*) malloc(mesh->m * sizeof(Vector));
    }
    
    for(unsigned int i = 0; i<mesh->n; i++)
    {

        for(unsigned int j=0; j<mesh->m; j++)
        {   
            // Fixed points 
            if( i == 0 && j== mesh->m-1)
            {
                continue;
            }
            if( i == mesh->n - 1 && j== mesh->m-1)
            {
                continue;
            }
            // Point i,j 
            Vector current_position     =  mesh->P[i][j];   // position of the current point i,j
            Vector original_position    =  mesh->P0[i][j];  // original position of the current point i,j
            
            // internal forces
            Vector f_int = newVector(0.0f, 0.0f, 0.0f);     // sum of internal forces

            unsigned int number_springs = 0;
            Spring* R = getPossibleSprings(i, j, mesh->n, mesh->m, &number_springs);

            for(unsigned int k=0; k < number_springs; k++) // iterate through all possible springs
            {
                Point target                    = R[k].ext_2;                       // R[k].ext_2 is the other extremity of the spring.
                Vector target_position          = mesh->P[target.i][target.j];
                Vector original_target_position = mesh->P0[target.i][target.j];

                Vector l_i_j_k_l = newVectorFromPoint(current_position, target_position);
                Vector l_0_i_j_k = newVectorFromPoint(original_position, original_target_position);
                
                Vector tmp = multVector( -1 * norm(l_0_i_j_k) / norm(l_i_j_k_l) , l_i_j_k_l);
                tmp = addVector(l_i_j_k_l, tmp); 
                tmp = multVector(R[k].stiffness, tmp);

                f_int = addVector(tmp, f_int);
            }

            // for(unsigned int k=0; k < number_springs; k++) // iterate through all possible springs
            // {
            //     Point target                    = R[k].ext_2;                       // R[k].ext_2 is the other extremity of the spring.
            //     Vector target_position          = mesh->P[target.i][target.j];
            //     Vector original_target_position = mesh->P0[target.i][target.j];

            //     Vector l_i_j_k_l = newVectorFromPoint(current_position, target_position);
            //     Vector l_0_i_j_k = newVectorFromPoint(original_position, original_target_position);

            //     float scal = R[k].stiffness * scalar_product(l_0_i_j_k, l_i_j_k_l) / norm(l_i_j_k_l);
                
            //     Vector tmp = multVector(scal, l_i_j_k_l);
            //     f_int = addVector(tmp, f_int);
            // }
            free(R);

            // gravity force that are applying
            Vector f_gr = {0.0f, -1.0f, 0.0f};

            // Viscous damping force
            Vector f_dis = multVector( -1 * C_DIS, mesh->V[i][j]);

            // Force from the fluid

            // Global result to the mesh
            Vector F = addVector(f_int, f_gr);
            F = addVector(F, f_dis);

            acceleration[i][j] = multVector(1.0f / Mu, F); // acceleration at t+delta_t
        }
    }

    // Update the speed and the position,
    for(unsigned int i = 0; i<mesh->n; i++)
    {
        for(unsigned int j=0; j<mesh->m; j++)
        {   
            mesh->V[i][j] = addVector(mesh->V[i][j], multVector(delta_t, acceleration[i][j]));
            mesh->P[i][j] = addVector(mesh->P[i][j], multVector(delta_t, mesh->V[i][j]));
        }
    }
    mesh->t += delta_t;
}

/** 
 * Convert a mesh into a set of points and lines in a vtk file
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
        fprintf(file, "2 %d %d\n", id1, id2);
    }

    fclose(file);
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
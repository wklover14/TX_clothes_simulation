#include "../include/mesh.h"

/**
 * Return true if a point is fixed and false if not
 */
bool isFixedPoint(unsigned int i, unsigned int j, Mesh* mesh, meshType type) {
    Vector origin = {0.0f, 0.0f, 0.0f};

    switch (type)
    {
        case FLAG: // only the two top points
            return (i == 0 && j == mesh->m - 1) || (i == mesh->n - 1 && j == mesh->m - 1);
            
        case TABLE: // The circle of center
                Vector center = { (origin.x + (mesh->n - 1) * SPACING) / 2.0f , origin.y, (origin.z + (mesh->m - 1) * SPACING) / 2.0f};               // center of the mesh
                float  distance = norm(newVectorFromPoint(center, mesh->P[i][j]));      // distance de l'origin
                return distance <= RADIUS;
            
        default:
            log_info("Type not handled");
            exit(EXIT_FAILURE);
    }
}

/**
 * Correctly allocate all attributes of a flag mesh, it fails if the mesh provided is NULL
 */
void initMesh(Mesh* mesh, unsigned int n,unsigned int m, meshType type)
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

    mesh->P     = getMatrix(n, m);
    mesh->P0    = getMatrix(n, m);
    mesh->V     = getMatrix(n, m);

    unsigned int N  = numberOfSprings(n, m); // total number of springs in the mesh
    mesh->springs   = (Spring*) malloc(N * sizeof(Spring));
    
    Vector origin = {0.0f, 0.0f, 0.0f};
    unsigned int spring_count = 0;

    for(unsigned int i = 0; i < n ; i++) 
    {   
        for(unsigned int j=0; j < m; j++)
        {
            /** 
             * Initialize the position of the point in the space here.
             * mesh->P[i][j] = newVector(x, y, z);
             * 
             * For now we give them the same position as their coordinate on the grid, with an initial velocity of zero
             * */ 
            switch (type)
            {
                case FLAG: // rectangle in the x,y plan
                    mesh->P[i][j]   = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->P0[i][j]  = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->V[i][j]   = newVector(0.0f, 0.0f, 0.0f);
                    break;

                case TABLE: // rectangle in the x,z plan
                    mesh->P[i][j]   = newVector(origin.x + i * SPACING, origin.y, origin.z + j * SPACING);
                    mesh->P0[i][j]  = newVector(origin.x + i * SPACING, origin.y, origin.z + j * SPACING);
                    mesh->V[i][j]   = newVector(0.0f, 0.0f, 0.0f);
                    break;

                default:
                    log_error("Type of mesh not handled");
                    freeMesh(mesh);
                    exit(EXIT_FAILURE);
            }
            fillSprings(mesh->springs, &spring_count, i, j, n, m);
        } 
    }

    log_info("Mesh Created!");
    Vector center = { (origin.x + (mesh->n - 1) * SPACING) / 2.0f , origin.y, (origin.z + (mesh->m - 1) * SPACING) / 2.0f};               // center of the mesh
    log_info("center = %s", VectorToString(center));
}


/**
 * Compute the next position of the mesh point.
 * For now, we ignore the fluid forces
 */
void updatePosition(Mesh* mesh, float delta_t, meshType type)
{
    // Init
    Vector** acc  = getMatrix(mesh->n, mesh->m); // acceleration matrex

    Vector f_gr = {0.0f, -9.81f, 0.0f}; // Gravity

    for(unsigned int i=0; i < mesh->n; i++) // For each point,
    {
        for(unsigned int j=0; j < mesh->m; j++)
        {
            if(isFixedPoint(i, j, mesh, type))
            {
                continue; 
            }
            // compute force F at i,j
            
            Vector current_position  = mesh->P[i][j];
            Vector original_position = mesh->P0[i][j]; 

            // Internal forces
            Vector f_int = {0.0f, 0.0f, 0.0f};
            
            unsigned int count = 0;
            Spring* R = getPossibleSprings(i, j, mesh->n, mesh->m, &count);

            for(unsigned int k=0; k<count; k++)
            {
                Point target = R[k].ext_2;
                Vector target_current_position = mesh->P[target.i][target.j]; // P[k][l] 
                Vector target_original_position = mesh->P0[target.i][target.j]; // P0[k][l]

                Vector l_i_j_k_l = newVectorFromPoint(target_current_position, current_position); // vector representing the spring

                float current_spring_len = norm(l_i_j_k_l);
                float original_spring_len = norm(newVectorFromPoint(original_position, target_original_position));

                float scal = - R[k].stiffness * (current_spring_len - original_spring_len);
                Vector direction = normalize(l_i_j_k_l);

                f_int = addVector(multVector(scal, direction),f_int);
            }
            free(R);

            // Viscous damping force
            Vector f_dis = multVector(-C_DIS, mesh->V[i][j]);

            Vector F = addVector(f_gr, f_int);
            F = addVector(F, f_dis);

            acc[i][j] = multVector( 1/Mu, F);
        }
    }

    // update the position of points based on the acceleartion
    for (unsigned int i = 0; i < mesh->n; i++)
    {
        for (unsigned int j = 0; j < mesh->m; j++)
        {
            if(isFixedPoint(i,j,mesh, type)){
                continue;
            }
            mesh->V[i][j] = addVector(mesh->V[i][j], multVector(delta_t, acc[i][j]));
            mesh->P[i][j] = addVector(mesh->P[i][j], multVector(delta_t, mesh->V[i][j]));
        }
        
    }
    freeMatrix(acc, mesh->n);
}

/**
 * De-allocate correctly a mesh
 */
void freeMesh(Mesh* mesh) {
    for (unsigned int i = 0; i < mesh->n; i++) {
        free(mesh->P[i]);
        free(mesh->V[i]);
        free(mesh->P0[i]);
    }
    free(mesh->P);
    free(mesh->V);
    free(mesh->P0);
    free(mesh->springs);
    free(mesh);
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
#include "../include/mesh.h"

const float    Mu               = 0.02f;                    // Mass of a point
const float    C_DIS            = 0.04f;                    // Damping coefficient
const float    C_VI             = 0.023f;                   // Viscous coefficient
const Vector   G                = {0.0f, 9.92f, 0.0f};      // Gravity
const float    STIFFNESS_1      = 1.0f;                     // Stiffess of a spring of lenght 1
const float    STIFFNESS_2      = 2.0f;                     // stiffess of a spring of lenght 2   

void initMesh(Mesh* mesh,unsigned int n,unsigned int m)
{
    if(mesh == NULL)
    {
        log_info("Mesh provided is empty, creating a new one");
        mesh = (Mesh*) malloc(sizeof(Mesh));
    } else {
        log_info("Creating new Mesh");
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

Spring newSpring(Point ext_a, Point ext_b, float stiff){
    Spring temp_spring = {ext_a, ext_b, stiff};
    return temp_spring;
}

// Compute the number of springs needed for the grid n lines, m columns
unsigned int numberOfSprings(unsigned int n,unsigned int m)
{
    unsigned int n_shear     = 2 * (n - 1) * (m - 1);               // number of springs on the diagonale, HINT: each point on the column has two diagonals except for the last and the first point.
    unsigned int n_flexion   = (m - 2) * n + (n - 2) * m;           // number of  2-lenght springs, HINT: each line has n-2 springs of len 2, m-2 for each column
    unsigned int n_struct    = (2 * n - 1) * (m - 1) + (n - 1);     // number of strings that make a square, HINT: for each point of a column we consider two springs forming a right angle (except for the last one), 
                                                                    // the point for the last column has only one spring to consider 
    return n_shear + n_flexion + n_struct ;                         // total number of springs in the mesh
}

// Add to the table springs all the possible springs from the index i,j in a n*m
void fillSprings(Spring* springs, unsigned int* spring_index, int i, int j, int n, int m)
{
    Point current = {i, j};

    // Structural Springs i+1, j and i,j+1
    if (i+1 < n)
    {
        Point ext_b = {i+1, j};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_1);
        (*spring_index)++;
    }

    if (j+1 < n)
    {
        Point ext_b = {i, j+1};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_1);
        (*spring_index)++;
    }

    // Shear Springs i+1, j+1 and structural spring
    if (i+1 < n && j+1 < m)
    {
        Point ext_b = {i+1, j+1};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_1);
        (*spring_index)++;
    }

    // Flexion springs i+2,j and i,j+2
    if (i+2 < n) 
    {
        Point ext_b = {i+2, j};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_1);
        (*spring_index)++;
    }

    if (j+2 < m)
    {
        Point ext_b = {i, j+2};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_1);
        (*spring_index)++;
    }
}
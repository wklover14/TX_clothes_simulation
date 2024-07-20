#include "../include/mesh.h"

const float    Mu      = 0.02f;                    // Mass of a point
const float    C_DIS   = 0.04f;                    // Damping coefficient
const float    C_VI    = 0.023f;                   // Viscous coefficient
const Vector   G       = {0.0f, 9.92f, 0.0f};      // Gravity

void initMesh(Mesh* mesh, int n, int m)
{
    if(mesh == NULL)
    {
        log_info("Mesh provided is empty, creating a new one");
        mesh = (Mesh*) malloc(sizeof(Mesh));
    } else
    {
        log_info("Creating new Mesh");
    }

    mesh->n = n;
    mesh->m = m;
    mesh->t = 0.0f; // the initial is zero    

    mesh->P = (Vector**) malloc(n * sizeof(Vector*));
    mesh->V = (Vector**) malloc(n * sizeof(Vector*));
    mesh->A = (Vector**) malloc(n * sizeof(Vector*));

    for(int i = 0; i < n ; i++) 
    {
        mesh->P[i] = (Vector*) malloc(m * sizeof(Vector));
        mesh->V[i] = (Vector*) malloc(m * sizeof(Vector));
        mesh->A[i] = (Vector*) malloc(m * sizeof(Vector));
        
        for(int j=0; j<m; j++)
        {
            /** 
             * Initialize the position of the point in the space here.
             * mesh->P[i][j] = newVector(x, y, z);
             * 
             * For now we give the them the same position as their coordinate on the grid, with an initial velocity of zero
             * */ 
            
            mesh->P[i][j] = newVector(i, j, 0.0f);
            mesh->V[i][j] = newVector(0.0f, 0.0f, 0.0f);
            mesh->A[i][j] = newVector(0.0f, 0.0f, 0.0f);
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
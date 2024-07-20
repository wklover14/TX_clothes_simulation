#include "../include/mesh.h"

const float    Mu      = 0.02f;                    // Mass of a point
const float    C_DIS   = 0.04f;                    // Damping coefficient
const float    C_VI    = 0.023f;                   // Viscous coefficient
const Vector   G       = {0.0f, 9.92f, 0.0f};      // Gravity

void initMesh(Mesh* m)
{
    if(m == NULL)
    {
        log_info("Mesh provided is empty, creating a new one");
        m = (Mesh*) malloc(sizeof(Mesh));
    }

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
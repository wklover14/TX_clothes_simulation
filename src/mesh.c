#include "../include/mesh.h"

/**
 * Return true if a point is fixed and false if not
 */
bool isFixedPoint(unsigned int i, unsigned int j, Mesh* mesh, meshType type) {
    Vector origin = {0.0f, 0.0f, 0.0f};

    switch (type)
    {
        case CURTAIN: // only the two top points
            return (i == 0 && j == mesh->m - 1) || (i == mesh->n - 1 && j == mesh->m - 1);
            
        case TABLE_CLOTH: // The circle of center
            Vector center = { (origin.x + (mesh->n - 1) * SPACING) / 2.0f , origin.y, (origin.z + (mesh->m - 1) * SPACING) / 2.0f};               // center of the mesh
            float  distance = norm(newVectorFromPoint(center, mesh->P[i][j]));      // distance de l'origin
            return distance <= RADIUS;
        
        case SOFT:
            // no points is fixed.
            return j >= mesh->n-2 ;

        default:
            log_error("Type not handled");
            exit(EXIT_FAILURE);
    }
}

/**
 * Ajust params according to the type.
 */
void customs_params(meshType type)
{
    switch (type)
    {
    case CURTAIN:
        // default params are OK.
        break;

    case TABLE_CLOTH:
        //default params are also ok
        break;

    case SOFT:
        M = 20;
        N = 20;
        SPACING = 0.2f;
        NB_UPDATES = 300;
        STEP = 1;
        ENERGY_THRESHOLD = 0.5f;
        DAMAGE_THRESHOLD = 2.50f;
        break;

    default:
        // if you want customs params for a new type : add it here
        break;
    }
}


/**
 * Correctly allocate all attributes of a flag mesh, it fails if the mesh provided is NULL
 */
void initMesh(Mesh* mesh, meshType type)
{
    if(mesh == NULL)
    {
        log_error("Mesh provided is empty!!");
        return;
    } 

    /**  
     *   customs params
     */
    customs_params(type);

    
    mesh->n = N;
    mesh->m = M;
    mesh->t = 0.0f; // the initial is zero    

    mesh->P     = getMatrix(N, M);
    mesh->P0    = getMatrix(N, M);
    mesh->V     = getMatrix(N, M);

    unsigned int nb_springs  = numberOfSprings(N, M); // total number of springs in the mesh
    mesh->springs   = (Spring*) malloc(nb_springs * sizeof(Spring));
    mesh->n_springs = nb_springs;
    
    Vector origin = {0.0f, 0.0f, 0.0f};
    unsigned int spring_count = 0;

    for(unsigned int i = 0; i < N ; i++) 
    {   
        for(unsigned int j=0; j < M; j++)
        {
            /** 
             * Initialize the position of the point in the space here.
             *
             * */ 
            switch (type)
            {
                case CURTAIN: // rectangle in the x,y 
                    mesh->P[i][j]   = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->P0[i][j]  = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->V[i][j]   = newVector(0.0f, 0.0f, 0.0f);
                    break;

                case TABLE_CLOTH: // rectangle in the x,z plan
                    mesh->P[i][j]   = newVector(origin.x + i * SPACING, origin.y, origin.z + j * SPACING);
                    mesh->P0[i][j]  = newVector(origin.x + i * SPACING, origin.y, origin.z + j * SPACING);
                    mesh->V[i][j]   = newVector(0.0f, 0.0f, 0.0f);
                    break;
                
                case SOFT: // rectangle in the x,y plan
                    mesh->P[i][j]   = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->P0[i][j]  = newVector(origin.x + i * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->V[i][j]   = newVector(0.0f, 0.0f, 0.0f);
                    break;

                default:
                    log_error("Type of mesh not handled");
                    freeMesh(mesh);
                    exit(EXIT_FAILURE);
            }
            fillSprings(mesh->springs, &spring_count, i, j, N, M);
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
    Vector** acc = getMatrix(mesh->n, mesh->m); // Acceleration matrix
    Vector f_gr = {0.0f, -0.1f, 0.0f}; // Gravity
    Vector u_fluid = {0.0f, 0.0f, 0.1f}; // Fluid 

    // Compute spring forces and update acceleration
    computeSpringForces(mesh, acc, type, delta_t);

    // Compute other forces (e.g., gravity, damping) and update acceleration
    computeOtherForces(mesh, acc, f_gr, u_fluid, type);

    // Update positions and velocities based on the acceleration
    updateMeshPositions(mesh, acc, delta_t, type);

    freeMatrix(acc, mesh->n); // Free allocated memory for acceleration matrix
}

/**
 * Compute forces applied to each spring and update acceleration matrix
 */
void computeSpringForces(Mesh* mesh, Vector** acc, meshType type, float delta_t)
{
    unsigned int number_springs = numberOfSprings(mesh->n, mesh->m);

    for (unsigned int k = 0; k < number_springs; k++)
    {
        Spring* current = &mesh->springs[k];

        if (current->isBreak) continue; // Skip broken springs

        Point A = current->ext_1;
        Point B = current->ext_2;

        // Get positions of the spring endpoints
        Vector current_position = mesh->P[A.i][A.j];
        Vector target_current_position = mesh->P[B.i][B.j];

        // Compute spring displacement vector and lengths
        Vector l_i_j_k_l = newVectorFromPoint(target_current_position, current_position);
        float current_spring_len = norm(l_i_j_k_l);
        float original_spring_len = norm(newVectorFromPoint(mesh->P0[A.i][A.j], mesh->P0[B.i][B.j]));

        // Compute spring force magnitude and direction
        float force_magnitude = -current->stiffness * (current_spring_len - original_spring_len);
        Vector direction = normalize(l_i_j_k_l);

        // Apply force to endpoint A
        if (!isFixedPoint(A.i, A.j, mesh, type))
        {
            acc[A.i][A.j] = addVector(acc[A.i][A.j], multVector(force_magnitude / Mu, direction));
        }

        // Apply force to endpoint B
        if (!isFixedPoint(B.i, B.j, mesh, type))
        {
            acc[B.i][B.j] = addVector(acc[B.i][B.j], multVector(-force_magnitude / Mu, direction));
        }

        float strain = (current_spring_len - original_spring_len) / original_spring_len;
        float potential_energy = 0.5f * current->stiffness * (current_spring_len - original_spring_len) * (current_spring_len - original_spring_len);

        // Update spring damage
        current->damage += strain * delta_t;

        // Check if the spring should break based on energy
        if (potential_energy > ENERGY_THRESHOLD || current->damage > DAMAGE_THRESHOLD)
        {
            current->isBreak = true;
            mesh->n_springs--;
        }
    }
}

/**
 * Compute other forces (e.g., gravity, damping) and update acceleration
 */
void computeOtherForces(Mesh* mesh, Vector** acc, Vector f_gr, Vector u_fluid, meshType type)
{
    for (unsigned int i = 0; i < mesh->n; i++)
    {
        for (unsigned int j = 0; j < mesh->m; j++)
        {
            if (isFixedPoint(i, j, mesh, type)) continue; // Skip fixed points

            // Compute viscous damping force
            Vector f_dis = multVector(-C_DIS, mesh->V[i][j]);

            // Fluid force
            Vector f_fluid = {0.0f, 0.0f, 0.0f};
            Vector n_ij;

            // compute the normal force
            unsigned int nb_springs = 0;
            Spring* R = getPossibleSprings(i, j, mesh->n, mesh->m, &nb_springs);
            // take the first and the last and do the cross product.
            if (nb_springs > 2)
            {
                // First vector 
                Vector a = newVectorFromPoint(mesh->P[R[0].ext_1.i][R[0].ext_1.j],mesh->P[R[0].ext_2.i][R[0].ext_2.j]);

                // second Vector
                Vector b = newVectorFromPoint(mesh->P[R[nb_springs - 1].ext_1.i][R[nb_springs - 1].ext_1.j],mesh->P[R[nb_springs - 1].ext_2.i][R[nb_springs - 1].ext_2.j]);

                // check of the colinearity
                if(!isCollinear(a, b))
                {
                    n_ij = normalize(crossProduct(a, b));
                } else {
                    log_error("A taccc");
                }

            } 
            else
            {
                log_error("Cannot find correct possible springs for %d, %d", i, j);
            }

            float scal = scalar_product(n_ij, addVector(u_fluid, multVector(-1.0f, mesh->V[i][j])));
            f_fluid = multVector(scal*C_VI, n_ij);

            // Sum all forces acting on the point
            Vector F = addVector(f_gr, f_dis);                      // Gravity and damping
            F = addVector(F, computeAddForces(mesh, type, i, j));   // Additional forces based on mesh type
            F = addVector(F, f_fluid);

            // Update acceleration with the total force
            acc[i][j] = addVector(acc[i][j], multVector(1 / Mu, F));
        }
    }
}

/**
 * Update positions and velocities based on the acceleration
 */
void updateMeshPositions(Mesh* mesh, Vector** acc, float delta_t, meshType type)
{
    for (unsigned int i = 0; i < mesh->n; i++)
    {
        for (unsigned int j = 0; j < mesh->m; j++)
        {
            if (isFixedPoint(i, j, mesh, type)) continue; // Skip fixed points

            // Update velocity and position of the point
            mesh->V[i][j] = addVector(mesh->V[i][j], multVector(delta_t, acc[i][j]));
            mesh->P[i][j] = addVector(mesh->P[i][j], multVector(delta_t, mesh->V[i][j]));
        }
    }
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
 * Return a vector of force depending on the meshtype, these are customs forces that are not listed in the 
 * initial documentation 
 */
Vector computeAddForces(Mesh* mesh, meshType type, unsigned int i, unsigned int j)
{
    Vector res = {0, 0, 0};
    float coef = 2.0f;

    switch (type)
    {
        case CURTAIN:
            /* No additionnal forces to add*/
            break;
            
        case TABLE_CLOTH:
            /* No additionnal forces to add*/
            break;
        
        case SOFT:
            res.y = 0.1f;

            // Apply a force to point onto the left and right edge of the soft
            // if( i <= 3 )
            // {
            //     res.x += -coef;
            //     // if( j >= mesh->m - 4)
            //     //     res.z += coef;
            // } 
            // else if (i >= mesh-> n-4)
            // {
            //     res.x += coef;
            // }  
            
            // apply a force to little square a t the bottom right of the soft
            if ( j <= 4)
            {
                res.y -= coef;
            }

            break;

        default:
            break;
    }

    // Return null vector
    return res;
}
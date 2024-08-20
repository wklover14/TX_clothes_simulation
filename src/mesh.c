#include "../include/mesh.h"
#include <omp.h>

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
            return false;

        case FLAG: // only the left edge
            return i == 0;

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
        STIFFNESS_H = 15.0f;
        STIFFNESS_V = 20.0f;
        STIFFNESS_D = 40.0f;
        M = 20;
        N = 20;
        SPACING = 0.2f;
        NB_UPDATES = 300;
        STEP = 1;
        ENERGY_THRESHOLD = 1.50f;
        DAMAGE_THRESHOLD = 4.50f;
        RADIUS = 0.2f;

        break;

    case FLAG:
        // default params are OK
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

                case FLAG: // rectangle in the x,y
                    mesh->P[i][j]   = newVector(origin.x + i * 1.4f * SPACING, origin.y + j * SPACING, origin.z);
                    mesh->P0[i][j]  = newVector(origin.x + i * 1.4f * SPACING, origin.y + j * SPACING, origin.z);
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
    
    char* center_string = VectorToString(center);
    log_info("center = %s", center_string);
    free(center_string);
}


/**
 * Compute the next position of the mesh point.
 * For now, we ignore the fluid forces
 */
void updatePosition(Mesh* mesh, float delta_t, meshType type)
{
    Vector** acc = getMatrix(mesh->n, mesh->m); // Acceleration matrix

    // Compute spring forces and update acceleration
    computeSpringForces(mesh, acc, type, delta_t);

    // Compute position and velocity for every point
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < mesh->n; i++) {
        for (int j = 0; j < mesh->m; j++) {
            if (!isFixedPoint(i, j, mesh, type)) {
                Vector f_dis = multVector(-C_DIS, mesh->V[i][j]);                      // Viscous damping force
                Vector f_fluid = computeFluidForce(mesh, i, j, FLUID);                // fluid force
                Vector F = addVector(GRAVITY, f_dis);
                F = addVector(F, computeAddForces(mesh, type, i, j));
                F = addVector(F, f_fluid);

                acc[i][j] = addVector(acc[i][j], multVector(1 / Mu, F));

                mesh->V[i][j] = addVector(mesh->V[i][j], multVector(delta_t, acc[i][j]));
                mesh->P[i][j] = addVector(mesh->P[i][j], multVector(delta_t, mesh->V[i][j]));
            }
        }
    }

    freeMatrix(acc, mesh->n); // Free allocated memory for acceleration matrix
}

/**
 * Compute forces applied to each spring and update acceleration matrix
 */
void computeSpringForces(Mesh* mesh, Vector** acc, meshType type, float delta_t)
{
    unsigned int number_springs = numberOfSprings(mesh->n, mesh->m);

    // Start a parallel region; each thread will have its own local acceleration matrix
    #pragma omp parallel
    {
        // Thread-local acceleration matrix to avoid conflicts with other threads
        Vector** local_acc = getMatrix(mesh->n, mesh->m);

        // Parallel for loop to iterate over all springs in the mesh
        #pragma omp for
        for (unsigned int k = 0; k < number_springs; k++)
        {
            Spring* current = &mesh->springs[k];

            // Skip broken springs
            if (current->isBreak) continue;

            // Get the points connected by the spring
            Point A = current->ext_1;
            Point B = current->ext_2;

            // Get the current positions of the spring's endpoints
            Vector current_position = mesh->P[A.i][A.j];
            Vector target_current_position = mesh->P[B.i][B.j];

            // Compute the vector representing the spring's displacement
            Vector l_i_j_k_l = newVectorFromPoint(target_current_position, current_position);
            float current_spring_len = norm(l_i_j_k_l);

            // Compute the original length of the spring
            float original_spring_len = norm(newVectorFromPoint(mesh->P0[A.i][A.j], mesh->P0[B.i][B.j]));

            // Calculate the spring force magnitude using Hooke's Law
            float force_magnitude = -current->stiffness * (current_spring_len - original_spring_len);

            // Calculate the direction of the spring force
            Vector direction = normalize(l_i_j_k_l);

            // Apply force to endpoint A if it's not a fixed point
            if (!isFixedPoint(A.i, A.j, mesh, type))
            {
                // Accumulate the force into the thread-local acceleration matrix
                local_acc[A.i][A.j] = addVector(local_acc[A.i][A.j], multVector(force_magnitude / Mu, direction));
            }

            // Apply force to endpoint B if it's not a fixed point
            if (!isFixedPoint(B.i, B.j, mesh, type))
            {
                // Accumulate the opposing force into the thread-local acceleration matrix
                local_acc[B.i][B.j] = addVector(local_acc[B.i][B.j], multVector(-force_magnitude / Mu, direction));
            }

            // Calculate strain and potential energy for damage and breakage checks
            float strain = (current_spring_len - original_spring_len) / original_spring_len;
            float potential_energy = 0.5f * current->stiffness * (current_spring_len - original_spring_len) * (current_spring_len - original_spring_len);

            // Atomically update the damage on the spring
            #pragma omp atomic
            current->damage += strain * delta_t;

            // Check if the spring should break based on energy or damage thresholds
            
            // Method using a len criteria
            // float ratio = current_spring_len / original_spring_len ;
            // if ( ratio >= 1.5f  )
            // {
            //     #pragma omp critical
            //     {
            //         current->isBreak = true;
            //         mesh->n_springs--;
            //     }
            // }
            
            // Method using a more complex criteria based on energy and damage
            if (potential_energy > ENERGY_THRESHOLD || current->damage > DAMAGE_THRESHOLD)
            {
                // Critical section to safely mark the spring as broken and decrease the spring count
                #pragma omp critical
                {
                    current->isBreak = true;
                    mesh->n_springs--;
                }
            }
        }

        // Critical section to merge thread-local acceleration matrices into the global matrix
        #pragma omp critical
        {
            // Iterate over the entire matrix and add local_acc to the global acc
            for (int i = 0; i < mesh->n; i++) {
                for (int j = 0; j < mesh->m; j++) {
                    acc[i][j] = addVector(acc[i][j], local_acc[i][j]);
                }
            }
        }

        // Free the memory allocated for the thread-local acceleration matrix
        freeMatrix(local_acc, mesh->n);
    }
}

/**
 * Compute the force generated by the fluid at point i,j
 */
Vector computeFluidForce(Mesh* mesh, unsigned int i, unsigned int j, Vector u_fluid)
{
    Vector f_fluid = {0.0f, 0.0f, 0.0f};
    Vector n_ij = {0.0f, 0.0f, 0.0f};

    // Compute the normal force
    unsigned int nb_springs = 0;
    Spring* R = getPossibleSprings(i, j, mesh->n, mesh->m, &nb_springs);

    if (nb_springs > 2)
    {
        // First vector 
        Vector a = newVectorFromPoint(mesh->P[R[0].ext_1.i][R[0].ext_1.j], mesh->P[R[0].ext_2.i][R[0].ext_2.j]);

        // Second Vector
        Vector b = newVectorFromPoint(mesh->P[R[nb_springs - 1].ext_1.i][R[nb_springs - 1].ext_1.j], 
                                      mesh->P[R[nb_springs - 1].ext_2.i][R[nb_springs - 1].ext_2.j]);

        // Check for colinearity
        if(!isCollinear(a, b))
        {
            n_ij = normalize(crossProduct(a, b));
        }
        else
        {
            log_error("Cannot compute normal for point %d, %d", i, j);
        }
    } 
    else
    {
        log_error("Cannot find correct possible springs for %d, %d", i, j);
    }

    float scal = scalar_product(n_ij, addVector(u_fluid, multVector(-1.0f, mesh->V[i][j])));
    f_fluid = multVector(scal * C_VI, n_ij);

    free(R);

    return f_fluid;
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
            if( i < mesh->n/2 )
            {
                res.x += - coef * mesh->P[i][j].y;
                // if( j >= mesh->m - 4)
                //     res.z += coef;
            } 
            else
            {
                 res.x += coef * mesh->P[i][j].y;
            }  
            
            // apply a force to little square a t the bottom right of the soft
            // if ( j <= 4)
            // {
            //     res.y -= coef;
            // }


            break;
        case FLAG:
            /* No additionnal forces to add*/
            break;

        default:
            break;
    }

    // Return null vector
    return res;
}
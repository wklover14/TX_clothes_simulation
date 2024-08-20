/**
*************************************************************
* @file     mesh.h
* @author   WATCHO KEUGONG Gabby Pavel, gwathok@etu.utc.fr
* @date     20/07/2024
* @brief
*************************************************************
*/

#ifndef MESH_H
#define MESH_H

/************************************
 * INCLUDES
 ************************************/
#include "space.h"
#include "log.h"
#include "spring.h"
#include "params.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

typedef struct Mesh
{
    unsigned int    n;          // number of lines
    unsigned int    m;          // number of columns

    float           t;          // the time at which position P are calculated 
    Vector**        P;          // Coordinate in the space at t time, should be used for rendering
    Vector**        V;          // Velocity matrix n*m

    Vector**        P0;         // Initial position matrix
    
    Spring*         springs;    // list of springs of the mesh, refered as R in the litterature
    unsigned int    n_springs;  // number of non-break springs in the mesh
}Mesh;

typedef enum {
    CURTAIN,                    // A curtain(rideau) in the x, y plan with two points fixed   
    TABLE_CLOTH,                // Square Table on the plan x, z with a circular table. The edge is tree times the radius.
    SOFT,                        // A cloth in the x, y plan which it strech untill it breaks
    FLAG
} meshType;

/************************************
 * EXPORTED VARIABLES AND CONST
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

bool isFixedPoint(unsigned int, unsigned int, Mesh* mesh, meshType);
// void customs_params(meshType type);

void initMesh(Mesh*, meshType);
void updatePosition(Mesh*, float, meshType);
void computeSpringForces(Mesh*, Vector**, meshType, float);
void freeMesh(Mesh*);

Vector computeAddForces(Mesh*, meshType, unsigned int, unsigned int);
Vector computeFluidForce(Mesh* , unsigned int , unsigned int , Vector );

#endif // !MESH_H

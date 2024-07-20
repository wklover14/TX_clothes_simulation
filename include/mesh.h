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
#include <stdlib.h>
#include "log.h"
#include <stdlib.h>

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

// A spring is a segment define by two points and a stiffness
typedef struct Spring{
    Point  ext_1;               // One extremum point
    Point  ext_2;               // The other extremum point
    float  stiffness;
} Spring;


typedef struct Mesh
{
    unsigned int    n;          // number of lines
    unsigned int    m;          // number of columns

    float           t;          // the time at which position P are calculated 
    Vector**        P;          // Coordinate in the space, should be used for rendering
    Vector**        V;          // Velocity matrix n*m
    Vector**        A;          // Acceleration matrix n*m
    
    Spring*         springs;    // list of springs of the mesh, refered as R in the litterature
}Mesh;

/************************************
 * EXPORTED VARIABLES AND CONST
 ************************************/
extern const float    Mu;       // Mass of a point
extern const float    C_DIS;    // Damping coefficient
extern const float    C_VI;     // Viscous coefficient
extern const Vector   G;        // Gravity

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void initMesh(Mesh*, int, int);
Vector** computeForce(Mesh*);
Vector** computeAcceleration(Mesh*, float);
Vector** computeVelocity(Mesh*, float);

#endif // !MESH_H

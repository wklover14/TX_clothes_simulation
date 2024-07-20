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
    float  stiffness = 0;
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
 * EXPORTED VARIABLES
 ************************************/
extern float    Mu      = 0.02f;                    // Mass of a point
extern float    C_DIS   = 0.04f;                    // Damping coefficient
extern float    C_VI    = 0.023f;                   // Viscous coefficient
extern Vector   G       = {0.0f, 9.92f, 0.0f};      // Gravity

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
Vector** computeForce(Mesh*);
Vector** computeAcceleration(Mesh*, float);
Vector** computeVelocity(Mesh*, float);

#endif // !MESH_H

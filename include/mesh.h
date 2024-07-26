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
#include "const.h"
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
}Mesh;

/************************************
 * EXPORTED VARIABLES AND CONST
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

bool isFixedPoint(unsigned int, unsigned int, Mesh* mesh);

void initMesh(Mesh*, unsigned int,unsigned int);
void updatePosition(Mesh*, float);
void freeMesh(Mesh*);

void convert_mesh_to_vtk(const Mesh *mesh, const char *output_filename);
void convert_mesh_to_unstructure_grid_vtk(const Mesh *mesh, const char *output_filename);

#endif // !MESH_H

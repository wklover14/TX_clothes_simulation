/**
*************************************************************
* @file     space.h
* @author   WATCHO KEUGONG Gabby Pavel, gwathok@etu.utc.fr
* @date     20/07/2024
* @brief
*************************************************************
*/

#ifndef SPACE_H
#define SPACE_H

/************************************
 * INCLUDES
 ************************************/

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

typedef struct Vector
{
    float x;
    float y;
    float z;
} Vector;


typedef struct Point
{   
    // Coordinate on the mesh grid
    unsigned int i;
    unsigned int j;
} Point;

/************************************
 * EXPORTED VARIABLES
 ************************************/

extern const Vector   G;                    // Gravity


/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

float   scalar_product(Vector , Vector );
float   norm(Vector a);
Vector  multVector(float , Vector);
char*   PointToString(Point);
char*   VectorToString(Vector);
Vector  newVector(float, float, float);
Vector  newVectorFromPoint(Vector, Vector);
Vector  addVector(Vector, Vector);
void    freeMatrix(Vector**, unsigned int);
#endif // !SPACE_H



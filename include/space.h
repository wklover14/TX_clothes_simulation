/**
*************************************************************
* @file     ${file_name}
* @author   WATCHO KEUGONG Gabby Pavel
* @date     ${date}
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
typedef struct Point
{
    int i;
    int j;
} Point;

typedef struct Vector
{
    float x;
    float y;
    float z;
} Vector;

/************************************
 * EXPORTED VARIABLES
 ************************************/


/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

float   scalar_product(Vector , Vector );
float   norm(Vector a);
Vector  mult(float , Vector);
char*   PointToString(Point);
char*   VectorToString(Vector);

#endif // !SPACE_H



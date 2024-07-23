#include "../include/space.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "space.h"


const Vector   G                = {0.0f, 9.92f, 0.0f};      // Gravity 

/**
 * The scalar product between a and b
 */
float  scalar_product(Vector a, Vector b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); 
}

/**
 * The euclidian norm of the vector a
 */
float norm(Vector a)
{
    return sqrt( pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2) );
}

/**
 * Multiply the vector a by the scalar alpha and return the new vector
 */
Vector  mult(float alpha, Vector a)
{
    Vector res;
    res.x = alpha * a.x;
    res.y = alpha * a.y;
    res.z = alpha * a.z;
    return res;
}

/**
 * Convert a point into a string
 */
char*   PointToString(Point p)
{
    // Allocate enough memory for the string representation
    // Assuming the format "(x, y)" with some extra space for safety
    char* result = (char*) malloc(15 * sizeof(char));
    if (result == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    snprintf(result, 15, "(%d, %d)", p.i, p.j);
    return result;
}

/**
 * Convert a vector into a string
 */
char*   VectorToString(Vector v)
{
    // Allocate enough memory for the string representation
    // Assuming the format "Vector(x: x, y: y, z: z)" with some extra space for safety
    char* result = (char*)malloc(25 * sizeof(char));
    if (result == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    snprintf(result, 25, "[%.2f, %.2f, %.2f]", v.x, v.y, v.z);
    return result;
}

/**
 * Generate a new vector 
 */
Vector newVector(float x, float y, float z)
{
    Vector tmp = {x, y, z};
    return tmp;
}
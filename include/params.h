/**
*************************************************************
* @file     params.h
* @author   WATCHO KEUGONG Gabby Pavel, gwathok@etu.utc.fr
* @date     31/07/2024
* @brief
*************************************************************
*/

#ifndef PARAMS_H
#define PARAMS_H

/************************************
 * EXPORTED VARIABLES
 ************************************/

// SPACE CONST
extern float    SPACING;                                           // Standard spacing between two points on the same axis

// MESH CONST
extern float    Mu;                                                // Mass of a point
extern float    C_DIS;                                             // Damping coefficient
extern float    C_VI;                                              // Viscous coefficient

// SPRINGS CONST
extern int      MAX_SPRINGS_PER_POINT;
extern float    STIFFNESS_H;                                       // Stiffness of a horizontal spring
extern float    STIFFNESS_V;                                       // Stiffness of a vertical spring
extern float    STIFFNESS_D;                                       // Stiffness of a diagonal spring

// FIXED POINT
extern float    RADIUS;                                            // For the table, the radius of the circle containing fixed points

#endif // PARAMS_H

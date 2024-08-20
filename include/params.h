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

#include "space.h"
/************************************
 * EXPORTED VARIABLES
 ************************************/

// SPACE CONST
extern unsigned int M, N;                                          // Matrix dimensions
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
extern float    ENERGY_THRESHOLD;                                  // Maximum energy a spring can accumulate
extern float    DAMAGE_THRESHOLD;                                  // Maximum damage a spring can take

// FIXED POINT
extern float    RADIUS;                                            // For the table, the radius of the circle containing fixed points

// SIMULATION
extern float           DELTA_T;                                    // Time interval between two consecutives updates.                                           
extern unsigned int    NB_UPDATES;                                 // Total numbers of updates
extern int             STEP;                                       // Step used for files generation, as instance a step of 10 means 1 file generated every 10 update

extern Vector   GRAVITY;
extern Vector   FLUID;
#endif // PARAMS_H

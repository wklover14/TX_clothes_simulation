/**
*************************************************************
* @file     const.h
* @author   WATCHO KEUGONG Gabby Pavel, gwathok@etu.utc.fr
* @date     26/07/2024
* @brief
*************************************************************
*/

#ifndef CONST_H
#define CONST_H 

/************************************
 * MACROS AND DEFINES
 ************************************/

// SPACE CONST
#define     SPACING                     0.2f                           //   standard spacing between two points on the same axis

// MESH CONST
#define     Mu                          1.00f                          // Mass of a point
#define     C_DIS                       0.99f                          // Damping coefficient
#define     C_VI                        0.023f                         // Viscous coefficient

// SPRINGS CONST
#define     MAX_SPRINGS_PER_POINT       12
#define     STIFFNESS_H                 10.01f                          // stiffess of a horizontal spring
#define     STIFFNESS_V                 10.01f                          // stiffess of a vertical spring 
#define     STIFFNESS_D                 10.01f                          // stiffess of a diagonal spring

#endif // !CONST_H

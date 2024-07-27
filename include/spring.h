/**
*************************************************************
* @file     spring.h
* @author   WATCHO KEUGONG Gabby Pavel, gwathok@etu.utc.fr
* @date     23/07/2024
* @brief
*************************************************************
*/

#ifndef SPRING_H
#define SPRING_H

/************************************
 * INCLUDES
 ************************************/
#include "space.h"
#include "log.h"
#include "const.h"
#include <stdlib.h>

/************************************
 * MACROS AND DEFINES
 ************************************/


/************************************
 * TYPEDEFS
 ************************************/

// A spring is a segment define by two points and a stiffness
typedef struct 
{
    Point  ext_1;               // One extremum point
    Point  ext_2;               // The other extremum point
    float  stiffness;      
} Spring;


/************************************
 * EXPORTED VARIABLES AND CONST
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

Spring newSpring(Point, Point, float);
unsigned int numberOfSprings(unsigned int,unsigned int);
void fillSprings(Spring* springs, unsigned int* spring_index,int i, int j, int n, int m);
Spring* getPossibleSprings(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int*);
#endif // !SPRING_H
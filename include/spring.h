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
#include <stdlib.h>

/************************************
 * MACROS AND DEFINES
 ************************************/

#define MAX_SPRINGS_PER_POINT 12
/************************************
 * TYPEDEFS
 ************************************/

// A spring is a segment define by two points and a stiffness
typedef struct 
{
    Point  ext_1;               // One extremum point
    Point  ext_2;               // The other extremum point
    float  stiffness;
    float  natural_lenght;      
} Spring;


/************************************
 * EXPORTED VARIABLES AND CONST
 ************************************/
extern const float    STIFFNESS_1;           // stiffess of a spring of lenght 1
extern const float    STIFFNESS_2;           // stiffess of a spring of lenght 2
extern const float    NATURAL_LEN_1;
extern const float    NATURAL_LEN_2;


/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

Spring newSpring(Point, Point, float, float);
unsigned int numberOfSprings(unsigned int,unsigned int);
void fillSprings(Spring* springs, unsigned int* spring_index, int i, int j, int n, int m);
Spring* getPossibleSprings(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int*);
#endif // !SPRING_H
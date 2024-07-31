#include "params.h"

// Initialize variables with default values, which work for the CURTAIN and TABLE_CLOTH cases.
unsigned int        M                               = 50;                                    
unsigned int        N                               = 50;                                    
float               SPACING                         = 1.0f;

// MESH CONST
float               Mu                              = 1.00f;
float               C_DIS                           = 0.90f;
float               C_VI                            = 0.0f;

// SPRINGS CONST
int                 MAX_SPRINGS_PER_POINT           = 12;
float               STIFFNESS_H                     = 20.0f;
float               STIFFNESS_V                     = 20.0f;
float               STIFFNESS_D                     = 20.0f;

// RADIUS
float               RADIUS                          = 13.0f;

float               DELTA_T                         = 0.1f;
unsigned int        NB_UPDATES                      = 10000;
int                 STEP                            = 30;
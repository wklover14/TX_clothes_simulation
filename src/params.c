#include "params.h"

// Initialize variables with default values, which work for the CURTAIN and
// TABLE_CLOTH cases.
unsigned int M = 50;
unsigned int N = 50;
float SPACING = 1.0f;

// MESH CONST
float Mu = 1.00f;
float C_DIS = 0.90f;
float C_VI = 0.1f;

// SPRINGS CONST
int MAX_SPRINGS_PER_POINT = 12;
float STIFFNESS_H = 20.0f;
float STIFFNESS_V = 20.0f;
float STIFFNESS_D = 20.0f;
float ENERGY_THRESHOLD = 100000.0f; // 0.90f correct value
float DAMAGE_THRESHOLD = 100000.0f;

// RADIUS
float RADIUS = 13.0f;

float DELTA_T = 0.1f;
unsigned int NB_UPDATES = 5000;
int STEP = 20;

Vector GRAVITY = {0.0f, -0.1f, 0.0f};
Vector FLUID = {0.0f, 0.0f, 0.1f};
#include "../include/space.h"
#include "space.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Return true if the two vectors are collinear
 */
bool isCollinear(Vector a, Vector b) {
  double epsilon = 1e-3; // Internal
  double scalarProduct = scalar_product(a, b);
  return (fabs(scalarProduct - 1.0) < epsilon) ||
         (fabs(scalarProduct + 1.0) < epsilon);
}

/**
 * The scalar product between a and b
 */
float scalar_product(Vector a, Vector b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/**
 * The euclidian norm of the vector a
 */
float norm(Vector a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

/**
 * Return the vector of norm one associated
 */
Vector normalize(Vector a) {
  float n = norm(a);
  if (n == 0.0f) {
    return a;
  }
  Vector res = multVector(1.0f / n, a);
  return res;
}

/**
 * multVectoriply the vector a by the scalar alpha and return the new vector
 */
Vector multVector(float alpha, Vector a) {
  Vector res;
  res.x = alpha * a.x;
  res.y = alpha * a.y;
  res.z = alpha * a.z;
  return res;
}

/**
 * Convert a point into a string
 */
char *PointToString(Point p) {
  // Allocate enough memory for the string representation
  // Assuming the format "(x, y)" with some extra space for safety
  char *result = (char *)malloc(15 * sizeof(char));
  if (result == NULL) {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  snprintf(result, 15, "(%d, %d)", p.i, p.j);
  return result;
}

/**
 * Convert a vector into a string
 */
char *VectorToString(Vector v) {
  // Allocate enough memory for the string representation
  // Assuming the format "Vector(x: x, y: y, z: z)" with some extra space for
  // safety
  char *result = (char *)malloc(25 * sizeof(char));
  if (result == NULL) {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  snprintf(result, 25, "[%.2f, %.2f, %.2f]", v.x, v.y, v.z);
  return result;
}

/**
 * Generate a new vector
 */
Vector newVector(float x, float y, float z) {
  Vector tmp = {x, y, z};
  return tmp;
}

/**
 * Return the vector ->ab
 */
Vector newVectorFromPoint(Vector a, Vector b) {
  Vector res = newVector(b.x - a.x, b.y - a.y, b.z - a.z);
  return res;
}

/**
 * Return a + b
 */
Vector addVector(Vector a, Vector b) {
  Vector res = {a.x + b.x, a.y + b.y, a.z + b.z};
  return res;
}

/**
 * Deallocate the memory used for a matrix with n lines
 */
void freeMatrix(Vector **mesh, unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    free(mesh[i]);
  }
  free(mesh);
}

void printMatrix(Vector **mat, unsigned int n, unsigned int m) {
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < m; j++) {
      printf("%s\t", VectorToString(mat[i][j]));
    }
    printf("\n");
  }
}

/**
 * Return a mtrix of n lines and m colums
 */
Vector **getMatrix(unsigned int n, unsigned int m) {
  Vector **res = (Vector **)malloc(n * sizeof(Vector *));
  for (unsigned int i = 0; i < n; i++) {
    res[i] = (Vector *)malloc(m * sizeof(Vector));
    for (int j = 0; j < m; j++) {
      res[i][j] = newVector(0, 0, 0);
    }
  }
  return res;
}

/**
 * Return the cross product of a and b
 */
Vector crossProduct(Vector a, Vector b) {
  Vector result = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x};
  return result;
}

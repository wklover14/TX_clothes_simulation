#include "../include/spring.h"

const float    STIFFNESS_H      = 0.00f;                     // stiffess of a structural spring
const float    STIFFNESS_V      = 0.02f;                     // stiffess of a shear spring  
const float    STIFFNESS_D      = 0.00f;                     // stiffess of a flexion spring  

/** 
 * Return a spring
 */
Spring newSpring(Point ext_a, Point ext_b, float stiff){
    Spring temp_spring = {ext_a, ext_b, stiff};
    return temp_spring;
}

/** 
 * Compute the number of springs needed for the grid n lines, m columns
 */ 
unsigned int numberOfSprings(unsigned int n,unsigned int m)
{
    unsigned int n_shear     = 2 * (n - 1) * (m - 1);               // number of springs on the diagonale, HINT: each point on the column has two diagonals except for the last and the first point.
    unsigned int n_flexion   = (m - 2) * n + (n - 2) * m;           // number of  2-lenght springs, HINT: each line has n-2 springs of len 2, m-2 for each column
    unsigned int n_struct    = (2 * n - 1) * (m - 1) + (n - 1);     // number of strings that make a square, HINT: for each point of a column we consider two springs forming a right angle (except for the last one), 
                                                                    // the point for the last column has only one spring to consider 
    return n_shear + n_flexion + n_struct ;                         // total number of springs in the mesh
}

/**
 * Add to the table springs all the possible springs from the index i,j in a n*m matrix
 */ 
void fillSprings(Spring* springs, unsigned int* spring_index, int i, int j, int n, int m)
{
    Point current = {i, j};

    // Structural Springs i+1, j and i,j+1
    if (i+1 < n)
    {
        Point ext_b = {i+1, j};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_H);
        (*spring_index)++;
    }

    if (j+1 < m)
    {
        Point ext_b = {i, j+1};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_V);
        (*spring_index)++;
    }

    // Shear Springs i+1, j+1 and structural spring
    if (i+1 < n && j+1 < m)
    {
        Point ext_b = {i+1, j+1};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_D);
        (*spring_index)++;
    }

    if (i-1>=0 && j+1<m)
    {
        Point ext_b = {i-1, j+1};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_D);
        (*spring_index)++;
    }

    // Flexion springs i+2,j and i,j+2
    if (i+2 < n) 
    {
        Point ext_b = {i+2, j};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_H);
        (*spring_index)++;
    }

    if (j+2 < m)
    {
        Point ext_b = {i, j+2};
        springs[*spring_index] = newSpring(current, ext_b, STIFFNESS_V);
        (*spring_index)++;
    }
}

/**
 * Return every string that are connected to i,j in a n*m matrix, 
 * Not efficient for the moment, should be replaced by using different kind of storage springs in the Mesh structure.
 */
Spring* getPossibleSprings(unsigned int i, unsigned int j, unsigned int n, unsigned int m, unsigned int* count)
{
    Spring* res = (Spring*) malloc(MAX_SPRINGS_PER_POINT * sizeof(Spring)); // 12 springs at most for a single point
    
    unsigned int spring_index = 0;
    Point current = {i,j};

    // Add springs for immediate neighbors (including diagonals)
    for(int i_iter = (int)i-1; i_iter <= (int)i+1; i_iter++) // we use the cast in int to allow negative value
    {
        for(int j_iter = (int)j -1; j_iter <=  (int)j+1; j_iter++)
        {
            if(i_iter == i && j_iter == j) continue; // skip the current point
            
            if( i_iter >= 0 && i_iter < n && j_iter >= 0 && j_iter < m )
            {
                Point ext_b = {i_iter, j_iter};
                if (i_iter != i && j_iter != j) {
                    // Diagonal springs
                    res[spring_index] = newSpring(current, ext_b, STIFFNESS_D);
                } else if( i_iter != i) {
                    // Horizontal springs
                    res[spring_index] = newSpring(current, ext_b, STIFFNESS_H);
                } else {
                    // Vertical springs
                    res[spring_index] = newSpring(current, ext_b, STIFFNESS_V);
                }
                spring_index++;
            }
        }
    }

    // spring of len 2.
    if( i >= 2 ) 
    {
        Point ext_b = {i-2, j};
        res[spring_index] = newSpring(current, ext_b, STIFFNESS_H); 
        spring_index++;
    }

    if( i+2 < n)
    {
        Point ext_b = {i+2, j};
        res[spring_index] = newSpring(current, ext_b, STIFFNESS_H); 
        spring_index++;
    }

    if( j >= 2)
    {
        Point ext_b = {i, j-2};
        res[spring_index] = newSpring(current, ext_b, STIFFNESS_V); 
        spring_index++;
    }
    if( j+2 < m)
    {
        Point ext_b = {i, j+2};
        res[spring_index] = newSpring(current, ext_b, STIFFNESS_V); 
        spring_index++;
    }

    *count = spring_index;
    return res;
}

